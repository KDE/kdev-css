// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#include "cssparser.h"



#include <QtCore/QDebug>
#include <KTextEditor/Range>
#include "parser/tokenizer.h"

namespace Css
{

void Parser::tokenize(const QString& contents)
{
    m_contents = contents;

    Tokenizer tokenizer(tokenStream, contents.toLatin1().data());

    int kind = Parser::Token_EOF;
    do
    {
        kind = tokenizer.yylex();

        if ( !kind ) // when the lexer returns 0, the end of file is reached
        {
            kind = Parser::Token_EOF;
        }
        Parser::Token &t = tokenStream->next();
        t.begin = tokenizer.tokenBegin();
        t.end = tokenizer.tokenEnd();
        t.kind = kind;
        //if ( m_debug ) qDebug() << kind << tokenText(t.begin,t.end) << t.begin << t.end;
    }
    while ( kind != Parser::Token_EOF );

    yylex(); // produce the look ahead token
}


QString Parser::tokenText(qint64 begin, qint64 end)
{
    return m_contents.mid(begin,end-begin+1);
}


void Parser::reportProblem( Parser::ProblemType type, const QString& message )
{
    if (type == Error)
        qDebug() << "** ERROR:" << message;
    else if (type == Warning)
        qDebug() << "** WARNING:" << message;
    else if (type == Info)
        qDebug() << "** Info:" << message;

    qint64 sLine;
    qint64 sCol;
    qint64 index = tokenStream->index()-1;
    tokenStream->startPosition(index, &sLine, &sCol);
    qint64 eLine;
    qint64 eCol;
    tokenStream->endPosition(index, &eLine, &eCol);
    KDevelop::Problem *p = new KDevelop::Problem();
    p->setSource(KDevelop::ProblemData::Parser);
    switch ( type )
    {
    case Error:
        p->setSeverity(KDevelop::ProblemData::Error);
        break;
    case Warning:
        p->setSeverity(KDevelop::ProblemData::Warning);
        break;
    case Info:
        p->setSeverity(KDevelop::ProblemData::Hint);
        break;
    }
    p->setDescription(message);
    p->setFinalLocation(KDevelop::DocumentRange(m_currentDocument, KTextEditor::Range(sLine, sCol, eLine, eCol+1)));
    m_problems << KDevelop::ProblemPointer(p);
}


// custom error recovery
void Parser::expectedToken(int /*expected*/, qint64 /*where*/, const QString& name)
{
    reportProblem( Parser::Error, QString("Expected token \"%1\"").arg(name));
}

void Parser::expectedSymbol(int /*expectedSymbol*/, const QString& name)
{
    qint64 line;
    qint64 col;
    qint64 index = tokenStream->index()-1;
    Token &token = tokenStream->token(index);
    kDebug() << "token starts at:" << token.begin;
    kDebug() << "index is:" << index;
    tokenStream->startPosition(index, &line, &col);
    QString tokenValue = tokenText(token.begin, token.end);
    qint64 eLine;
    qint64 eCol;
    tokenStream->endPosition(index, &eLine, &eCol);
    reportProblem( Parser::Error,
                   QString("Expected symbol \"%1\" (current token: \"%2\" [%3] at %4:%5 - %6:%7)")
                   .arg(name)
                   .arg(token.kind != 0 ? tokenValue : "EOF")
                   .arg(token.kind)
                   .arg(line)
                   .arg(col)
                   .arg(eLine)
                   .arg(eCol));
}

void Parser::setDebug( bool debug )
{
    m_debug = debug;
}

void Parser::setCurrentDocument(QString url)
{
    m_currentDocument = url;
}

} // end of namespace Css


namespace Css
{

bool Parser::parseAttrib(AttribAst **yynode)
{
    *yynode = create<AttribAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->attrName = -1;

    if (yytoken == Token_LBRACKET)
    {
        if (yytoken != Token_LBRACKET)
        {
            expectedToken(yytoken, Token_LBRACKET, "[");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_0 = 0;
        if (!parseMaybeSpace(&__node_0))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken != Token_IDENT)
        {
            expectedToken(yytoken, Token_IDENT, "identifier");
            return false;
        }
        (*yynode)->attrName = tokenStream->index() - 1;
        yylex();

        MaybeSpaceAst *__node_1 = 0;
        if (!parseMaybeSpace(&__node_1))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken == Token_BEGINSWITH
            || yytoken == Token_CONTAINS
            || yytoken == Token_DASHMATCH
            || yytoken == Token_ENDSWITH
            || yytoken == Token_EQUALS
            || yytoken == Token_INCLUDES)
        {
            MatchAst *__node_2 = 0;
            if (!parseMatch(&__node_2))
            {
                expectedSymbol(AstNode::MatchKind, "match");
                return false;
            }
            (*yynode)->match = __node_2;

            MaybeSpaceAst *__node_3 = 0;
            if (!parseMaybeSpace(&__node_3))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
            IdentOrStringAst *__node_4 = 0;
            if (!parseIdentOrString(&__node_4))
            {
                expectedSymbol(AstNode::IdentOrStringKind, "identOrString");
                return false;
            }
            (*yynode)->ident = __node_4;

            MaybeSpaceAst *__node_5 = 0;
            if (!parseMaybeSpace(&__node_5))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
        if (yytoken != Token_RBRACKET)
        {
            expectedToken(yytoken, Token_RBRACKET, "]");
            return false;
        }
        yylex();

    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseCharset(CharsetAst **yynode)
{
    *yynode = create<CharsetAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_CHARSET_SYM)
    {
        if (yytoken != Token_CHARSET_SYM)
        {
            expectedToken(yytoken, Token_CHARSET_SYM, "@charset");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_6 = 0;
        if (!parseMaybeSpace(&__node_6))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken != Token_STRING)
        {
            expectedToken(yytoken, Token_STRING, "string");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_7 = 0;
        if (!parseMaybeSpace(&__node_7))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken != Token_SEMICOLON)
        {
            expectedToken(yytoken, Token_SEMICOLON, ";");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_8 = 0;
        if (!parseMaybeSpace(&__node_8))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseDeclaration(DeclarationAst **yynode)
{
    *yynode = create<DeclarationAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->colon = -1;
    (*yynode)->semicolon = -1;

    if (yytoken == Token_IDENT)
    {
        PropertyAst *__node_9 = 0;
        if (!parseProperty(&__node_9))
        {
            expectedSymbol(AstNode::PropertyKind, "property");
            return false;
        }
        (*yynode)->property = __node_9;

        if (yytoken == Token_COLON)
        {
            if (yytoken != Token_COLON)
            {
                expectedToken(yytoken, Token_COLON, ":");
                return false;
            }
            (*yynode)->colon = tokenStream->index() - 1;
            yylex();

        }
        else if (true /*epsilon*/)
        {
            reportProblem( Error, "Expected Colon" );
        }
        else
        {
            return false;
        }
        MaybeSpaceAst *__node_10 = 0;
        if (!parseMaybeSpace(&__node_10))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        ExprAst *__node_11 = 0;
        if (!parseExpr(&__node_11))
        {
            expectedSymbol(AstNode::ExprKind, "expr");
            return false;
        }
        (*yynode)->expr = __node_11;

        PrioAst *__node_12 = 0;
        if (!parsePrio(&__node_12))
        {
            expectedSymbol(AstNode::PrioKind, "prio");
            return false;
        }
        if (yytoken == Token_SEMICOLON)
        {
            if (yytoken != Token_SEMICOLON)
            {
                expectedToken(yytoken, Token_SEMICOLON, ";");
                return false;
            }
            (*yynode)->semicolon = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_13 = 0;
            if (!parseMaybeSpace(&__node_13))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseDeclarationList(DeclarationListAst **yynode)
{
    *yynode = create<DeclarationListAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_IDENT || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_EOF
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_RBRACE
        || yytoken == Token_SGML_CD
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        if (yytoken == Token_IDENT)
        {
            do
            {
                DeclarationAst *__node_14 = 0;
                if (!parseDeclaration(&__node_14))
                {
                    expectedSymbol(AstNode::DeclarationKind, "declaration");
                    return false;
                }
                (*yynode)->declarationsSequence = snoc((*yynode)->declarationsSequence, __node_14, memoryPool);

            }
            while (yytoken == Token_IDENT);
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseElementName(ElementNameAst **yynode)
{
    *yynode = create<ElementNameAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->ident = -1;
    (*yynode)->star = -1;

    if (yytoken == Token_IDENT
        || yytoken == Token_STAR)
    {
        if (yytoken == Token_IDENT)
        {
            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->ident = tokenStream->index() - 1;
            yylex();

        }
        else if (yytoken == Token_STAR)
        {
            if (yytoken != Token_STAR)
            {
                expectedToken(yytoken, Token_STAR, "*");
                return false;
            }
            (*yynode)->star = tokenStream->index() - 1;
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseExpr(ExprAst **yynode)
{
    *yynode = create<ExprAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_CMS
        || yytoken == Token_DEGS
        || yytoken == Token_DIMEN
        || yytoken == Token_EMS
        || yytoken == Token_EXS
        || yytoken == Token_FLOAT
        || yytoken == Token_GRADS
        || yytoken == Token_HERZ
        || yytoken == Token_HEXCOLOR
        || yytoken == Token_IDENT
        || yytoken == Token_INS
        || yytoken == Token_INTEGER
        || yytoken == Token_KHERZ
        || yytoken == Token_MINUS
        || yytoken == Token_MMS
        || yytoken == Token_MSECS
        || yytoken == Token_PCS
        || yytoken == Token_PERCENTAGE
        || yytoken == Token_PLUS
        || yytoken == Token_PTS
        || yytoken == Token_PXS
        || yytoken == Token_QEMS
        || yytoken == Token_RADS
        || yytoken == Token_SECS
        || yytoken == Token_STRING
        || yytoken == Token_UNICODERANGE
        || yytoken == Token_URI || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_EOF
        || yytoken == Token_IDENT
        || yytoken == Token_IMPORTANT_SYM
        || yytoken == Token_LBRACKET
        || yytoken == Token_RBRACE
        || yytoken == Token_SEMICOLON
        || yytoken == Token_SGML_CD
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        if (yytoken == Token_CMS
            || yytoken == Token_DEGS
            || yytoken == Token_DIMEN
            || yytoken == Token_EMS
            || yytoken == Token_EXS
            || yytoken == Token_FLOAT
            || yytoken == Token_GRADS
            || yytoken == Token_HERZ
            || yytoken == Token_HEXCOLOR
            || yytoken == Token_IDENT
            || yytoken == Token_INS
            || yytoken == Token_INTEGER
            || yytoken == Token_KHERZ
            || yytoken == Token_MINUS
            || yytoken == Token_MMS
            || yytoken == Token_MSECS
            || yytoken == Token_PCS
            || yytoken == Token_PERCENTAGE
            || yytoken == Token_PLUS
            || yytoken == Token_PTS
            || yytoken == Token_PXS
            || yytoken == Token_QEMS
            || yytoken == Token_RADS
            || yytoken == Token_SECS
            || yytoken == Token_STRING
            || yytoken == Token_UNICODERANGE
            || yytoken == Token_URI)
        {
            do
            {
                TermAst *__node_15 = 0;
                if (!parseTerm(&__node_15))
                {
                    expectedSymbol(AstNode::TermKind, "term");
                    return false;
                }
                (*yynode)->termsSequence = snoc((*yynode)->termsSequence, __node_15, memoryPool);

            }
            while (yytoken == Token_CMS
                   || yytoken == Token_DEGS
                   || yytoken == Token_DIMEN
                   || yytoken == Token_EMS
                   || yytoken == Token_EXS
                   || yytoken == Token_FLOAT
                   || yytoken == Token_GRADS
                   || yytoken == Token_HERZ
                   || yytoken == Token_HEXCOLOR
                   || yytoken == Token_IDENT
                   || yytoken == Token_INS
                   || yytoken == Token_INTEGER
                   || yytoken == Token_KHERZ
                   || yytoken == Token_MINUS
                   || yytoken == Token_MMS
                   || yytoken == Token_MSECS
                   || yytoken == Token_PCS
                   || yytoken == Token_PERCENTAGE
                   || yytoken == Token_PLUS
                   || yytoken == Token_PTS
                   || yytoken == Token_PXS
                   || yytoken == Token_QEMS
                   || yytoken == Token_RADS
                   || yytoken == Token_SECS
                   || yytoken == Token_STRING
                   || yytoken == Token_UNICODERANGE
                   || yytoken == Token_URI);
            while (yytoken == Token_COMMA
                   || yytoken == Token_DIVIDE)
            {
                OperatorAst *__node_16 = 0;
                if (!parseOperator(&__node_16))
                {
                    expectedSymbol(AstNode::OperatorKind, "operator");
                    return false;
                }
                do
                {
                    TermAst *__node_17 = 0;
                    if (!parseTerm(&__node_17))
                    {
                        expectedSymbol(AstNode::TermKind, "term");
                        return false;
                    }
                    (*yynode)->termsSequence = snoc((*yynode)->termsSequence, __node_17, memoryPool);

                }
                while (yytoken == Token_CMS
                       || yytoken == Token_DEGS
                       || yytoken == Token_DIMEN
                       || yytoken == Token_EMS
                       || yytoken == Token_EXS
                       || yytoken == Token_FLOAT
                       || yytoken == Token_GRADS
                       || yytoken == Token_HERZ
                       || yytoken == Token_HEXCOLOR
                       || yytoken == Token_IDENT
                       || yytoken == Token_INS
                       || yytoken == Token_INTEGER
                       || yytoken == Token_KHERZ
                       || yytoken == Token_MINUS
                       || yytoken == Token_MMS
                       || yytoken == Token_MSECS
                       || yytoken == Token_PCS
                       || yytoken == Token_PERCENTAGE
                       || yytoken == Token_PLUS
                       || yytoken == Token_PTS
                       || yytoken == Token_PXS
                       || yytoken == Token_QEMS
                       || yytoken == Token_RADS
                       || yytoken == Token_SECS
                       || yytoken == Token_STRING
                       || yytoken == Token_UNICODERANGE
                       || yytoken == Token_URI);
            }
        }
        else if (true /*epsilon*/)
        {
            reportProblem( Error, "Expected exprssion" );
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseHexcolor(HexcolorAst **yynode)
{
    *yynode = create<HexcolorAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_HEXCOLOR)
    {
        if (yytoken != Token_HEXCOLOR)
        {
            expectedToken(yytoken, Token_HEXCOLOR, "hex color");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_18 = 0;
        if (!parseMaybeSpace(&__node_18))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseIdentOrString(IdentOrStringAst **yynode)
{
    *yynode = create<IdentOrStringAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->ident = -1;
    (*yynode)->string = -1;

    if (yytoken == Token_IDENT
        || yytoken == Token_STRING)
    {
        if (yytoken == Token_IDENT)
        {
            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->ident = tokenStream->index() - 1;
            yylex();

        }
        else if (yytoken == Token_STRING)
        {
            if (yytoken != Token_STRING)
            {
                expectedToken(yytoken, Token_STRING, "string");
                return false;
            }
            (*yynode)->string = tokenStream->index() - 1;
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseImport(ImportAst **yynode)
{
    *yynode = create<ImportAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_IMPORT_SYM)
    {
        if (yytoken != Token_IMPORT_SYM)
        {
            expectedToken(yytoken, Token_IMPORT_SYM, "@import");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_19 = 0;
        if (!parseMaybeSpace(&__node_19))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        StringOrUriAst *__node_20 = 0;
        if (!parseStringOrUri(&__node_20))
        {
            expectedSymbol(AstNode::StringOrUriKind, "stringOrUri");
            return false;
        }
        MaybeSpaceAst *__node_21 = 0;
        if (!parseMaybeSpace(&__node_21))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken != Token_SEMICOLON)
        {
            expectedToken(yytoken, Token_SEMICOLON, ";");
            return false;
        }
        yylex();

        MaybeSpaceAst *__node_22 = 0;
        if (!parseMaybeSpace(&__node_22))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseImportList(ImportListAst **yynode)
{
    *yynode = create<ImportListAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_IMPORT_SYM || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_EOF
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        while (yytoken == Token_IMPORT_SYM)
        {
            ImportAst *__node_23 = 0;
            if (!parseImport(&__node_23))
            {
                expectedSymbol(AstNode::ImportKind, "import");
                return false;
            }
            (*yynode)->importsSequence = snoc((*yynode)->importsSequence, __node_23, memoryPool);

            MaybeSgmlAst *__node_24 = 0;
            if (!parseMaybeSgml(&__node_24))
            {
                expectedSymbol(AstNode::MaybeSgmlKind, "maybeSgml");
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseMatch(MatchAst **yynode)
{
    *yynode = create<MatchAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_BEGINSWITH
        || yytoken == Token_CONTAINS
        || yytoken == Token_DASHMATCH
        || yytoken == Token_ENDSWITH
        || yytoken == Token_EQUALS
        || yytoken == Token_INCLUDES)
    {
        if (yytoken == Token_EQUALS)
        {
            if (yytoken != Token_EQUALS)
            {
                expectedToken(yytoken, Token_EQUALS, "=");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_INCLUDES)
        {
            if (yytoken != Token_INCLUDES)
            {
                expectedToken(yytoken, Token_INCLUDES, "~=");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_DASHMATCH)
        {
            if (yytoken != Token_DASHMATCH)
            {
                expectedToken(yytoken, Token_DASHMATCH, "|=");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_BEGINSWITH)
        {
            if (yytoken != Token_BEGINSWITH)
            {
                expectedToken(yytoken, Token_BEGINSWITH, "^=");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_ENDSWITH)
        {
            if (yytoken != Token_ENDSWITH)
            {
                expectedToken(yytoken, Token_ENDSWITH, "$=");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_CONTAINS)
        {
            if (yytoken != Token_CONTAINS)
            {
                expectedToken(yytoken, Token_CONTAINS, "*=");
                return false;
            }
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseMaybeSgml(MaybeSgmlAst **yynode)
{
    *yynode = create<MaybeSgmlAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_SGML_CD
        || yytoken == Token_WHITESPACE || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_EOF
        || yytoken == Token_IDENT
        || yytoken == Token_IMPORT_SYM
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        MaybeSpaceAst *__node_25 = 0;
        if (!parseMaybeSpace(&__node_25))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken == Token_SGML_CD)
        {
            if (yytoken != Token_SGML_CD)
            {
                expectedToken(yytoken, Token_SGML_CD, "<!--/-->");
                return false;
            }
            yylex();

            while (yytoken == Token_SGML_CD
                   || yytoken == Token_WHITESPACE)
            {
                MaybeSpaceAst *__node_26 = 0;
                if (!parseMaybeSpace(&__node_26))
                {
                    expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                    return false;
                }
                if (yytoken != Token_SGML_CD)
                {
                    expectedToken(yytoken, Token_SGML_CD, "<!--/-->");
                    return false;
                }
                yylex();

            }
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseMaybeSpace(MaybeSpaceAst **yynode)
{
    *yynode = create<MaybeSpaceAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_WHITESPACE || yytoken == Token_BEGINSWITH
        || yytoken == Token_CMS
        || yytoken == Token_COLON
        || yytoken == Token_COMMA
        || yytoken == Token_CONTAINS
        || yytoken == Token_DASHMATCH
        || yytoken == Token_DEGS
        || yytoken == Token_DIMEN
        || yytoken == Token_DIVIDE
        || yytoken == Token_DOT
        || yytoken == Token_EMS
        || yytoken == Token_ENDSWITH
        || yytoken == Token_EOF
        || yytoken == Token_EQUALS
        || yytoken == Token_EXS
        || yytoken == Token_FLOAT
        || yytoken == Token_GRADS
        || yytoken == Token_HERZ
        || yytoken == Token_HEXCOLOR
        || yytoken == Token_IDENT
        || yytoken == Token_IMPORTANT_SYM
        || yytoken == Token_IMPORT_SYM
        || yytoken == Token_INCLUDES
        || yytoken == Token_INS
        || yytoken == Token_INTEGER
        || yytoken == Token_KHERZ
        || yytoken == Token_LBRACE
        || yytoken == Token_LBRACKET
        || yytoken == Token_MINUS
        || yytoken == Token_MMS
        || yytoken == Token_MSECS
        || yytoken == Token_PCS
        || yytoken == Token_PERCENTAGE
        || yytoken == Token_PLUS
        || yytoken == Token_PTS
        || yytoken == Token_PXS
        || yytoken == Token_QEMS
        || yytoken == Token_RADS
        || yytoken == Token_RBRACE
        || yytoken == Token_RBRACKET
        || yytoken == Token_RPAREN
        || yytoken == Token_SECS
        || yytoken == Token_SEMICOLON
        || yytoken == Token_SGML_CD
        || yytoken == Token_STAR
        || yytoken == Token_STRING
        || yytoken == Token_UNICODERANGE
        || yytoken == Token_URI
        || yytoken == Token_WHITESPACE)
    {
        if (yytoken == Token_WHITESPACE)
        {
            do
            {
                if (yytoken != Token_WHITESPACE)
                {
                    expectedToken(yytoken, Token_WHITESPACE, "whitespace");
                    return false;
                }
                yylex();

            }
            while (yytoken == Token_WHITESPACE);
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseOperator(OperatorAst **yynode)
{
    *yynode = create<OperatorAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COMMA
        || yytoken == Token_DIVIDE)
    {
        if (yytoken == Token_DIVIDE)
        {
            if (yytoken != Token_DIVIDE)
            {
                expectedToken(yytoken, Token_DIVIDE, "/");
                return false;
            }
            yylex();

            MaybeSpaceAst *__node_27 = 0;
            if (!parseMaybeSpace(&__node_27))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_COMMA)
        {
            if (yytoken != Token_COMMA)
            {
                expectedToken(yytoken, Token_COMMA, ",");
                return false;
            }
            yylex();

            MaybeSpaceAst *__node_28 = 0;
            if (!parseMaybeSpace(&__node_28))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parsePrio(PrioAst **yynode)
{
    *yynode = create<PrioAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_IMPORTANT_SYM || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_EOF
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_RBRACE
        || yytoken == Token_SEMICOLON
        || yytoken == Token_SGML_CD
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        if (yytoken == Token_IMPORTANT_SYM)
        {
            if (yytoken != Token_IMPORTANT_SYM)
            {
                expectedToken(yytoken, Token_IMPORTANT_SYM, "!important");
                return false;
            }
            yylex();

            MaybeSpaceAst *__node_29 = 0;
            if (!parseMaybeSpace(&__node_29))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseProperty(PropertyAst **yynode)
{
    *yynode = create<PropertyAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->ident = -1;

    if (yytoken == Token_IDENT)
    {
        if (yytoken != Token_IDENT)
        {
            expectedToken(yytoken, Token_IDENT, "identifier");
            return false;
        }
        (*yynode)->ident = tokenStream->index() - 1;
        yylex();

        MaybeSpaceAst *__node_30 = 0;
        if (!parseMaybeSpace(&__node_30))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parsePseudo(PseudoAst **yynode)
{
    *yynode = create<PseudoAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->ident = -1;
    (*yynode)->ident = -1;

    if (yytoken == Token_COLON)
    {
        if (yytoken != Token_COLON)
        {
            expectedToken(yytoken, Token_COLON, ":");
            return false;
        }
        yylex();

        if (yytoken == Token_IDENT)
        {
            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->ident = tokenStream->index() - 1;
            yylex();

        }
        else if (yytoken == Token_FUNCTION)
        {
            if (yytoken != Token_FUNCTION)
            {
                expectedToken(yytoken, Token_FUNCTION, "function");
                return false;
            }
            yylex();

            if (yytoken == Token_NTH)
            {
                if (yytoken != Token_NTH)
                {
                    expectedToken(yytoken, Token_NTH, "nth");
                    return false;
                }
                yylex();

            }
            else if (yytoken == Token_INTEGER)
            {
                if (yytoken != Token_INTEGER)
                {
                    expectedToken(yytoken, Token_INTEGER, "integer");
                    return false;
                }
                yylex();

            }
            else if (yytoken == Token_IDENT)
            {
                if (yytoken != Token_IDENT)
                {
                    expectedToken(yytoken, Token_IDENT, "identifier");
                    return false;
                }
                yylex();

            }
            else
            {
                return false;
            }
            if (yytoken != Token_RPAREN)
            {
                expectedToken(yytoken, Token_RPAREN, ")");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_NOTFUNCTION)
        {
            if (yytoken != Token_NOTFUNCTION)
            {
                expectedToken(yytoken, Token_NOTFUNCTION, "not function");
                return false;
            }
            yylex();

            MaybeSpaceAst *__node_31 = 0;
            if (!parseMaybeSpace(&__node_31))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
            SimpleSelectorAst *__node_32 = 0;
            if (!parseSimpleSelector(&__node_32))
            {
                expectedSymbol(AstNode::SimpleSelectorKind, "simpleSelector");
                return false;
            }
            MaybeSpaceAst *__node_33 = 0;
            if (!parseMaybeSpace(&__node_33))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
            if (yytoken != Token_RPAREN)
            {
                expectedToken(yytoken, Token_RPAREN, ")");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_COLON)
        {
            if (yytoken != Token_COLON)
            {
                expectedToken(yytoken, Token_COLON, ":");
                return false;
            }
            yylex();

            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->ident = tokenStream->index() - 1;
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseRule(RuleAst **yynode)
{
    *yynode = create<RuleAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        RulesetAst *__node_34 = 0;
        if (!parseRuleset(&__node_34))
        {
            expectedSymbol(AstNode::RulesetKind, "ruleset");
            return false;
        }
        (*yynode)->ruleset = __node_34;

    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseRuleList(RuleListAst **yynode)
{
    *yynode = create<RuleListAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE || yytoken == Token_EOF)
    {
        while (yytoken == Token_COLON
               || yytoken == Token_DOT
               || yytoken == Token_IDENT
               || yytoken == Token_LBRACKET
               || yytoken == Token_STAR
               || yytoken == Token_WHITESPACE)
        {
            RuleAst *__node_35 = 0;
            if (!parseRule(&__node_35))
            {
                expectedSymbol(AstNode::RuleKind, "rule");
                return false;
            }
            (*yynode)->rulesSequence = snoc((*yynode)->rulesSequence, __node_35, memoryPool);

            MaybeSgmlAst *__node_36 = 0;
            if (!parseMaybeSgml(&__node_36))
            {
                expectedSymbol(AstNode::MaybeSgmlKind, "maybeSgml");
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseRuleset(RulesetAst **yynode)
{
    *yynode = create<RulesetAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        SelectorListAst *__node_37 = 0;
        if (!parseSelectorList(&__node_37))
        {
            expectedSymbol(AstNode::SelectorListKind, "selectorList");
            return false;
        }
        (*yynode)->selectors = __node_37;

        if (yytoken != Token_LBRACE)
        {
            expectedToken(yytoken, Token_LBRACE, "{");
            return false;
        }
        yylex();

        DeclarationListAst *__node_38 = 0;
        if (!parseDeclarationList(&__node_38))
        {
            expectedSymbol(AstNode::DeclarationListKind, "declarationList");
            return false;
        }
        (*yynode)->declarations = __node_38;

        if (yytoken == Token_RBRACE)
        {
            if (yytoken != Token_RBRACE)
            {
                expectedToken(yytoken, Token_RBRACE, "}");
                return false;
            }
            yylex();

        }
        else if (true /*epsilon*/)
        {
            reportProblem( Error, "Expected '}'" );
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseSelector(SelectorAst **yynode)
{
    *yynode = create<SelectorAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        SimpleSelectorAst *__node_39 = 0;
        if (!parseSimpleSelector(&__node_39))
        {
            expectedSymbol(AstNode::SimpleSelectorKind, "simpleSelector");
            return false;
        }
        (*yynode)->simpleSelector = __node_39;

    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseSelectorList(SelectorListAst **yynode)
{
    *yynode = create<SelectorListAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        SelectorAst *__node_40 = 0;
        if (!parseSelector(&__node_40))
        {
            expectedSymbol(AstNode::SelectorKind, "selector");
            return false;
        }
        (*yynode)->selectorsSequence = snoc((*yynode)->selectorsSequence, __node_40, memoryPool);

        while (yytoken == Token_COMMA)
        {
            if (yytoken != Token_COMMA)
            {
                expectedToken(yytoken, Token_COMMA, ",");
                return false;
            }
            yylex();

            SelectorAst *__node_41 = 0;
            if (!parseSelector(&__node_41))
            {
                expectedSymbol(AstNode::SelectorKind, "selector");
                return false;
            }
            (*yynode)->selectorsSequence = snoc((*yynode)->selectorsSequence, __node_41, memoryPool);

        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseSimpleSelector(SimpleSelectorAst **yynode)
{
    *yynode = create<SimpleSelectorAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_LBRACKET
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE)
    {
        MaybeSpaceAst *__node_42 = 0;
        if (!parseMaybeSpace(&__node_42))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
        if (yytoken == Token_IDENT
            || yytoken == Token_STAR)
        {
            ElementNameAst *__node_43 = 0;
            if (!parseElementName(&__node_43))
            {
                expectedSymbol(AstNode::ElementNameKind, "elementName");
                return false;
            }
            (*yynode)->element = __node_43;

            if (yytoken == Token_COLON
                || yytoken == Token_DOT
                || yytoken == Token_LBRACKET)
            {
                SpecifierListAst *__node_44 = 0;
                if (!parseSpecifierList(&__node_44))
                {
                    expectedSymbol(AstNode::SpecifierListKind, "specifierList");
                    return false;
                }
                (*yynode)->specifier = __node_44;

            }
            else if (true /*epsilon*/)
            {
            }
            else
            {
                return false;
            }
        }
        else if (yytoken == Token_COLON
                 || yytoken == Token_DOT
                 || yytoken == Token_LBRACKET)
        {
            SpecifierListAst *__node_45 = 0;
            if (!parseSpecifierList(&__node_45))
            {
                expectedSymbol(AstNode::SpecifierListKind, "specifierList");
                return false;
            }
            (*yynode)->specifier = __node_45;

        }
        else
        {
            return false;
        }
        MaybeSpaceAst *__node_46 = 0;
        if (!parseMaybeSpace(&__node_46))
        {
            expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseSpecifier(SpecifierAst **yynode)
{
    *yynode = create<SpecifierAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->className = -1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_LBRACKET)
    {
        if (yytoken == Token_DOT)
        {
            if (yytoken != Token_DOT)
            {
                expectedToken(yytoken, Token_DOT, ".");
                return false;
            }
            yylex();

            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->className = tokenStream->index() - 1;
            yylex();

        }
        else if (yytoken == Token_LBRACKET)
        {
            AttribAst *__node_47 = 0;
            if (!parseAttrib(&__node_47))
            {
                expectedSymbol(AstNode::AttribKind, "attrib");
                return false;
            }
            (*yynode)->attrib = __node_47;

        }
        else if (yytoken == Token_COLON)
        {
            PseudoAst *__node_48 = 0;
            if (!parsePseudo(&__node_48))
            {
                expectedSymbol(AstNode::PseudoKind, "pseudo");
                return false;
            }
            (*yynode)->pseudo = __node_48;

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseSpecifierList(SpecifierListAst **yynode)
{
    *yynode = create<SpecifierListAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_LBRACKET)
    {
        do
        {
            SpecifierAst *__node_49 = 0;
            if (!parseSpecifier(&__node_49))
            {
                expectedSymbol(AstNode::SpecifierKind, "specifier");
                return false;
            }
            (*yynode)->specifiersSequence = snoc((*yynode)->specifiersSequence, __node_49, memoryPool);

        }
        while (yytoken == Token_COLON
               || yytoken == Token_DOT
               || yytoken == Token_LBRACKET);
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseStart(StartAst **yynode)
{
    *yynode = create<StartAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_CHARSET_SYM
        || yytoken == Token_COLON
        || yytoken == Token_DOT
        || yytoken == Token_IDENT
        || yytoken == Token_IMPORT_SYM
        || yytoken == Token_LBRACKET
        || yytoken == Token_SGML_CD
        || yytoken == Token_STAR
        || yytoken == Token_WHITESPACE || yytoken == Token_EOF)
    {
        if (yytoken == Token_CHARSET_SYM)
        {
            CharsetAst *__node_50 = 0;
            if (!parseCharset(&__node_50))
            {
                expectedSymbol(AstNode::CharsetKind, "charset");
                return false;
            }
            (*yynode)->charset = __node_50;

        }
        else if (true /*epsilon*/)
        {
        }
        else
        {
            return false;
        }
        MaybeSgmlAst *__node_51 = 0;
        if (!parseMaybeSgml(&__node_51))
        {
            expectedSymbol(AstNode::MaybeSgmlKind, "maybeSgml");
            return false;
        }
        ImportListAst *__node_52 = 0;
        if (!parseImportList(&__node_52))
        {
            expectedSymbol(AstNode::ImportListKind, "importList");
            return false;
        }
        (*yynode)->imports = __node_52;

        RuleListAst *__node_53 = 0;
        if (!parseRuleList(&__node_53))
        {
            expectedSymbol(AstNode::RuleListKind, "ruleList");
            return false;
        }
        (*yynode)->rules = __node_53;

        if (Token_EOF != yytoken)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseStringOrUri(StringOrUriAst **yynode)
{
    *yynode = create<StringOrUriAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->string = -1;
    (*yynode)->uri = -1;

    if (yytoken == Token_STRING
        || yytoken == Token_URI)
    {
        if (yytoken == Token_STRING)
        {
            if (yytoken != Token_STRING)
            {
                expectedToken(yytoken, Token_STRING, "string");
                return false;
            }
            (*yynode)->string = tokenStream->index() - 1;
            yylex();

        }
        else if (yytoken == Token_URI)
        {
            if (yytoken != Token_URI)
            {
                expectedToken(yytoken, Token_URI, "uri");
                return false;
            }
            (*yynode)->uri = tokenStream->index() - 1;
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseTerm(TermAst **yynode)
{
    *yynode = create<TermAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->string = -1;
    (*yynode)->ident = -1;
    (*yynode)->uri = -1;
    (*yynode)->unicoderange = -1;

    if (yytoken == Token_CMS
        || yytoken == Token_DEGS
        || yytoken == Token_DIMEN
        || yytoken == Token_EMS
        || yytoken == Token_EXS
        || yytoken == Token_FLOAT
        || yytoken == Token_GRADS
        || yytoken == Token_HERZ
        || yytoken == Token_HEXCOLOR
        || yytoken == Token_IDENT
        || yytoken == Token_INS
        || yytoken == Token_INTEGER
        || yytoken == Token_KHERZ
        || yytoken == Token_MINUS
        || yytoken == Token_MMS
        || yytoken == Token_MSECS
        || yytoken == Token_PCS
        || yytoken == Token_PERCENTAGE
        || yytoken == Token_PLUS
        || yytoken == Token_PTS
        || yytoken == Token_PXS
        || yytoken == Token_QEMS
        || yytoken == Token_RADS
        || yytoken == Token_SECS
        || yytoken == Token_STRING
        || yytoken == Token_UNICODERANGE
        || yytoken == Token_URI)
    {
        if (yytoken == Token_STRING)
        {
            if (yytoken != Token_STRING)
            {
                expectedToken(yytoken, Token_STRING, "string");
                return false;
            }
            (*yynode)->string = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_54 = 0;
            if (!parseMaybeSpace(&__node_54))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_IDENT)
        {
            if (yytoken != Token_IDENT)
            {
                expectedToken(yytoken, Token_IDENT, "identifier");
                return false;
            }
            (*yynode)->ident = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_55 = 0;
            if (!parseMaybeSpace(&__node_55))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_CMS
                 || yytoken == Token_DEGS
                 || yytoken == Token_DIMEN
                 || yytoken == Token_EMS
                 || yytoken == Token_EXS
                 || yytoken == Token_FLOAT
                 || yytoken == Token_GRADS
                 || yytoken == Token_HERZ
                 || yytoken == Token_INS
                 || yytoken == Token_INTEGER
                 || yytoken == Token_KHERZ
                 || yytoken == Token_MINUS
                 || yytoken == Token_MMS
                 || yytoken == Token_MSECS
                 || yytoken == Token_PCS
                 || yytoken == Token_PERCENTAGE
                 || yytoken == Token_PLUS
                 || yytoken == Token_PTS
                 || yytoken == Token_PXS
                 || yytoken == Token_QEMS
                 || yytoken == Token_RADS
                 || yytoken == Token_SECS)
        {
            if (yytoken == Token_MINUS
                || yytoken == Token_PLUS)
            {
                UnaryOperatorAst *__node_56 = 0;
                if (!parseUnaryOperator(&__node_56))
                {
                    expectedSymbol(AstNode::UnaryOperatorKind, "unaryOperator");
                    return false;
                }
                (*yynode)->op = __node_56;

            }
            else if (true /*epsilon*/)
            {
            }
            else
            {
                return false;
            }
            if (yytoken == Token_CMS
                || yytoken == Token_DEGS
                || yytoken == Token_EMS
                || yytoken == Token_EXS
                || yytoken == Token_FLOAT
                || yytoken == Token_GRADS
                || yytoken == Token_HERZ
                || yytoken == Token_INS
                || yytoken == Token_INTEGER
                || yytoken == Token_KHERZ
                || yytoken == Token_MMS
                || yytoken == Token_MSECS
                || yytoken == Token_PCS
                || yytoken == Token_PERCENTAGE
                || yytoken == Token_PTS
                || yytoken == Token_PXS
                || yytoken == Token_QEMS
                || yytoken == Token_RADS
                || yytoken == Token_SECS)
            {
                UnaryTermAst *__node_57 = 0;
                if (!parseUnaryTerm(&__node_57))
                {
                    expectedSymbol(AstNode::UnaryTermKind, "unaryTerm");
                    return false;
                }
                (*yynode)->term = __node_57;

            }
            else if (yytoken == Token_DIMEN)
            {
                if (yytoken != Token_DIMEN)
                {
                    expectedToken(yytoken, Token_DIMEN, "dimen");
                    return false;
                }
                yylex();

            }
            else
            {
                return false;
            }
        }
        else if (yytoken == Token_URI)
        {
            if (yytoken != Token_URI)
            {
                expectedToken(yytoken, Token_URI, "uri");
                return false;
            }
            (*yynode)->uri = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_58 = 0;
            if (!parseMaybeSpace(&__node_58))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_UNICODERANGE)
        {
            if (yytoken != Token_UNICODERANGE)
            {
                expectedToken(yytoken, Token_UNICODERANGE, "unicoderange");
                return false;
            }
            (*yynode)->unicoderange = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_59 = 0;
            if (!parseMaybeSpace(&__node_59))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_HEXCOLOR)
        {
            HexcolorAst *__node_60 = 0;
            if (!parseHexcolor(&__node_60))
            {
                expectedSymbol(AstNode::HexcolorKind, "hexcolor");
                return false;
            }
            (*yynode)->hexcolor = __node_60;

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseUnaryOperator(UnaryOperatorAst **yynode)
{
    *yynode = create<UnaryOperatorAst>();

    (*yynode)->startToken = tokenStream->index() - 1;

    if (yytoken == Token_MINUS
        || yytoken == Token_PLUS)
    {
        if (yytoken == Token_MINUS)
        {
            if (yytoken != Token_MINUS)
            {
                expectedToken(yytoken, Token_MINUS, "-");
                return false;
            }
            yylex();

        }
        else if (yytoken == Token_PLUS)
        {
            if (yytoken != Token_PLUS)
            {
                expectedToken(yytoken, Token_PLUS, "+");
                return false;
            }
            yylex();

        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}

bool Parser::parseUnaryTerm(UnaryTermAst **yynode)
{
    *yynode = create<UnaryTermAst>();

    (*yynode)->startToken = tokenStream->index() - 1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;
    (*yynode)->value = -1;

    if (yytoken == Token_CMS
        || yytoken == Token_DEGS
        || yytoken == Token_EMS
        || yytoken == Token_EXS
        || yytoken == Token_FLOAT
        || yytoken == Token_GRADS
        || yytoken == Token_HERZ
        || yytoken == Token_INS
        || yytoken == Token_INTEGER
        || yytoken == Token_KHERZ
        || yytoken == Token_MMS
        || yytoken == Token_MSECS
        || yytoken == Token_PCS
        || yytoken == Token_PERCENTAGE
        || yytoken == Token_PTS
        || yytoken == Token_PXS
        || yytoken == Token_QEMS
        || yytoken == Token_RADS
        || yytoken == Token_SECS)
    {
        if (yytoken == Token_INTEGER)
        {
            if (yytoken != Token_INTEGER)
            {
                expectedToken(yytoken, Token_INTEGER, "integer");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_61 = 0;
            if (!parseMaybeSpace(&__node_61))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_FLOAT)
        {
            if (yytoken != Token_FLOAT)
            {
                expectedToken(yytoken, Token_FLOAT, "float");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_62 = 0;
            if (!parseMaybeSpace(&__node_62))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_PERCENTAGE)
        {
            if (yytoken != Token_PERCENTAGE)
            {
                expectedToken(yytoken, Token_PERCENTAGE, "%");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_63 = 0;
            if (!parseMaybeSpace(&__node_63))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_PXS)
        {
            if (yytoken != Token_PXS)
            {
                expectedToken(yytoken, Token_PXS, "ps");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_64 = 0;
            if (!parseMaybeSpace(&__node_64))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_CMS)
        {
            if (yytoken != Token_CMS)
            {
                expectedToken(yytoken, Token_CMS, "cm");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_65 = 0;
            if (!parseMaybeSpace(&__node_65))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_MMS)
        {
            if (yytoken != Token_MMS)
            {
                expectedToken(yytoken, Token_MMS, "mm");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_66 = 0;
            if (!parseMaybeSpace(&__node_66))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_INS)
        {
            if (yytoken != Token_INS)
            {
                expectedToken(yytoken, Token_INS, "in");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_67 = 0;
            if (!parseMaybeSpace(&__node_67))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_PTS)
        {
            if (yytoken != Token_PTS)
            {
                expectedToken(yytoken, Token_PTS, "pt");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_68 = 0;
            if (!parseMaybeSpace(&__node_68))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_PCS)
        {
            if (yytoken != Token_PCS)
            {
                expectedToken(yytoken, Token_PCS, "pc");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_69 = 0;
            if (!parseMaybeSpace(&__node_69))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_DEGS)
        {
            if (yytoken != Token_DEGS)
            {
                expectedToken(yytoken, Token_DEGS, "deg");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_70 = 0;
            if (!parseMaybeSpace(&__node_70))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_RADS)
        {
            if (yytoken != Token_RADS)
            {
                expectedToken(yytoken, Token_RADS, "rad");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_71 = 0;
            if (!parseMaybeSpace(&__node_71))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_GRADS)
        {
            if (yytoken != Token_GRADS)
            {
                expectedToken(yytoken, Token_GRADS, "grad");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_72 = 0;
            if (!parseMaybeSpace(&__node_72))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_MSECS)
        {
            if (yytoken != Token_MSECS)
            {
                expectedToken(yytoken, Token_MSECS, "ms");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_73 = 0;
            if (!parseMaybeSpace(&__node_73))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_SECS)
        {
            if (yytoken != Token_SECS)
            {
                expectedToken(yytoken, Token_SECS, "s");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_74 = 0;
            if (!parseMaybeSpace(&__node_74))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_HERZ)
        {
            if (yytoken != Token_HERZ)
            {
                expectedToken(yytoken, Token_HERZ, "Hz");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_75 = 0;
            if (!parseMaybeSpace(&__node_75))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_KHERZ)
        {
            if (yytoken != Token_KHERZ)
            {
                expectedToken(yytoken, Token_KHERZ, "kHz");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_76 = 0;
            if (!parseMaybeSpace(&__node_76))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_EMS)
        {
            if (yytoken != Token_EMS)
            {
                expectedToken(yytoken, Token_EMS, "em");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_77 = 0;
            if (!parseMaybeSpace(&__node_77))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_QEMS)
        {
            if (yytoken != Token_QEMS)
            {
                expectedToken(yytoken, Token_QEMS, "__qem");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_78 = 0;
            if (!parseMaybeSpace(&__node_78))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else if (yytoken == Token_EXS)
        {
            if (yytoken != Token_EXS)
            {
                expectedToken(yytoken, Token_EXS, "ex");
                return false;
            }
            (*yynode)->value = tokenStream->index() - 1;
            yylex();

            MaybeSpaceAst *__node_79 = 0;
            if (!parseMaybeSpace(&__node_79))
            {
                expectedSymbol(AstNode::MaybeSpaceKind, "maybeSpace");
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    (*yynode)->endToken = tokenStream->index() - 2;

    return true;
}


} // end of namespace Css

