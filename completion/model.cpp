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

#include "debug.h"

#include <KTextEditor/View>
#include <KTextEditor/Document>

#include "cssdefaultvisitor.h"
#include "cssdebugvisitor.h"
#include "../parser/parsesession.h"
#include "../parser/editorintegrator.h"
#include "contentassistdata.h"
#include "../parser/htmlparser.h"

namespace Css {

#define ifDebug(x) x

CodeCompletionModel::CodeCompletionModel(QObject *parent)
    : KTextEditor::CodeCompletionModel(parent)
    , m_completionContext(NoContext)
{
}

class FindCurrentNodeVisitor : public DefaultVisitor
{
public:
    FindCurrentNodeVisitor(EditorIntegrator* editor, const KTextEditor::Range& range)
        : m_editor(editor), m_range(range), m_lastSelectorElement(-1), m_lastProperty(-1),
          m_context(CodeCompletionModel::SelectorContext)
    {}

    void visitDeclaration(DeclarationAst* node) override
    {
        {
            KTextEditor::Cursor pos = findPosition(node->startToken, EditorIntegrator::FrontEdge);
            ifDebug( qCDebug(KDEV_CSS) << m_editor->tokenToString(node->startToken) << m_range.start() << pos; )
            if (m_range.start() >= pos) {
                if (node->colon != -1 && m_range.start() >= findPosition(node->colon, EditorIntegrator::FrontEdge)) {
                    ifDebug( qCDebug(KDEV_CSS) << "using ValueContext"; )
                    m_context = CodeCompletionModel::ValueContext;
                } else {
                    ifDebug( qCDebug(KDEV_CSS) << "using PropertyContext 1"; )
                    m_context = CodeCompletionModel::PropertyContext;
                }
            }
        }
        if (node->semicolon != -1) {
            ifDebug( qCDebug(KDEV_CSS) << m_range.start() << findPosition(node->semicolon, EditorIntegrator::FrontEdge); )
            if (m_range.start() > findPosition(node->semicolon, EditorIntegrator::FrontEdge)) {
                ifDebug( qCDebug(KDEV_CSS) << "using PropertyContext 2"; )
                m_context = CodeCompletionModel::PropertyContext;
            }
        }
        DefaultVisitor::visitDeclaration(node);
    }

    void visitRuleset(RulesetAst* node) override
    {
        if (node->lbrace != -1
              && m_range.start() >= findPosition(node->lbrace))
        {
            ifDebug( qCDebug(KDEV_CSS) << "using PropertyContext"; )
            m_context = CodeCompletionModel::PropertyContext;
        } else if (m_range.start() >= findPosition(node->startToken, EditorIntegrator::FrontEdge)) {
            ifDebug( qCDebug(KDEV_CSS) << "using SelectorContext 1"; )
            m_context = CodeCompletionModel::SelectorContext;
        }
        DefaultVisitor::visitRuleset(node);
        if (node->rbrace != -1 && m_range.start() >= findPosition(node->rbrace)) {
            ifDebug( qCDebug(KDEV_CSS) << "using SelectorContext 2"; )
            m_context = CodeCompletionModel::SelectorContext;
        }
    }

    void visitSimpleSelector(SimpleSelectorAst* node) override
    {
        if (node->element) {
            ifDebug( qCDebug(KDEV_CSS) << m_lastProperty << m_range.start() << findPosition(node->endToken); )
            if (m_range.start() > findPosition(node->endToken)) {
                m_lastSelectorElement = node->element->ident;
                ifDebug( qCDebug(KDEV_CSS) << "set lastSelectorElement" << m_editor->tokenToString(m_lastSelectorElement); )
            }
        }
        DefaultVisitor::visitSimpleSelector(node);
    }

    void visitProperty(PropertyAst* node) override
    {
        ifDebug( qCDebug(KDEV_CSS) << m_lastProperty << m_range.start() << findPosition(node->endToken); )
        if (m_range.start() > findPosition(node->endToken)) {
            m_lastProperty = node->ident;
            ifDebug( qCDebug(KDEV_CSS) << "set lastProperty" << m_editor->tokenToString(m_lastProperty); )
        }
        DefaultVisitor::visitProperty(node);
    }

    CodeCompletionModel::CompletionContext currentContext() { return m_context; }
    QString lastSelectorElement() {
        if (m_lastSelectorElement == -1) return QString();
        return m_editor->tokenToString(m_lastSelectorElement);
    }
    QString lastProperty() {
        if (m_lastProperty == -1) return QString();
        return m_editor->tokenToString(m_lastProperty);
    }
    KTextEditor::Cursor findPosition(qint64 token,
                                        EditorIntegrator::Edge edge = EditorIntegrator::BackEdge)
    {
      return m_editor->findPosition(token, edge).castToSimpleCursor();
    }
private:
    EditorIntegrator *m_editor;
    KTextEditor::Range m_range;
    qint64 m_lastSelectorElement;
    qint64 m_lastProperty;
    CodeCompletionModel::CompletionContext m_context;
};

void CodeCompletionModel::completionInvoked(KTextEditor::View* view, const KTextEditor::Range& range, InvocationType invocationType)
{
    Q_UNUSED(invocationType);

    ifDebug( qCDebug(KDEV_CSS) << range; )

    QList<HtmlParser::Part> parts;
    if (view->document()->mimeType() == "text/css") {
        ifDebug( qCDebug(KDEV_CSS) << "css"; )
        HtmlParser::Part part;
        part.kind = HtmlParser::Part::Standalone;
        part.contents = view->document()->text();
        part.range = view->document()->documentRange();
        parts << part;
    } else {
        ifDebug( qCDebug(KDEV_CSS) << "html"; )
        HtmlParser p;
        p.setContents(view->document()->text());
        parts = p.parse();
        if (parts.isEmpty()) {
            parts << HtmlParser::Part(); //empty part
        }
    }
    Q_ASSERT(!parts.isEmpty());

    ParseSession session;
    Css::AstNode* ast = nullptr;
    HtmlParser::Part part;
    foreach (const HtmlParser::Part &p, parts) {
        if (p.range.contains(range.start())
                || p.range.start() == range.start()
                || p.range.end() == range.end()
                ) {
            session.setContents(p.contents);
            session.setOffset(KDevelop::CursorInRevision::castFromSimpleCursor(p.range.start()));
            if (p.kind != HtmlParser::Part::InlineStyle) {
                StartAst* a = static_cast<StartAst*>(ast);
                session.parse(&a);
                ast = a;
            } else {
                DeclarationListAst* a = static_cast<DeclarationListAst*>(ast);
                session.parse(&a);
                ast = a;
            }
            part = p;
            break;
        }
    }

    ifDebug(
//         DebugVisitor vis(session.tokenStream(), session.contents());
//         vis.visitNode(ast);
    )

    beginResetModel();
    if (ast) {
        EditorIntegrator editor;
        editor.setParseSession(&session);

        FindCurrentNodeVisitor visitor(&editor, range);
        visitor.visitNode(ast);
        m_completionContext = visitor.currentContext();
        qCDebug(KDEV_CSS) << "context" << m_completionContext;
        switch (m_completionContext) {
            case PropertyContext:
            {
                QString el;
                if (part.kind != HtmlParser::Part::InlineStyle) {
                    qCDebug(KDEV_CSS) << "lastSelectorElement" << visitor.lastSelectorElement();
                    el = visitor.lastSelectorElement();
                } else {
                    el = part.tag;
                }
                ContentAssistData::Element element = ContentAssistData::self()->element(el);
                m_items = element.fields;
                setRowCount(m_items.count());
                endResetModel();
                return;
            }
            case ValueContext:
            {
                qCDebug(KDEV_CSS) << "lastProperty" << visitor.lastProperty();
                ContentAssistData::Field field = ContentAssistData::self()->field(visitor.lastProperty());
                m_items = field.values.keys();
                setRowCount(m_items.count());
                endResetModel();
                return;
            }
            case SelectorContext:
                m_items = ContentAssistData::self()->elements();
                setRowCount(m_items.count());
                endResetModel();
                return;
            default:
                break;
        }
    }
    m_items.clear();
    setRowCount(0);
    endResetModel();
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

    //qCDebug(KDEV_CSS) << end << text.left(end.column()+1);
    if (findWordStart.lastIndexIn(text.left(end.column()+1)) >= 0)
        start.setColumn(findWordStart.pos(1)-1);
    //qCDebug(KDEV_CSS) << findWordStart.cap(0);

    if (findWordEnd.indexIn(text.mid(end.column()+1)) >= 0)
        end.setColumn(end.column()+1 + findWordEnd.cap(1).length()-1);
    //qCDebug(KDEV_CSS) << findWordEnd.cap(0);

    KTextEditor::Range ret = KTextEditor::Range(start, end);

    return ret;
}

//default implementation with '-' added to reg exp
bool CodeCompletionModel::shouldAbortCompletion(KTextEditor::View* view, const KTextEditor::Range& range, const QString& currentCompletion)
{
    Q_UNUSED(view);
    Q_UNUSED(range);
    static const QRegExp allowedText("^([\\w\\-]*)");
    bool ret = !allowedText.exactMatch(currentCompletion);
    qCDebug(KDEV_CSS) << currentCompletion << "shouldAbort:" << ret;
    return ret;
}

bool CodeCompletionModel::shouldStartCompletion(KTextEditor::View* view, const QString& insertedText, bool userInsertion, const KTextEditor::Cursor& position)
{
        Q_UNUSED(view);
    Q_UNUSED(position);
    if(insertedText.isEmpty())
        return false;

    QChar lastChar = insertedText.at(insertedText.count() - 1);
    if (userInsertion && (lastChar.isLetter() || lastChar.isNumber() || lastChar == ':')) {
        return true;
    }
    return false;
}

void CodeCompletionModel::executeCompletionItem2(KTextEditor::Document* document, const KTextEditor::Range& word, const QModelIndex& index) const
{
    QString text = data(index.sibling(index.row(), Name), Qt::DisplayRole).toString();
    if (m_completionContext == PropertyContext) {
        text += ':';
    } else if (m_completionContext == ValueContext) {
        text += ';';
    }
    document->replaceText(word, text);
}


}

