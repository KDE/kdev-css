// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cssvisitor.h"

namespace Css
{

Visitor::ParserFuncType Visitor::sParserTable[] =
{
    reinterpret_cast<ParserFuncType>(&Visitor::visitAttrib),
    reinterpret_cast<ParserFuncType>(&Visitor::visitCharset),
    reinterpret_cast<ParserFuncType>(&Visitor::visitDeclaration),
    reinterpret_cast<ParserFuncType>(&Visitor::visitDeclarationList),
    reinterpret_cast<ParserFuncType>(&Visitor::visitElementName),
    reinterpret_cast<ParserFuncType>(&Visitor::visitExpr),
    reinterpret_cast<ParserFuncType>(&Visitor::visitHexcolor),
    reinterpret_cast<ParserFuncType>(&Visitor::visitHtml),
    reinterpret_cast<ParserFuncType>(&Visitor::visitIdentOrString),
    reinterpret_cast<ParserFuncType>(&Visitor::visitImport),
    reinterpret_cast<ParserFuncType>(&Visitor::visitImportList),
    reinterpret_cast<ParserFuncType>(&Visitor::visitInlineStyle),
    reinterpret_cast<ParserFuncType>(&Visitor::visitMatch),
    reinterpret_cast<ParserFuncType>(&Visitor::visitMaybeSgml),
    reinterpret_cast<ParserFuncType>(&Visitor::visitMaybeSpace),
    reinterpret_cast<ParserFuncType>(&Visitor::visitOperator),
    reinterpret_cast<ParserFuncType>(&Visitor::visitPrio),
    reinterpret_cast<ParserFuncType>(&Visitor::visitProperty),
    reinterpret_cast<ParserFuncType>(&Visitor::visitPseudo),
    reinterpret_cast<ParserFuncType>(&Visitor::visitRule),
    reinterpret_cast<ParserFuncType>(&Visitor::visitRuleList),
    reinterpret_cast<ParserFuncType>(&Visitor::visitRuleset),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSelector),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSelectorList),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSimpleSelector),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSimpleSelectorWithWhitespace),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSpecifier),
    reinterpret_cast<ParserFuncType>(&Visitor::visitSpecifierList),
    reinterpret_cast<ParserFuncType>(&Visitor::visitStart),
    reinterpret_cast<ParserFuncType>(&Visitor::visitStringOrUri),
    reinterpret_cast<ParserFuncType>(&Visitor::visitStyleElement),
    reinterpret_cast<ParserFuncType>(&Visitor::visitTerm),
    reinterpret_cast<ParserFuncType>(&Visitor::visitUnaryOperator),
    reinterpret_cast<ParserFuncType>(&Visitor::visitUnaryTerm)
}; // sParserTable[]

} // end of namespace Css

