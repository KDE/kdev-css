// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_AST_H_INCLUDED
#define CSS_AST_H_INCLUDED

#include <QtCore/QList>
#include <kdev-pg-list.h>

#include <parser/parserexport.h>


#include <QtCore/QString>
#include <kdebug.h>
#include <language/interfaces/iproblem.h>
#include "parser/editorintegrator.h"


namespace Css
{

struct AttribAst;
struct CharsetAst;
struct DeclarationAst;
struct DeclarationListAst;
struct ElementNameAst;
struct ExprAst;
struct HexcolorAst;
struct HtmlAst;
struct IdentOrStringAst;
struct ImportAst;
struct ImportListAst;
struct MatchAst;
struct MaybeSgmlAst;
struct MaybeSpaceAst;
struct OperatorAst;
struct PrioAst;
struct PropertyAst;
struct PseudoAst;
struct RuleAst;
struct RuleListAst;
struct RulesetAst;
struct SelectorAst;
struct SelectorListAst;
struct SimpleSelectorAst;
struct SimpleSelectorWithWhitespaceAst;
struct SpecifierAst;
struct SpecifierListAst;
struct StartAst;
struct StringOrUriAst;
struct StyleElementAst;
struct TermAst;
struct UnaryOperatorAst;
struct UnaryTermAst;


class Tokenizer;

struct KDEVCSSPARSER_EXPORT AstNode
{
    enum AstNodeKind
    {
        AttribKind = 1000,
        CharsetKind = 1001,
        DeclarationKind = 1002,
        DeclarationListKind = 1003,
        ElementNameKind = 1004,
        ExprKind = 1005,
        HexcolorKind = 1006,
        HtmlKind = 1007,
        IdentOrStringKind = 1008,
        ImportKind = 1009,
        ImportListKind = 1010,
        MatchKind = 1011,
        MaybeSgmlKind = 1012,
        MaybeSpaceKind = 1013,
        OperatorKind = 1014,
        PrioKind = 1015,
        PropertyKind = 1016,
        PseudoKind = 1017,
        RuleKind = 1018,
        RuleListKind = 1019,
        RulesetKind = 1020,
        SelectorKind = 1021,
        SelectorListKind = 1022,
        SimpleSelectorKind = 1023,
        SimpleSelectorWithWhitespaceKind = 1024,
        SpecifierKind = 1025,
        SpecifierListKind = 1026,
        StartKind = 1027,
        StringOrUriKind = 1028,
        StyleElementKind = 1029,
        TermKind = 1030,
        UnaryOperatorKind = 1031,
        UnaryTermKind = 1032,
        AST_NODE_KIND_COUNT
    };

    int kind;
    qint64 startToken;
    qint64 endToken;

};

struct KDEVCSSPARSER_EXPORT AttribAst: public AstNode
{
    enum { KIND = AttribKind };

    qint64 attrName;
    MatchAst *match;
    IdentOrStringAst *ident;
};

struct KDEVCSSPARSER_EXPORT CharsetAst: public AstNode
{
    enum { KIND = CharsetKind };

};

struct KDEVCSSPARSER_EXPORT DeclarationAst: public AstNode
{
    enum { KIND = DeclarationKind };

    PropertyAst *property;
    qint64 colon;
    ExprAst *expr;
    qint64 semicolon;
};

struct KDEVCSSPARSER_EXPORT DeclarationListAst: public AstNode
{
    enum { KIND = DeclarationListKind };

    const KDevPG::ListNode<DeclarationAst *> *declarationsSequence;
};

struct KDEVCSSPARSER_EXPORT ElementNameAst: public AstNode
{
    enum { KIND = ElementNameKind };

    qint64 ident;
    qint64 star;
};

struct KDEVCSSPARSER_EXPORT ExprAst: public AstNode
{
    enum { KIND = ExprKind };

    const KDevPG::ListNode<TermAst *> *termsSequence;
};

struct KDEVCSSPARSER_EXPORT HexcolorAst: public AstNode
{
    enum { KIND = HexcolorKind };

};

struct KDEVCSSPARSER_EXPORT HtmlAst: public AstNode
{
    enum { KIND = HtmlKind };

    QList<StyleElementAst*> elements;
};

struct KDEVCSSPARSER_EXPORT IdentOrStringAst: public AstNode
{
    enum { KIND = IdentOrStringKind };

    qint64 ident;
    qint64 string;
};

struct KDEVCSSPARSER_EXPORT ImportAst: public AstNode
{
    enum { KIND = ImportKind };

};

struct KDEVCSSPARSER_EXPORT ImportListAst: public AstNode
{
    enum { KIND = ImportListKind };

    const KDevPG::ListNode<ImportAst *> *importsSequence;
};

struct KDEVCSSPARSER_EXPORT MatchAst: public AstNode
{
    enum { KIND = MatchKind };

};

struct KDEVCSSPARSER_EXPORT MaybeSgmlAst: public AstNode
{
    enum { KIND = MaybeSgmlKind };

};

struct KDEVCSSPARSER_EXPORT MaybeSpaceAst: public AstNode
{
    enum { KIND = MaybeSpaceKind };

};

struct KDEVCSSPARSER_EXPORT OperatorAst: public AstNode
{
    enum { KIND = OperatorKind };

};

struct KDEVCSSPARSER_EXPORT PrioAst: public AstNode
{
    enum { KIND = PrioKind };

};

struct KDEVCSSPARSER_EXPORT PropertyAst: public AstNode
{
    enum { KIND = PropertyKind };

    qint64 ident;
};

struct KDEVCSSPARSER_EXPORT PseudoAst: public AstNode
{
    enum { KIND = PseudoKind };

    qint64 ident;
};

struct KDEVCSSPARSER_EXPORT RuleAst: public AstNode
{
    enum { KIND = RuleKind };

    RulesetAst *ruleset;
};

struct KDEVCSSPARSER_EXPORT RuleListAst: public AstNode
{
    enum { KIND = RuleListKind };

    const KDevPG::ListNode<RuleAst *> *rulesSequence;
};

struct KDEVCSSPARSER_EXPORT RulesetAst: public AstNode
{
    enum { KIND = RulesetKind };

    SelectorListAst *selectors;
    qint64 lbrace;
    DeclarationListAst *declarations;
    qint64 rbrace;
};

struct KDEVCSSPARSER_EXPORT SelectorAst: public AstNode
{
    enum { KIND = SelectorKind };

    SimpleSelectorWithWhitespaceAst *simpleSelector;
};

struct KDEVCSSPARSER_EXPORT SelectorListAst: public AstNode
{
    enum { KIND = SelectorListKind };

    const KDevPG::ListNode<SelectorAst *> *selectorsSequence;
};

struct KDEVCSSPARSER_EXPORT SimpleSelectorAst: public AstNode
{
    enum { KIND = SimpleSelectorKind };

    ElementNameAst *element;
    SpecifierListAst *specifier;
};

struct KDEVCSSPARSER_EXPORT SimpleSelectorWithWhitespaceAst: public AstNode
{
    enum { KIND = SimpleSelectorWithWhitespaceKind };

    SimpleSelectorAst *simpleSelector;
};

struct KDEVCSSPARSER_EXPORT SpecifierAst: public AstNode
{
    enum { KIND = SpecifierKind };

    qint64 className;
    AttribAst *attrib;
    PseudoAst *pseudo;
    qint64 idSel;
};

struct KDEVCSSPARSER_EXPORT SpecifierListAst: public AstNode
{
    enum { KIND = SpecifierListKind };

    const KDevPG::ListNode<SpecifierAst *> *specifiersSequence;
};

struct KDEVCSSPARSER_EXPORT StartAst: public AstNode
{
    enum { KIND = StartKind };

    CharsetAst *charset;
    ImportListAst *imports;
    RuleListAst *rules;
};

struct KDEVCSSPARSER_EXPORT StringOrUriAst: public AstNode
{
    enum { KIND = StringOrUriKind };

    qint64 string;
    qint64 uri;
};

struct KDEVCSSPARSER_EXPORT StyleElementAst: public AstNode
{
    enum { KIND = StyleElementKind };

    ParseSession* session;
    StartAst *start;
};

struct KDEVCSSPARSER_EXPORT TermAst: public AstNode
{
    enum { KIND = TermKind };

    qint64 string;
    qint64 ident;
    UnaryOperatorAst *op;
    UnaryTermAst *term;
    qint64 uri;
    qint64 unicoderange;
    HexcolorAst *hexcolor;
};

struct KDEVCSSPARSER_EXPORT UnaryOperatorAst: public AstNode
{
    enum { KIND = UnaryOperatorKind };

};

struct KDEVCSSPARSER_EXPORT UnaryTermAst: public AstNode
{
    enum { KIND = UnaryTermKind };

    qint64 value;
};



} // end of namespace Css

#endif

