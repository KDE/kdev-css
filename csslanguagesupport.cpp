/*
   Copyright (C) 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "csslanguagesupport.h"

#include <kpluginfactory.h>
#include <kpluginloader.h>
#include <kaboutdata.h>
#include <KTextEditor/Document>

#include <language/codecompletion/codecompletion.h>
#include <interfaces/idocument.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>

#include "completion/model.h"
#include "parsejob.h"
#include "navigation/fieldwidget.h"
#include "navigation/navigationwidget.h"

K_PLUGIN_FACTORY(KDevCssSupportFactory, registerPlugin<Css::LanguageSupport>();)
K_EXPORT_PLUGIN(KDevCssSupportFactory(KAboutData("kdevcsssupport","kdevcss", ki18n("Css Support"), "0.1", ki18n("Support for Css Language"), KAboutData::License_GPL)
    .addAuthor(ki18n("Niko Sams"), ki18n("Author"), "niko.sams@gmail.com", "http://nikosams.blogspot.com")
))

namespace Css
{
LanguageSupport* LanguageSupport::m_self = 0;

#if KDE_VERSION > KDE_MAKE_VERSION(4, 3, 80)
int debugArea() { static int s_area = KDebug::registerArea("kdevcsssupport"); return s_area; }
#else
int debugArea() { return 1; }
#endif

LanguageSupport::LanguageSupport(QObject* parent, const QVariantList& /*args*/)
    : KDevelop::IPlugin(KDevCssSupportFactory::componentData(), parent),
      KDevelop::ILanguageSupport()
{
    KDEV_USE_EXTENSION_INTERFACE(KDevelop::ILanguageSupport)

    m_self = this;

    CodeCompletionModel* ccModel = new CodeCompletionModel(this);
    new KDevelop::CodeCompletion(this, ccModel, name());
}

QString LanguageSupport::name() const
{
    return "Css";
}

KDevelop::ParseJob *LanguageSupport::createParseJob(const KUrl &url)
{
    kDebug(debugArea()) << url;
    return new ParseJob(url);
}

LanguageSupport *LanguageSupport::self()
{
    return m_self;
}

//TODO: drop QPair², maybe we don't need it at all or else use a struct
QPair<QPair<QString, KDevelop::SimpleRange>, QString> LanguageSupport::cursorIdentifier(const KUrl& url, const KDevelop::SimpleCursor& position) const
{
    {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        KDevelop::TopDUContext* top = KDevelop::DUChain::self()->chainForDocument(url);
        KDevelop::DUContext* ctx = top->findContextAt(position);
        if (!ctx || ctx->type() != KDevelop::DUContext::Class) {
            return qMakePair(qMakePair(QString(), KDevelop::SimpleRange::invalid()), QString());
        }
    }

    KDevelop::IDocument* doc = core()->documentController()->documentForUrl(url);
    if(!doc || !doc->textDocument() || !doc->textDocument()->activeView())
        return qMakePair(qMakePair(QString(), KDevelop::SimpleRange::invalid()), QString());

    int lineNumber = position.line;
    int lineLength = doc->textDocument()->lineLength(lineNumber);

    QString line = doc->textDocument()->text(KTextEditor::Range(lineNumber, 0, lineNumber, lineLength));

    int start = position.column;
    int end = position.column;

    while(start > 0 && (line[start].isLetterOrNumber() || line[start] == '-') && (line[start-1].isLetterOrNumber() || line[start-1] == '-'))
        --start;

    while(end <  lineLength && (line[end].isLetterOrNumber() || line[end] == '-'))
        ++end;

    KDevelop::SimpleRange wordRange = KDevelop::SimpleRange(lineNumber, start, lineNumber, end);

    return qMakePair( qMakePair(line.mid(start, end-start), wordRange), line.mid(end) );
}

KDevelop::SimpleRange LanguageSupport::specialLanguageObjectRange(const KUrl& url, const KDevelop::SimpleCursor& position)
{
    QPair<QPair<QString, KDevelop::SimpleRange>, QString> id = cursorIdentifier(url, position);
    kDebug() << id.first.first << id.second;
    if (id.first.second.isValid()) {
        return id.first.second;
    }
    //id.second;
    return KDevelop::ILanguageSupport::specialLanguageObjectRange(url, position);
}

QWidget* LanguageSupport::specialLanguageObjectNavigationWidget(const KUrl& url, const KDevelop::SimpleCursor& position)
{
    QPair<QPair<QString, KDevelop::SimpleRange>, QString> id = cursorIdentifier(url, position);
    kDebug() << id.first.first << id.second;
    if (id.first.second.isValid()) {
        ContentAssistData::Field field = ContentAssistData::self()->field(id.first.first);
        if (!field.name.isEmpty()) {
            KDevelop::TopDUContextPointer top;
            {
                KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
                top = KDevelop::DUChain::self()->chainForDocument(url);
            }
            return new NavigationWidget(top, field);
        }
    }
    return KDevelop::ILanguageSupport::specialLanguageObjectNavigationWidget(url, position);
}

}

#include "csslanguagesupport.moc"
