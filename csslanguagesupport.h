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

#ifndef KDEVCSSLANGUAGESUPPORT_H
#define KDEVCSSLANGUAGESUPPORT_H

#include <interfaces/iplugin.h>
#include <language/interfaces/ilanguagesupport.h>

namespace KDevelop
{
class ParseJob;
}

namespace Css
{
class LanguageSupport : public KDevelop::IPlugin, public KDevelop::ILanguageSupport
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::ILanguageSupport)

public:
    explicit LanguageSupport(QObject *parent, const QVariantList& args = QVariantList());

    /*Name Of the Language*/
    QString name() const override;

    KDevelop::ParseJob *createParseJob(const KDevelop::IndexedString& url) override;

    static LanguageSupport* self();

    KTextEditor::Range specialLanguageObjectRange(const QUrl& url, const KTextEditor::Cursor& position) override;
    QPair<QWidget*, KTextEditor::Range> specialLanguageObjectNavigationWidget(const QUrl& url, const KTextEditor::Cursor& position) override;

private:
    static LanguageSupport* m_self;
};

}

#endif

