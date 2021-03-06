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
#include <KTextEditor/Document>

#include <language/codecompletion/codecompletion.h>
#include <interfaces/idocument.h>
#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <language/duchain/duchain.h>
#include <language/duchain/duchainlock.h>

#include "debug.h"
#include "completion/model.h"
#include "parsejob.h"
#include "navigation/navigationwidget.h"
#include "parser/parsesession.h"
#include "parser/editorintegrator.h"
#include "parser/cssdebugvisitor.h"
#include "version.h"

using namespace Css;

LanguageSupport* LanguageSupport::m_self = nullptr;

K_PLUGIN_FACTORY_WITH_JSON(KDevCssSupportFactory, "kdevcsssupport.json", registerPlugin<LanguageSupport>();)

LanguageSupport::LanguageSupport(QObject* parent, const QVariantList& /*args*/)
    : KDevelop::IPlugin(QStringLiteral("kdevcsssupport"), parent),
      KDevelop::ILanguageSupport()
{
    m_self = this;

    CodeCompletionModel* ccModel = new CodeCompletionModel(this);
    new KDevelop::CodeCompletion(this, ccModel, name());
}

QString LanguageSupport::name() const
{
    return QStringLiteral("Css");
}

KDevelop::ParseJob *LanguageSupport::createParseJob(const KDevelop::IndexedString& url)
{
    qCDebug(KDEV_CSS) << url;
    return new ParseJob(url, this);
}

LanguageSupport *LanguageSupport::self()
{
    return m_self;
}

class FindNodeVisitor : public DefaultVisitor
{
public:
    FindNodeVisitor(EditorIntegrator* editor, const KTextEditor::Cursor& position)
        : DefaultVisitor()
        , m_node(nullptr)
        , m_property(nullptr)
        , m_editor(editor)
        , m_position(position)
    {}

    void visitProperty(PropertyAst* node) override
    {
        if (!m_node && m_editor->findRange(node->ident).castToSimpleRange().contains(m_position)) {
            m_node = node;
        }
        if (!m_node) {
            m_property = node;
        }
        DefaultVisitor::visitProperty(node);
    }

    void visitTerm(TermAst* node) override
    {
        if (!m_node && m_editor->findRange(node).castToSimpleRange().contains(m_position)) {
            m_node = node;
        }
        DefaultVisitor::visitTerm(node);
    }

    AstNode *node()
    {
        return m_node;
    }

    PropertyAst *property()
    {
        return m_property;
    }
private:
    AstNode *m_node;
    PropertyAst *m_property;
    EditorIntegrator* m_editor;
    KTextEditor::Cursor m_position;
};

struct CursorIdentifier {
    CursorIdentifier(int kind_) : kind(kind_) {}
    int kind;
    KDevelop::RangeInRevision range;
    QString contents;
    QString property; //when ExprKind
};

CursorIdentifier cursorIdentifier(const QUrl& url, const KTextEditor::Cursor& position)
{
    KDevelop::IDocument* doc = KDevelop::ICore::self()->documentController()->documentForUrl(url);
    if(!doc || !doc->textDocument() || doc->textDocument()->views().isEmpty())
        return CursorIdentifier(0);

    KDevelop::RangeInRevision ctxRange;
    {
        KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
        KDevelop::TopDUContext* top = KDevelop::DUChain::self()->chainForDocument(url);
        if (!top) return CursorIdentifier(0);
        KDevelop::DUContext* ctx = top->findContextAt(KDevelop::CursorInRevision::castFromSimpleCursor(position));
        if (!ctx || ctx->type() != KDevelop::DUContext::Class) {
            return CursorIdentifier(0);
        }
        ctxRange = ctx->range();
    }


    ParseSession session;
    Css::DeclarationListAst* ast = nullptr;
    session.setOffset(ctxRange.start);
    session.setContents(doc->textDocument()->text(ctxRange.castToSimpleRange()));
    session.parse(&ast);
    {
        EditorIntegrator editor;
        editor.setParseSession(&session);
        FindNodeVisitor v(&editor, position);
        v.visitNode(ast);
        if (v.node() && v.node()->kind == PropertyAst::KIND) {
            PropertyAst* n = static_cast<PropertyAst*>(v.node());
            CursorIdentifier ret(v.node()->kind);
            ret.contents = editor.tokenToString(n->ident);
            ret.range = editor.findRange(n->ident);
            return ret;
        } else if (v.node() && v.node()->kind == TermAst::KIND && v.property()) {
            TermAst* n = static_cast<TermAst*>(v.node());
            if (n->ident != -1) {
                CursorIdentifier ret(v.node()->kind);
                ret.property = editor.tokenToString(v.property()->ident);
                ret.contents = editor.tokenToString(n->ident);
                ret.range = editor.findRange(n->ident);
                return ret;
            } else if (n->hexcolor) {
                CursorIdentifier ret(n->hexcolor->kind);
                ret.contents = editor.nodeToString(n->hexcolor);
                ret.range = editor.findRange(n->hexcolor, n->hexcolor);
                return ret;
            }
        }
    }
    return CursorIdentifier(0);
}

KTextEditor::Range LanguageSupport::specialLanguageObjectRange(const QUrl& url, const KTextEditor::Cursor& position)
{
    CursorIdentifier id = cursorIdentifier(url, position);
    qCDebug(KDEV_CSS) << id.kind << id.contents;
    if (id.kind) {
        return id.range.castToSimpleRange();
    }
    return KDevelop::ILanguageSupport::specialLanguageObjectRange(url, position);
}

QPair<QWidget*, KTextEditor::Range> LanguageSupport::specialLanguageObjectNavigationWidget(const QUrl& url, const KTextEditor::Cursor& position)
{
    CursorIdentifier id = cursorIdentifier(url, position);
    qCDebug(KDEV_CSS) << id.kind << id.contents;
    if (id.kind) {
        KDevelop::TopDUContextPointer top;
        {
            KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
            top = KDevelop::DUChain::self()->chainForDocument(url);
            if (!top) {
                return {nullptr, KTextEditor::Range::invalid()};
            }
        }
        QWidget* widget = nullptr;
        if (id.kind == PropertyAst::KIND) {
            ContentAssistData::Field field = ContentAssistData::self()->field(id.contents);
            if (!field.name.isEmpty()) {
                widget = new NavigationWidget(top, field);
            }
        } else if (id.kind == TermAst::KIND) {
            ContentAssistData::Field field = ContentAssistData::self()->field(id.property);
            if (field.values.contains(id.contents)) {
                widget = new NavigationWidget(top, field.values[id.contents]);
            } else if (QColor(id.contents.trimmed()).isValid()) {
                widget = new NavigationWidget(top, id.contents.trimmed());
            }
        } else if (id.kind == HexcolorAst::KIND) {
            widget = new NavigationWidget(top, id.contents.trimmed());
        } else {
            Q_ASSERT_X(false, "Css::LanguageSupport::specialLanguageObjectNavigationWidget",
                              qPrintable(QString("unhandled id kind: %1").arg(id.kind)));
        }
        if (widget) {
            return {widget, id.range.castToSimpleRange()};
        }
    }
    return KDevelop::ILanguageSupport::specialLanguageObjectNavigationWidget(url, position);
}

#include "csslanguagesupport.moc"
