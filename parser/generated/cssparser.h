// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_H_INCLUDED
#define CSS_H_INCLUDED

#include "cssast.h"
#include <kdev-pg-memory-pool.h>
#include <kdev-pg-allocator.h>
#include <kdev-pg-token-stream.h>

#include <parser/parserexport.h>
namespace Css
{

class KDEVCSSPARSER_EXPORT Parser
{
public:
    typedef KDevPG::TokenStream::Token Token;
    KDevPG::TokenStream *tokenStream;
    int yytoken;

    inline Token LA(qint64 k = 1) const
    {
        return tokenStream->token(tokenStream->index() - 1 + k - 1);
    }
    inline int yylex()
    {
        return (yytoken = tokenStream->nextToken());
    }
    inline void rewind(qint64 index)
    {
        tokenStream->rewind(index);
        yylex();
    }

// token stream
    void setTokenStream(KDevPG::TokenStream *s)
    {
        tokenStream = s;
    }

// error handling
    void expectedSymbol(int kind, const QString& name);
    void expectedToken(int kind, qint64 token, const QString& name);

    bool mBlockErrors;
    inline bool blockErrors(bool block)
    {
        bool previous = mBlockErrors;
        mBlockErrors = block;
        return previous;
    }

// memory pool
    typedef KDevPG::MemoryPool memoryPoolType;

    KDevPG::MemoryPool *memoryPool;
    void setMemoryPool(KDevPG::MemoryPool *p)
    {
        memoryPool = p;
    }
    template <class T>
    inline T *create()
    {
        T *node = new (memoryPool->allocate(sizeof(T))) T();
        node->kind = T::KIND;
        return node;
    }

    enum TokenType
    {
        Token_BEGINSWITH = 1000,
        Token_CHARSET_SYM = 1001,
        Token_CMS = 1002,
        Token_COLON = 1003,
        Token_COMMA = 1004,
        Token_CONTAINS = 1005,
        Token_DASHMATCH = 1006,
        Token_DEGS = 1007,
        Token_DIMEN = 1008,
        Token_DIVIDE = 1009,
        Token_DOT = 1010,
        Token_DPCM = 1011,
        Token_DPI = 1012,
        Token_EMS = 1013,
        Token_ENDSWITH = 1014,
        Token_EOF = 1015,
        Token_EQUALS = 1016,
        Token_EXS = 1017,
        Token_FLOAT = 1018,
        Token_FONT_FACE_SYM = 1019,
        Token_FUNCTION = 1020,
        Token_GRADS = 1021,
        Token_HERZ = 1022,
        Token_HEXCOLOR = 1023,
        Token_IDENT = 1024,
        Token_IDSEL = 1025,
        Token_IMPORTANT_SYM = 1026,
        Token_IMPORT_SYM = 1027,
        Token_INCLUDES = 1028,
        Token_INS = 1029,
        Token_INTEGER = 1030,
        Token_INVALID = 1031,
        Token_KHERZ = 1032,
        Token_LBRACE = 1033,
        Token_LBRACKET = 1034,
        Token_LPAREN = 1035,
        Token_MEDIA_AND = 1036,
        Token_MEDIA_NOT = 1037,
        Token_MEDIA_ONLY = 1038,
        Token_MEDIA_SYM = 1039,
        Token_MINUS = 1040,
        Token_MMS = 1041,
        Token_MSECS = 1042,
        Token_NAMESPACE_SYM = 1043,
        Token_NOTFUNCTION = 1044,
        Token_NTH = 1045,
        Token_PAGE_SYM = 1046,
        Token_PCS = 1047,
        Token_PERCENTAGE = 1048,
        Token_PLUS = 1049,
        Token_PTS = 1050,
        Token_PXS = 1051,
        Token_QEMS = 1052,
        Token_RADS = 1053,
        Token_RBRACE = 1054,
        Token_RBRACKET = 1055,
        Token_RPAREN = 1056,
        Token_SECS = 1057,
        Token_SEMICOLON = 1058,
        Token_SGML_CD = 1059,
        Token_STAR = 1060,
        Token_STRING = 1061,
        Token_UNICODERANGE = 1062,
        Token_URI = 1063,
        Token_WHITESPACE = 1064,
        TokenTypeSize
    }; // TokenType

// user defined declarations:
public:

    /**
     * Transform the raw input into tokens.
     * When this method returns, the parser's token stream has been filled
     * and any parse*() method can be called.
     */
    void tokenize(const QString& contents);

    enum ProblemType
    {
        Error,
        Warning,
        Info
    };
    void reportProblem( Parser::ProblemType type, const QString& message );
    QList<KDevelop::ProblemPointer> problems()
    {
        return m_problems;
    }
    QString tokenText(qint64 begin, qint64 end);
    void setDebug(bool debug);
    void setCurrentDocument(QString url);


private:

    QString m_contents;
    bool m_debug;
    QString m_currentDocument;
    QList<KDevelop::ProblemPointer> m_problems;


public:
    Parser()
    {
        memoryPool = 0;
        tokenStream = 0;
        yytoken = Token_EOF;
        mBlockErrors = false;
    }

    virtual ~Parser() {}

    bool parseAttrib(AttribAst **yynode);
    bool parseCharset(CharsetAst **yynode);
    bool parseDeclaration(DeclarationAst **yynode);
    bool parseDeclarationList(DeclarationListAst **yynode);
    bool parseElementName(ElementNameAst **yynode);
    bool parseExpr(ExprAst **yynode);
    bool parseHexcolor(HexcolorAst **yynode);
    bool parseIdentOrString(IdentOrStringAst **yynode);
    bool parseImport(ImportAst **yynode);
    bool parseImportList(ImportListAst **yynode);
    bool parseMatch(MatchAst **yynode);
    bool parseMaybeSgml(MaybeSgmlAst **yynode);
    bool parseMaybeSpace(MaybeSpaceAst **yynode);
    bool parseOperator(OperatorAst **yynode);
    bool parsePrio(PrioAst **yynode);
    bool parseProperty(PropertyAst **yynode);
    bool parsePseudo(PseudoAst **yynode);
    bool parseRule(RuleAst **yynode);
    bool parseRuleList(RuleListAst **yynode);
    bool parseRuleset(RulesetAst **yynode);
    bool parseSelector(SelectorAst **yynode);
    bool parseSelectorList(SelectorListAst **yynode);
    bool parseSimpleSelector(SimpleSelectorAst **yynode);
    bool parseSimpleSelectorWithWhitespace(SimpleSelectorWithWhitespaceAst **yynode);
    bool parseSpecifier(SpecifierAst **yynode);
    bool parseSpecifierList(SpecifierListAst **yynode);
    bool parseStart(StartAst **yynode);
    bool parseStringOrUri(StringOrUriAst **yynode);
    bool parseTerm(TermAst **yynode);
    bool parseUnaryOperator(UnaryOperatorAst **yynode);
    bool parseUnaryTerm(UnaryTermAst **yynode);
};

} // end of namespace Css

#endif

