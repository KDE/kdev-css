/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2010 Niko Sams <niko.sams@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "declarationbuilder.h"

#include <interfaces/icore.h>
#include <language/duchain/classdeclaration.h>

#include "cssast.h"
#include "../../parser/parsesession.h"
#include "../../parser/editorintegrator.h"

namespace Css
{

void DeclarationBuilder::closeDeclaration()
{
    eventuallyAssignInternalContext();
    DeclarationBuilderBase::closeDeclaration();
}

void DeclarationBuilder::visitRuleset(RulesetAst *node)
{
    {
        KDevelop::SimpleRange range = editor()->findRange(node->selectors);
        KDevelop::QualifiedIdentifier id(KDevelop::Identifier(KDevelop::IndexedString(editor()->nodeToString(node->selectors))));
        KDevelop::DUChainWriteLocker lock(KDevelop::DUChain::lock());
        KDevelop::ClassDeclaration* dec = openDefinition<KDevelop::ClassDeclaration>(id, range);
        dec->setKind(KDevelop::Declaration::Type);
        dec->clearBaseClasses();
        dec->setClassType(KDevelop::ClassDeclarationData::Class);
    }
    DeclarationBuilderBase::visitRuleset(node);
    closeDeclaration();
}


}
