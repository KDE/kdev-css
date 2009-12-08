// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_DEFAULT_VISITOR_H_INCLUDED
#define CSS_DEFAULT_VISITOR_H_INCLUDED

#include "cssvisitor.h"

#include <parser/parserexport.h>
namespace Css
{

class KDEVCSSPARSER_EXPORT DefaultVisitor: public Visitor
{
public:
    virtual void visitAttrib(AttribAst *node);
    virtual void visitCharset(CharsetAst *node);
    virtual void visitDeclaration(DeclarationAst *node);
    virtual void visitDeclarationList(DeclarationListAst *node);
    virtual void visitElementName(ElementNameAst *node);
    virtual void visitExpr(ExprAst *node);
    virtual void visitHexcolor(HexcolorAst *node);
    virtual void visitIdentOrString(IdentOrStringAst *node);
    virtual void visitImport(ImportAst *node);
    virtual void visitImportList(ImportListAst *node);
    virtual void visitMatch(MatchAst *node);
    virtual void visitMaybeSgml(MaybeSgmlAst *node);
    virtual void visitMaybeSpace(MaybeSpaceAst *node);
    virtual void visitOperator(OperatorAst *node);
    virtual void visitPrio(PrioAst *node);
    virtual void visitProperty(PropertyAst *node);
    virtual void visitPseudo(PseudoAst *node);
    virtual void visitRule(RuleAst *node);
    virtual void visitRuleList(RuleListAst *node);
    virtual void visitRuleset(RulesetAst *node);
    virtual void visitSelector(SelectorAst *node);
    virtual void visitSelectorList(SelectorListAst *node);
    virtual void visitSimpleSelector(SimpleSelectorAst *node);
    virtual void visitSpecifier(SpecifierAst *node);
    virtual void visitSpecifierList(SpecifierListAst *node);
    virtual void visitStart(StartAst *node);
    virtual void visitStringOrUri(StringOrUriAst *node);
    virtual void visitTerm(TermAst *node);
    virtual void visitUnaryOperator(UnaryOperatorAst *node);
    virtual void visitUnaryTerm(UnaryTermAst *node);
};

} // end of namespace Css

#endif

