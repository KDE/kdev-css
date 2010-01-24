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
#include <QReadWriteLock>
#include <QtCore/QReadLocker>
#include <QtCore/QThread>

#include <kdebug.h>

#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/topducontext.h>
#include <interfaces/ilanguage.h>
#include <language/duchain/parsingenvironment.h>

#include "parser/editorintegrator.h"
#include "parser/parsesession.h"
#include "csslanguagesupport.h"


namespace Css
{
extern int debugArea();
#define debug() kDebug(debugArea())

///TODO: push this into kdevplatform - copied from cpp for now

///Facilities to prevent multiple parse-jobs from processing the same url.
QMutex urlParseMutex;
QMap<KDevelop::IndexedString, QPair<Qt::HANDLE, uint> > parsingUrls;

struct UrlParseLock {
    UrlParseLock(KDevelop::IndexedString url) : m_url(url) {
        QMutexLocker lock(&urlParseMutex);
        while (parsingUrls.contains(m_url) && parsingUrls[m_url].first != QThread::currentThreadId()) {
            //Wait here until no other thread is updating parsing the url
            lock.unlock();
            sleep(1);
            lock.relock();
        }
        if (parsingUrls.contains(m_url)) {
            ++parsingUrls[m_url].second;
        } else {
            parsingUrls.insert(m_url, qMakePair(QThread::currentThreadId(), 1u));
        }
    }

    ~UrlParseLock() {
        QMutexLocker lock(&urlParseMutex);
        Q_ASSERT(parsingUrls.contains(m_url));
        Q_ASSERT(parsingUrls[m_url].first == QThread::currentThreadId());
        --parsingUrls[m_url].second;
        if (parsingUrls[m_url].second == 0) {
            parsingUrls.remove(m_url);
        }
    }

    KDevelop::IndexedString m_url;
};

ParseJob::ParseJob(const KUrl &url)
        : KDevelop::ParseJob(url)
{
}

ParseJob::~ParseJob()
{
}

LanguageSupport *ParseJob::css() const
{
    return LanguageSupport::self();
}

void ParseJob::run()
{
    UrlParseLock urlLock(document());

    {
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
        if (!(minimumFeatures() & KDevelop::TopDUContext::ForceUpdate) && !needsUpdate) {
            debug() << "Already up to date" << document().str();
            return;
        }
    }

    debug() << "parsing" << document().str();

    bool readFromDisk = !contentsAvailableFromEditor();

    ParseSession session;

    QString fileName = document().str();

    if (readFromDisk) {
        QFile file(fileName);
        //TODO: Read the first lines to determine encoding using Css encoding and use that for the text stream

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            /*
            KDevelop::ProblemPointer p(new KDevelop::Problem());
            p->setSource(KDevelop::ProblemData::Disk);
            p->setDescription(i18n("Could not open file '%1'", document().str()));
            switch (file.error()) {
            case QFile::ReadError:
                p->setExplanation(i18n("File could not be read from."));
                break;
            case QFile::OpenError:
                p->setExplanation(i18n("File could not be opened."));
                break;
            case QFile::PermissionsError:
                p->setExplanation(i18n("File permissions prevent opening for read."));
                break;
            default:
                break;
            }
            p->setFinalLocation(KDevelop::DocumentRange(document().str(), KTextEditor::Cursor(0, 0), KTextEditor::Cursor(0, 0)));
            // TODO addProblem(p);
            */
            kWarning() << "Could not open file" << document().str();
            return abortJob();
        }

        QTextStream s(&file);
//         if( codec )
//             s.setCodec( QTextCodec::codecForName(codec) );
        session.setContents(s.readAll());
        file.close();
    } else {
        session.setContents(contentsFromEditor());
        session.setCurrentDocument(document().str());
    }


    // 2) parse
    StartAst* ast = 0;
    session.parse(&ast);

    if (abortRequested()) {
        return abortJob();
    }

    EditorIntegrator editor(&session);

    QReadLocker parseLock(css()->language()->parseLock());


    KDevelop::ReferencedTopDUContext top;
    {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        top = KDevelop::DUChain::self()->chainForDocument(document());
    }

    if (top) {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        top->clearImportedParentContexts();
        top->parsingEnvironmentFile()->clearModificationRevisions();
        top->clearProblems();
    } else {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ParsingEnvironmentFile *file = new KDevelop::ParsingEnvironmentFile(document());
        file->setLanguage(KDevelop::IndexedString("Css"));
        top = new KDevelop::TopDUContext(document(), KDevelop::SimpleRange(KDevelop::SimpleCursor(0, 0), KDevelop::SimpleCursor(INT_MAX, INT_MAX)), file);
        KDevelop::DUChain::self()->addDocumentChain(top);
    }
    setDuChain(top);

    if (abortRequested()) {
        return abortJob();
    }

    foreach(const KDevelop::ProblemPointer &p, session.problems()) {
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        top->addProblem(p);
    }

    KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());

    top->setFeatures(minimumFeatures());
    KDevelop::ParsingEnvironmentFilePointer file = top->parsingEnvironmentFile();

    QFileInfo fileInfo(fileName);
    QDateTime lastModified = fileInfo.lastModified();
    if (readFromDisk) {
        file->setModificationRevision(KDevelop::ModificationRevision(lastModified));
    } else {
        file->setModificationRevision(KDevelop::ModificationRevision(lastModified, revisionToken()));
    }

    KDevelop::DUChain::self()->updateContextEnvironment( top->topContext(), file.data() );
}

}

#include "parsejob.moc"
// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on; auto-insert-doxygen on
