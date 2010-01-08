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
#include <cssdebugvisitor.h>
#include "../parser/parsesession.h"
#include "../parser/editorintegrator.h"
#include "contentassistdata.h"

namespace Css {

extern int debugArea();
#define debug() kDebug(debugArea())

CodeCompletionModel::CodeCompletionModel(QObject *parent)
    : CodeCompletionModel2(parent), m_assistData(new ContentAssistData)
{
}

enum CompletionContext {
    NoContext,
    SelectorContext,
    PropertyContext,
    ValueContext
};

class FindCurrentNodeVisitor : public DefaultVisitor
{
public:
    FindCurrentNodeVisitor(EditorIntegrator* editor, const KTextEditor::Range& range)
        : m_editor(editor), m_range(range), m_lastSelectorElement(-1), m_context(SelectorContext)
    {}
    virtual void visitNode(AstNode* node)
    {
        if (!node) return;
        if (m_context != NoContext) {
            debug() << "context already set" << m_context;
            DefaultVisitor::visitNode(node);
            return;
        }
        debug();
        KDevelop::SimpleCursor pos = m_editor->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        debug() << m_editor->tokenToString(node->startToken) << m_range.start() << pos.textCursor();
        if (m_range.start() <=  pos.textCursor()) {
            //kDebug(debugArea()) << m_editor->tokenToString(node->startToken);
            debug() << "found range" << node << node->kind << m_range << pos.textCursor();
            debug() << "currentNode" << node << m_editor->tokenToString(node->startToken) << m_editor->tokenToString(node->endToken);
            debug() << "currentNode kind" << node->kind;
            if (node->kind == AstNode::DeclarationListKind || node->kind == AstNode::DeclarationKind) {
                debug() << "using PropertyContext";
                m_context = PropertyContext;
            } else if (node->kind == AstNode::ExprKind) {
                debug() << "using ValueContext";
                m_context = ValueContext;
            } else if (node->kind == AstNode::StartKind) {
                debug() << "using SelectorContext";
                m_context = SelectorContext;
            }
            //don't continue visiting
        } else {
            DefaultVisitor::visitNode(node);
        }
    }

    virtual void visitDeclarationList(DeclarationListAst* node)
    {
        KDevelop::SimpleCursor pos = m_editor->findPosition(node->startToken, EditorIntegrator::FrontEdge);
        debug() << pos.textCursor() << m_range;
        if (m_range.start() >= pos.textCursor()) {
            if (m_context == SelectorContext) {
                m_context = NoContext;
                debug() << "using NoContext";
            }
            DefaultVisitor::visitDeclarationList(node);
            if (m_context == NoContext) {
                m_context = SelectorContext;
                debug() << "using SelectorContext";
            }
        } else {
            debug() << "!(range > pos)";
        }
    }

    virtual void visitSimpleSelector(SimpleSelectorAst* node)
    {
        m_lastSelectorElement = node->element->ident;
        DefaultVisitor::visitSimpleSelector(node);
    }

    virtual void visitProperty(PropertyAst* node)
    {
        m_lastProperty = node->ident;
        DefaultVisitor::visitProperty(node);
    }

    CompletionContext currentContext() { return m_context; }
    QString lastSelectorElement() {
        if (m_lastSelectorElement == -1) return QString();
        return m_editor->tokenToString(m_lastSelectorElement);
    }
    QString lastProperty() {
        if (m_lastProperty == -1) return QString();
        return m_editor->tokenToString(m_lastProperty);
    }
private:
    EditorIntegrator *m_editor;
    KTextEditor::Range m_range;
    qint64 m_lastSelectorElement;
    qint64 m_lastProperty;
    CompletionContext m_context;
};

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
    Q_UNUSED(invocationType);

    debug() << range;
    ParseSession session;
    session.setContents(view->document()->text());
    Css::StartAst* ast = 0;
    session.parse(&ast);
    DebugVisitor vis(session.tokenStream(), session.contents());
    vis.visitNode(ast);

    if (ast) {
        EditorIntegrator editor(&session);

        FindCurrentNodeVisitor visitor(&editor, range);
        visitor.visitNode(ast);
        switch (visitor.currentContext()) {
            case PropertyContext:
            {
                debug() << "lastSelectorElement" << visitor.lastSelectorElement();
                ContentAssistData::Element element = m_assistData->element(visitor.lastSelectorElement());
                m_items = element.fields;
                setRowCount(m_items.count());
                reset();
                break;
            }
            case ValueContext:
            {
                debug() << "lastProperty" << visitor.lastProperty();
                ContentAssistData::Field field = m_assistData->field(visitor.lastProperty());
                m_items = field.values.keys();
                setRowCount(m_items.count());
                reset();
                break;
            }
            case SelectorContext:
                m_items = m_assistData->elements();
                setRowCount(m_items.count());
                reset();
                break;
            default:
                m_items.clear();
                setRowCount(0);
                reset();
                break;
        }
    }
}

QVariant CodeCompletionModel::data(const QModelIndex & index, int role) const
{
    if (index.parent().isValid()) return QVariant();
    if (role == Qt::DisplayRole && index.column() == CodeCompletionModel::Name) {
        if (m_items.count() < index.row()) return QVariant();
        return m_items.at(index.row());
    }
    return QVariant();
}

//default implementation with '-' added to reg exps
KTextEditor::Range CodeCompletionModel::completionRange(KTextEditor::View* view, const KTextEditor::Cursor &position)
{
    KTextEditor::Cursor end = position;

    QString text = " "+view->document()->line(end.line())+" ";

    static QRegExp findWordStart( "[^_\\w\\-]([_\\w\\-]+)$" );
    static QRegExp findWordEnd( "^([_\\w\\-]*)[^_\\w\\-]" );

    KTextEditor::Cursor start = end;

    //debug() << end << text.left(end.column()+1);
    if (findWordStart.lastIndexIn(text.left(end.column()+1)) >= 0)
        start.setColumn(findWordStart.pos(1)-1);
    //debug() << findWordStart.cap(0);

    if (findWordEnd.indexIn(text.mid(end.column()+1)) >= 0)
        end.setColumn(end.column()+1 + findWordEnd.cap(1).length()-1);
    //debug() << findWordEnd.cap(0);

    KTextEditor::Range ret = KTextEditor::Range(start, end);

    return ret;
}

//default implementation with '-' added to reg exp
bool CodeCompletionModel::shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::SmartRange& range, const QString& currentCompletion)
{
    Q_UNUSED(view);
    Q_UNUSED(range);
    static const QRegExp allowedText("^([\\w\\-]*)");
    bool ret = !allowedText.exactMatch(currentCompletion);
    debug() << currentCompletion << "shouldAbort:" << ret;
    return ret;
}

}

#include "model.moc"
