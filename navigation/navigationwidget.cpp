/*
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

#include "navigationwidget.h"

#include "fieldnavigationcontext.h"

namespace Css {

NavigationWidget::NavigationWidget(KDevelop::TopDUContextPointer topContext, const Css::ContentAssistData::Field& field)
    : KDevelop::AbstractNavigationWidget()
{
  m_topContext = topContext;
kDebug() << field.name;
  initBrowser(200);

  //The first context is registered so it is kept alive by the shared-pointer mechanism
  m_startContext = KDevelop::NavigationContextPointer(new FieldNavigationContext(topContext, field));
  setContext( m_startContext );

}


}
