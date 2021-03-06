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

#ifndef CONTEXTBUILDER_H
#define CONTEXTBUILDER_H

#include "cssdefaultvisitor.h"
#include <language/duchain/builders/abstractcontextbuilder.h>

// #include "cssduchainexport.h"
// #include "helper.h"


namespace Css
{
class EditorIntegrator;
class ParseSession;

typedef KDevelop::AbstractContextBuilder<AstNode, SpecifierAst> ContextBuilderBase;

class ContextBuilder: public ContextBuilderBase, public DefaultVisitor
{

public:
    ContextBuilder();
    ~ContextBuilder() override;

    KDevelop::ReferencedTopDUContext build(const KDevelop::IndexedString& url, AstNode* node,
            const KDevelop::ReferencedTopDUContext& updateContext = KDevelop::ReferencedTopDUContext()) override;

    void setEditor(EditorIntegrator* editor);
protected:
    EditorIntegrator* editor() const;

    KDevelop::TopDUContext* newTopContext(const KDevelop::RangeInRevision& range,
                                          KDevelop::ParsingEnvironmentFile* file = nullptr) override;

    void startVisiting(AstNode* node) override;
    void setContextOnNode(AstNode* node, KDevelop::DUContext* ctx) override;
    KDevelop::DUContext* contextFromNode(AstNode* node) override;
    KDevelop::RangeInRevision editorFindRange(AstNode* fromRange, AstNode* toRange) override;
    /// Find Cursor for start of a node, useful to limit findLocalDeclarations() searches.
    KDevelop::CursorInRevision startPos(AstNode* node);

    KDevelop::QualifiedIdentifier identifierForNode(SpecifierAst* id) override;

    void visitRuleset(RulesetAst* node) override;

    /// Whether semantic problems should get reported
    bool m_reportErrors;

private:
    EditorIntegrator* m_editor;
};

}

#endif
