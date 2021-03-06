/*****************************************************************************
 * Copyright (c) 2007 Piyush verma <piyush.verma@gmail.com>                  *
 * Copyright (c) 2008,2010 Niko Sams <niko.sams@gmail.com>                   *
 * Copyright (c) 2010 Milian Wolff <mail@milianw.de>                         *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *****************************************************************************/

#include "parsejob.h"

#include <QFile>
#include <QMimeDatabase>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QThread>

#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <language/duchain/parsingenvironment.h>
#include <language/duchain/declaration.h>
#include <language/backgroundparser/urlparselock.h>

#include "debug.h"
#include "parser/editorintegrator.h"
#include "parser/parsesession.h"
#include "csslanguagesupport.h"
#include "duchain/builders/declarationbuilder.h"
#include "parser/htmlparser.h"


namespace Css
{

ParseJob::ParseJob(const KDevelop::IndexedString& url, KDevelop::ILanguageSupport* languageSupport)
        : KDevelop::ParseJob(url, languageSupport)
{
    qCDebug(KDEV_CSS);
}

ParseJob::~ParseJob()
{
    qCDebug(KDEV_CSS);
}

void ParseJob::run(ThreadWeaver::JobPointer /*self*/, ThreadWeaver::Thread */*thread*/)
{
    KDevelop::UrlParseLock urlLock(document());

    if ( !(minimumFeatures() & KDevelop::TopDUContext::ForceUpdate) ) {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        bool needsUpdate = true;
        foreach(const KDevelop::ParsingEnvironmentFilePointer &file, KDevelop::DUChain::self()->allEnvironmentFiles(document())) {
            if (file->needsUpdate()) {
                needsUpdate = true;
                break;
            } else {
                needsUpdate = false;
            }
        }
        if (!needsUpdate) {
            qCDebug(KDEV_CSS) << "Already up to date" << document().str();
            return;
        }
    }

    qCDebug(KDEV_CSS) << "parsing" << document().str();

    KDevelop::ProblemPointer p = readContents();
    if (p) {
        //TODO: associate problem with topducontext
        return abortJob();;
    }

    QList<HtmlParser::Part> parts;

    QMimeDatabase db;
    if (db.mimeTypeForUrl(document().toUrl()).name() == "text/css") {
        HtmlParser::Part part;
        part.kind = HtmlParser::Part::Standalone;
        part.contents = contents().contents;
        part.range.start() = {0, 0};
        //part.range.end = TODO (needed?)
        parts << part;
    } else {
        HtmlParser p;
        p.setContents(contents().contents);
        parts = p.parse();
        if (parts.isEmpty()) {
            parts << HtmlParser::Part(); //empty part
        }
    }
    Q_ASSERT(!parts.isEmpty());

    HtmlAst *fileAst = new HtmlAst;
    fileAst->kind = HtmlAst::KIND;
    foreach (const HtmlParser::Part &part, parts) {
        ParseSession *session = new ParseSession;
        session->setCurrentDocument(document());
        session->setOffset(KDevelop::CursorInRevision::castFromSimpleCursor(part.range.start()));
        session->setContents(part.contents);
        if (part.kind != HtmlParser::Part::InlineStyle) {
            StyleElementAst *el = new StyleElementAst;
            el->kind = StyleElementAst::KIND;
            StartAst* ast = nullptr;
            session->parse(&ast);
            el->start = ast;
            el->session = session;
            fileAst->elements << el;
        } else {
            InlineStyleAst *el = new InlineStyleAst;
            el->kind = InlineStyleAst::KIND;
            DeclarationListAst* ast = nullptr;
            session->parse(&ast);
            el->declarationList = ast;
            el->session = session;
            fileAst->elements << el;
        }

        if (abortRequested())
        {
            return abortJob();
        }
    }

    KDevelop::ReferencedTopDUContext top;
    {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        top = KDevelop::DUChain::self()->chainForDocument(document());
    }
    if (top) {
        qCDebug(KDEV_CSS) << "re-compiling" << document().str();
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        top->clearImportedParentContexts();
        top->parsingEnvironmentFile()->clearModificationRevisions();
        top->clearProblems();
    } else {
        qCDebug(KDEV_CSS) << "compiling" << document().str();
    }

    QReadLocker parseLock(languageSupport()->parseLock());

    EditorIntegrator editor;
    DeclarationBuilder builder;
    builder.setEditor(&editor);
    top = builder.build(document(), fileAst, top);
    qCDebug(KDEV_CSS) << top;
    Q_ASSERT(top);

    foreach (AstNode *el, fileAst->elements) {

        ParseSession *session = nullptr;
        if (el->kind == StyleElementAst::KIND) {
            session = static_cast<StyleElementAst*>(el)->session;
            static_cast<StyleElementAst*>(el)->session = nullptr; //deleted below
        } else if (el->kind == InlineStyleAst::KIND) {
            session = static_cast<InlineStyleAst*>(el)->session;
            static_cast<InlineStyleAst*>(el)->session = nullptr; //deleted below
        }
        Q_ASSERT(session);
        foreach(const KDevelop::ProblemPointer &p, session->problems()) {
            KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
            top->addProblem(p);
        }
        delete session;

        if (abortRequested()) {
            return abortJob();
        }
        delete el;
    }
    delete fileAst;
    setDuChain(top);

    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

    top->setFeatures(minimumFeatures());
    KDevelop::ParsingEnvironmentFilePointer file = top->parsingEnvironmentFile();

    QFileInfo fileInfo(document().str());
    QDateTime lastModified = fileInfo.lastModified();
    file->setModificationRevision(contents().modification);
    KDevelop::DUChain::self()->updateContextEnvironment( top->topContext(), file.data() );
    KDevelop::DUChain::self()->emitUpdateReady(document(), top);
}

}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
