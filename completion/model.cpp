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

#include "model.h"

#include <KTextEditor/View>
#include <KTextEditor/Document>

#include <cssdefaultvisitor.h>
#include "../parser/parsesession.h"
#include "../parser/editorintegrator.h"
#include "contentassistdata.h"

namespace Css {

extern int debugArea();

CodeCompletionModel::CodeCompletionModel(QObject *parent)
    : CodeCompletionModel2(parent), m_assistData(new ContentAssistData)
{
}

class FindCurrentNodeVisitor : public DefaultVisitor
{
public:
    FindCurrentNodeVisitor(EditorIntegrator* editor, const KTextEditor::Range& range)
        : m_node(0), m_editor(editor), m_range(range), m_lastSelectorElement(-1)
    {}
    virtual void visitNode(AstNode* node)
    {
        if (!node) return;
        KDevelop::SimpleCursor pos = m_editor->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        if (m_range.contains(pos.textCursor())) {
            m_node = node;
            //don't continue visiting
        } else {
            DefaultVisitor::visitNode(node);
        }
    }

    virtual void visitSimpleSelector(SimpleSelectorAst* node)
    {
        m_lastSelectorElement = node->element->ident;
        DefaultVisitor::visitSimpleSelector(node);
    }

    AstNode *currentNode() { return m_node; }
    QString lastSelectorElement() {
        if (m_lastSelectorElement == -1) return QString();
        return m_editor->tokenToString(m_lastSelectorElement);
    }
private:
    AstNode *m_node;
    EditorIntegrator *m_editor;
    KTextEditor::Range m_range;
    qint64 m_lastSelectorElement;
};

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
    Q_UNUSED(invocationType);

    kDebug(debugArea()) << range;
    ParseSession session;
    session.setContents(view->document()->text());
    Css::StartAst* ast = 0;
    session.parse(&ast);
    if (ast) {
        EditorIntegrator editor(&session);

        FindCurrentNodeVisitor visitor(&editor, range);
        visitor.visitNode(ast);
        AstNode* currentNode = visitor.currentNode();
        if (currentNode) {
            kDebug(debugArea()) << "currentNode kind" << currentNode->kind;
            if (currentNode->kind == AstNode::DeclarationListKind) {
                kDebug(debugArea()) << "lastSelectorElement" << visitor.lastSelectorElement();
                ContentAssistData::Element element = m_assistData->element(visitor.lastSelectorElement());
                m_currentFields = element.fields;
                kDebug(debugArea()) << m_currentFields;
                setRowCount(m_currentFields.count());
                reset();
            }
        }
    }
}

QVariant CodeCompletionModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole && index.column() == CodeCompletionModel::Name) {
        if (m_currentFields.count() < index.row()) return QVariant();
        return m_currentFields.at(index.row());
    }
    return QVariant();
}


}
