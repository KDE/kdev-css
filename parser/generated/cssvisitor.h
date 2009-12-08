// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_VISITOR_H_INCLUDED
#define CSS_VISITOR_H_INCLUDED

#include "cssast.h"

#include <parser/parserexport.h>
namespace Css
{

class KDEVCSSPARSER_EXPORT Visitor
{
    typedef void (Visitor::*ParserFuncType)(AstNode *);
    static ParserFuncType sParserTable[];

public:
    virtual ~Visitor() {}
    virtual void visitNode(AstNode *node)
    {
        if (node) (this->*sParserTable[node->kind - 1000])(node);
    }
    virtual void visitAttrib(AttribAst *) {}
    virtual void visitCharset(CharsetAst *) {}
    virtual void visitDeclaration(DeclarationAst *) {}
    virtual void visitDeclarationList(DeclarationListAst *) {}
    virtual void visitElementName(ElementNameAst *) {}
    virtual void visitExpr(ExprAst *) {}
    virtual void visitHexcolor(HexcolorAst *) {}
    virtual void visitIdentOrString(IdentOrStringAst *) {}
    virtual void visitImport(ImportAst *) {}
    virtual void visitImportList(ImportListAst *) {}
    virtual void visitMatch(MatchAst *) {}
    virtual void visitMaybeSgml(MaybeSgmlAst *) {}
    virtual void visitMaybeSpace(MaybeSpaceAst *) {}
    virtual void visitOperator(OperatorAst *) {}
    virtual void visitPrio(PrioAst *) {}
    virtual void visitProperty(PropertyAst *) {}
    virtual void visitPseudo(PseudoAst *) {}
    virtual void visitRule(RuleAst *) {}
    virtual void visitRuleList(RuleListAst *) {}
    virtual void visitRuleset(RulesetAst *) {}
    virtual void visitSelector(SelectorAst *) {}
    virtual void visitSelectorList(SelectorListAst *) {}
    virtual void visitSimpleSelector(SimpleSelectorAst *) {}
    virtual void visitSpecifier(SpecifierAst *) {}
    virtual void visitSpecifierList(SpecifierListAst *) {}
    virtual void visitStart(StartAst *) {}
    virtual void visitStringOrUri(StringOrUriAst *) {}
    virtual void visitTerm(TermAst *) {}
    virtual void visitUnaryOperator(UnaryOperatorAst *) {}
    virtual void visitUnaryTerm(UnaryTermAst *) {}
};

} // end of namespace Css

#endif

