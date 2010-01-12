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


namespace Css
{

struct AttribAst;
struct CharsetAst;
struct DeclarationAst;
struct DeclarationListAst;
struct ElementNameAst;
struct ExprAst;
struct HexcolorAst;
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
struct SpecifierAst;
struct SpecifierListAst;
struct StartAst;
struct StringOrUriAst;
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
        IdentOrStringKind = 1007,
        ImportKind = 1008,
        ImportListKind = 1009,
        MatchKind = 1010,
        MaybeSgmlKind = 1011,
        MaybeSpaceKind = 1012,
        OperatorKind = 1013,
        PrioKind = 1014,
        PropertyKind = 1015,
        PseudoKind = 1016,
        RuleKind = 1017,
        RuleListKind = 1018,
        RulesetKind = 1019,
        SelectorKind = 1020,
        SelectorListKind = 1021,
        SimpleSelectorKind = 1022,
        SpecifierKind = 1023,
        SpecifierListKind = 1024,
        StartKind = 1025,
        StringOrUriKind = 1026,
        TermKind = 1027,
        UnaryOperatorKind = 1028,
        UnaryTermKind = 1029,
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

    SimpleSelectorAst *simpleSelector;
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

