// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cssdefaultvisitor.h"

namespace Css
{

void DefaultVisitor::visitAttrib(AttribAst *node)
{
    visitNode(node->match);
    visitNode(node->ident);
}

void DefaultVisitor::visitCharset(CharsetAst *)
{
}

void DefaultVisitor::visitDeclaration(DeclarationAst *node)
{
    visitNode(node->property);
    visitNode(node->expr);
}

void DefaultVisitor::visitDeclarationList(DeclarationListAst *node)
{
    if (node->declarationsSequence)
    {
        const KDevPG::ListNode<DeclarationAst*> *__it = node->declarationsSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitElementName(ElementNameAst *)
{
}

void DefaultVisitor::visitExpr(ExprAst *node)
{
    if (node->termsSequence)
    {
        const KDevPG::ListNode<TermAst*> *__it = node->termsSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitHexcolor(HexcolorAst *)
{
}

void DefaultVisitor::visitIdentOrString(IdentOrStringAst *)
{
}

void DefaultVisitor::visitImport(ImportAst *)
{
}

void DefaultVisitor::visitImportList(ImportListAst *node)
{
    if (node->importsSequence)
    {
        const KDevPG::ListNode<ImportAst*> *__it = node->importsSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitMatch(MatchAst *)
{
}

void DefaultVisitor::visitMaybeSgml(MaybeSgmlAst *)
{
}

void DefaultVisitor::visitMaybeSpace(MaybeSpaceAst *)
{
}

void DefaultVisitor::visitOperator(OperatorAst *)
{
}

void DefaultVisitor::visitPrio(PrioAst *)
{
}

void DefaultVisitor::visitProperty(PropertyAst *)
{
}

void DefaultVisitor::visitPseudo(PseudoAst *)
{
}

void DefaultVisitor::visitRule(RuleAst *node)
{
    visitNode(node->ruleset);
}

void DefaultVisitor::visitRuleList(RuleListAst *node)
{
    if (node->rulesSequence)
    {
        const KDevPG::ListNode<RuleAst*> *__it = node->rulesSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitRuleset(RulesetAst *node)
{
    visitNode(node->selectors);
    visitNode(node->declarations);
}

void DefaultVisitor::visitSelector(SelectorAst *node)
{
    visitNode(node->simpleSelector);
}

void DefaultVisitor::visitSelectorList(SelectorListAst *node)
{
    if (node->selectorsSequence)
    {
        const KDevPG::ListNode<SelectorAst*> *__it = node->selectorsSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitSimpleSelector(SimpleSelectorAst *node)
{
    visitNode(node->element);
    visitNode(node->specifier);
}

void DefaultVisitor::visitSpecifier(SpecifierAst *node)
{
    visitNode(node->attrib);
    visitNode(node->pseudo);
}

void DefaultVisitor::visitSpecifierList(SpecifierListAst *node)
{
    if (node->specifiersSequence)
    {
        const KDevPG::ListNode<SpecifierAst*> *__it = node->specifiersSequence->front(), *__end = __it;
        do
        {
            visitNode(__it->element);
            __it = __it->next;
        }
        while (__it != __end);
    }
}

void DefaultVisitor::visitStart(StartAst *node)
{
    visitNode(node->charset);
    visitNode(node->imports);
    visitNode(node->rules);
}

void DefaultVisitor::visitStringOrUri(StringOrUriAst *)
{
}

void DefaultVisitor::visitTerm(TermAst *node)
{
    visitNode(node->op);
    visitNode(node->term);
    visitNode(node->hexcolor);
}

void DefaultVisitor::visitUnaryOperator(UnaryOperatorAst *)
{
}

void DefaultVisitor::visitUnaryTerm(UnaryTermAst *)
{
}


} // end of namespace Css

