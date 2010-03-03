// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_DEBUG_VISITOR_H_INCLUDED
#define CSS_DEBUG_VISITOR_H_INCLUDED

#include "cssdefaultvisitor.h"

#include <parser/parserexport.h>
#include <kdev-pg-token-stream.h>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

namespace Css
{

class KDEVCSSPARSER_EXPORT DebugVisitor: public DefaultVisitor
{
public:
    DebugVisitor(KDevPG::TokenStream *str, const QString& content = QString())
            : m_str(str), m_indent(0), m_content(content) {}
    virtual void visitAttrib(AttribAst *node)
    {
        if (!m_indent) printToken(node, "attrib");
        if (node->match) printToken(node->match, "match", "match");
        if (node->ident) printToken(node->ident, "identOrString", "ident");
        m_indent++;
        DefaultVisitor::visitAttrib(node);
        m_indent--;
    }

    virtual void visitCharset(CharsetAst *node)
    {
        if (!m_indent) printToken(node, "charset");
        m_indent++;
        DefaultVisitor::visitCharset(node);
        m_indent--;
    }

    virtual void visitDeclaration(DeclarationAst *node)
    {
        if (!m_indent) printToken(node, "declaration");
        if (node->property) printToken(node->property, "property", "property");
        if (node->expr) printToken(node->expr, "expr", "expr");
        m_indent++;
        DefaultVisitor::visitDeclaration(node);
        m_indent--;
    }

    virtual void visitDeclarationList(DeclarationListAst *node)
    {
        if (!m_indent) printToken(node, "declarationList");
        if (node->declarationsSequence)
        {
            const KDevPG::ListNode<DeclarationAst*> *__it = node->declarationsSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "declaration", "declarations[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitDeclarationList(node);
        m_indent--;
    }

    virtual void visitElementName(ElementNameAst *node)
    {
        if (!m_indent) printToken(node, "elementName");
        m_indent++;
        DefaultVisitor::visitElementName(node);
        m_indent--;
    }

    virtual void visitExpr(ExprAst *node)
    {
        if (!m_indent) printToken(node, "expr");
        if (node->termsSequence)
        {
            const KDevPG::ListNode<TermAst*> *__it = node->termsSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "term", "terms[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitExpr(node);
        m_indent--;
    }

    virtual void visitFunction(FunctionAst *node)
    {
        if (!m_indent) printToken(node, "function");
        m_indent++;
        DefaultVisitor::visitFunction(node);
        m_indent--;
    }

    virtual void visitHexcolor(HexcolorAst *node)
    {
        if (!m_indent) printToken(node, "hexcolor");
        m_indent++;
        DefaultVisitor::visitHexcolor(node);
        m_indent--;
    }

    virtual void visitHtml(HtmlAst *node)
    {
        if (!m_indent) printToken(node, "html");
        m_indent++;
        DefaultVisitor::visitHtml(node);
        m_indent--;
    }

    virtual void visitIdentOrString(IdentOrStringAst *node)
    {
        if (!m_indent) printToken(node, "identOrString");
        m_indent++;
        DefaultVisitor::visitIdentOrString(node);
        m_indent--;
    }

    virtual void visitImport(ImportAst *node)
    {
        if (!m_indent) printToken(node, "import");
        m_indent++;
        DefaultVisitor::visitImport(node);
        m_indent--;
    }

    virtual void visitImportList(ImportListAst *node)
    {
        if (!m_indent) printToken(node, "importList");
        if (node->importsSequence)
        {
            const KDevPG::ListNode<ImportAst*> *__it = node->importsSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "import", "imports[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitImportList(node);
        m_indent--;
    }

    virtual void visitInlineStyle(InlineStyleAst *node)
    {
        if (!m_indent) printToken(node, "inlineStyle");
        if (node->declarationList) printToken(node->declarationList, "declarationList", "declarationList");
        m_indent++;
        DefaultVisitor::visitInlineStyle(node);
        m_indent--;
    }

    virtual void visitMatch(MatchAst *node)
    {
        if (!m_indent) printToken(node, "match");
        m_indent++;
        DefaultVisitor::visitMatch(node);
        m_indent--;
    }

    virtual void visitMaybeSgml(MaybeSgmlAst *node)
    {
        if (!m_indent) printToken(node, "maybeSgml");
        m_indent++;
        DefaultVisitor::visitMaybeSgml(node);
        m_indent--;
    }

    virtual void visitMaybeSpace(MaybeSpaceAst *node)
    {
        if (!m_indent) printToken(node, "maybeSpace");
        m_indent++;
        DefaultVisitor::visitMaybeSpace(node);
        m_indent--;
    }

    virtual void visitOperator(OperatorAst *node)
    {
        if (!m_indent) printToken(node, "operator");
        m_indent++;
        DefaultVisitor::visitOperator(node);
        m_indent--;
    }

    virtual void visitPrio(PrioAst *node)
    {
        if (!m_indent) printToken(node, "prio");
        m_indent++;
        DefaultVisitor::visitPrio(node);
        m_indent--;
    }

    virtual void visitProperty(PropertyAst *node)
    {
        if (!m_indent) printToken(node, "property");
        m_indent++;
        DefaultVisitor::visitProperty(node);
        m_indent--;
    }

    virtual void visitPseudo(PseudoAst *node)
    {
        if (!m_indent) printToken(node, "pseudo");
        m_indent++;
        DefaultVisitor::visitPseudo(node);
        m_indent--;
    }

    virtual void visitRule(RuleAst *node)
    {
        if (!m_indent) printToken(node, "rule");
        if (node->ruleset) printToken(node->ruleset, "ruleset", "ruleset");
        m_indent++;
        DefaultVisitor::visitRule(node);
        m_indent--;
    }

    virtual void visitRuleList(RuleListAst *node)
    {
        if (!m_indent) printToken(node, "ruleList");
        if (node->rulesSequence)
        {
            const KDevPG::ListNode<RuleAst*> *__it = node->rulesSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "rule", "rules[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitRuleList(node);
        m_indent--;
    }

    virtual void visitRuleset(RulesetAst *node)
    {
        if (!m_indent) printToken(node, "ruleset");
        if (node->selectors) printToken(node->selectors, "selectorList", "selectors");
        if (node->declarations) printToken(node->declarations, "declarationList", "declarations");
        m_indent++;
        DefaultVisitor::visitRuleset(node);
        m_indent--;
    }

    virtual void visitSelector(SelectorAst *node)
    {
        if (!m_indent) printToken(node, "selector");
        if (node->simpleSelector) printToken(node->simpleSelector, "simpleSelectorWithWhitespace", "simpleSelector");
        m_indent++;
        DefaultVisitor::visitSelector(node);
        m_indent--;
    }

    virtual void visitSelectorList(SelectorListAst *node)
    {
        if (!m_indent) printToken(node, "selectorList");
        if (node->selectorsSequence)
        {
            const KDevPG::ListNode<SelectorAst*> *__it = node->selectorsSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "selector", "selectors[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitSelectorList(node);
        m_indent--;
    }

    virtual void visitSimpleSelector(SimpleSelectorAst *node)
    {
        if (!m_indent) printToken(node, "simpleSelector");
        if (node->element) printToken(node->element, "elementName", "element");
        if (node->specifier) printToken(node->specifier, "specifierList", "specifier");
        m_indent++;
        DefaultVisitor::visitSimpleSelector(node);
        m_indent--;
    }

    virtual void visitSimpleSelectorWithWhitespace(SimpleSelectorWithWhitespaceAst *node)
    {
        if (!m_indent) printToken(node, "simpleSelectorWithWhitespace");
        if (node->simpleSelector) printToken(node->simpleSelector, "simpleSelector", "simpleSelector");
        m_indent++;
        DefaultVisitor::visitSimpleSelectorWithWhitespace(node);
        m_indent--;
    }

    virtual void visitSpecifier(SpecifierAst *node)
    {
        if (!m_indent) printToken(node, "specifier");
        if (node->attrib) printToken(node->attrib, "attrib", "attrib");
        if (node->pseudo) printToken(node->pseudo, "pseudo", "pseudo");
        m_indent++;
        DefaultVisitor::visitSpecifier(node);
        m_indent--;
    }

    virtual void visitSpecifierList(SpecifierListAst *node)
    {
        if (!m_indent) printToken(node, "specifierList");
        if (node->specifiersSequence)
        {
            const KDevPG::ListNode<SpecifierAst*> *__it = node->specifiersSequence->front(), *__end = __it;
            do
            {
                printToken(__it->element, "specifier", "specifiers[]");
                __it = __it->next;
            }
            while (__it != __end);
        }
        m_indent++;
        DefaultVisitor::visitSpecifierList(node);
        m_indent--;
    }

    virtual void visitStart(StartAst *node)
    {
        if (!m_indent) printToken(node, "start");
        if (node->charset) printToken(node->charset, "charset", "charset");
        if (node->imports) printToken(node->imports, "importList", "imports");
        if (node->rules) printToken(node->rules, "ruleList", "rules");
        m_indent++;
        DefaultVisitor::visitStart(node);
        m_indent--;
    }

    virtual void visitStringOrUri(StringOrUriAst *node)
    {
        if (!m_indent) printToken(node, "stringOrUri");
        m_indent++;
        DefaultVisitor::visitStringOrUri(node);
        m_indent--;
    }

    virtual void visitStyleElement(StyleElementAst *node)
    {
        if (!m_indent) printToken(node, "styleElement");
        if (node->start) printToken(node->start, "start", "start");
        m_indent++;
        DefaultVisitor::visitStyleElement(node);
        m_indent--;
    }

    virtual void visitTerm(TermAst *node)
    {
        if (!m_indent) printToken(node, "term");
        if (node->op) printToken(node->op, "unaryOperator", "op");
        if (node->term) printToken(node->term, "unaryTerm", "term");
        if (node->hexcolor) printToken(node->hexcolor, "hexcolor", "hexcolor");
        if (node->function) printToken(node->function, "function", "function");
        m_indent++;
        DefaultVisitor::visitTerm(node);
        m_indent--;
    }

    virtual void visitUnaryOperator(UnaryOperatorAst *node)
    {
        if (!m_indent) printToken(node, "unaryOperator");
        m_indent++;
        DefaultVisitor::visitUnaryOperator(node);
        m_indent--;
    }

    virtual void visitUnaryTerm(UnaryTermAst *node)
    {
        if (!m_indent) printToken(node, "unaryTerm");
        m_indent++;
        DefaultVisitor::visitUnaryTerm(node);
        m_indent--;
    }

private:
    void printToken(AstNode *node, const QString &mType, const QString &mName = QString())
    {
        QString tokenString;
        if (!m_content.isEmpty())
        {
            KDevPG::TokenStream::Token startToken = m_str->token(node->startToken);
            KDevPG::TokenStream::Token endToken = m_str->token(node->endToken);
            int begin = startToken.begin;
            int end = endToken.end;
            if (end-begin > 30)
            {
                tokenString = m_content.mid(begin, 10);
                tokenString += " ...";
                tokenString += QString("%1 more").arg(end-begin-20);
                tokenString += "... ";
                tokenString += m_content.mid(end-10, 10);
            }
            else
            {
                tokenString = m_content.mid(begin, end-begin+1);
            }
            tokenString = tokenString.replace('\n', "\\n");
            tokenString = tokenString.replace('\r', "\\r");
        }
        qint64 beginLine,endLine,beginCol,endCol;
        m_str->startPosition(node->startToken, &beginLine, &beginCol);
        m_str->endPosition(node->endToken, &endLine, &endCol);
        qDebug() << QString().fill(' ', m_indent) + mName + (!mName.isEmpty() ? "->" : "") + mType + "[" << m_str->token( node->startToken ).begin << "," << beginLine << "," << beginCol << "] --- [" << m_str->token( node->endToken ).end << "," << endLine << "," << endCol << "] " << tokenString;
    }
    KDevPG::TokenStream *m_str;
    int m_indent;
    QString m_content;
};

} // end of namespace Css

#endif

