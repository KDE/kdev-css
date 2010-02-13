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

#include <language/codecompletion/codecompletion.h>

#include "completion/model.h"
#include "parsejob.h"

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



}

#include "csslanguagesupport.moc"
