/*
    Copyright (C) 2010 Milian Wolff <mail@milianw.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef CSS_COLORNAVIGATIONCONTEXT_H
#define CSS_COLORNAVIGATIONCONTEXT_H

#include <language/duchain/navigation/abstractnavigationcontext.h>

#include <QColor>

namespace Css {

class ColorNavigationContext : public KDevelop::AbstractNavigationContext
{
public:
    ColorNavigationContext(KDevelop::TopDUContextPointer topContext,
                           const QString& name);

    QString name() const override;
    QString html(bool shorten = false) override;

private:
    const QString m_name;
};

}

#endif // CSS_COLORNAVIGATIONCONTEXT_H
