/*
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright 2003 Lars Knoll (knoll@kde.org)
 * Copyright 2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Computer, Inc.
 * Copyright (C) 2008 Maksim Orlovich <maksim@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#define TOKEN_DEBUG
#define YYDEBUG 0

#include "tokenizer.h"
#include "cssparser.h"

#include <kdebug.h>
#include <kglobal.h>
#include <kurl.h>

#include <stdlib.h>
#include <assert.h>

using namespace Css;

//TODO NIKO:
#define END 0

#if YYDEBUG > 0
extern int cssyydebug;
#endif

Tokenizer::Tokenizer(KDevPG::TokenStream *tokenStream, const QString &string)
    : m_tokenStream(tokenStream)
{
    yy_start = 1;

#if YYDEBUG > 0
    cssyydebug = 1;
#endif

    int length = string.length() + 8;

    data = (unsigned short *)malloc( length *sizeof( unsigned short ) );
    memcpy(data, string.unicode(), string.length()*sizeof( unsigned short));

    // the flex scanner sometimes give invalid reads for any
    // smaller padding - try e.g. css/invalid-rules-005.html or see #167318
    data[length - 1] = 0;
    data[length - 2] = 0;
    data[length - 3] = 0;
    data[length - 4] = 0;
    data[length - 5] = 0;
    data[length - 6] = 0;
    data[length - 7] = 0;
    data[length - 8] = 0;

    yyTok = -1;
    yy_hold_char = 0;
    yyleng = 0;
    yytext = yy_c_buf_p = data;
    yy_hold_char = *yy_c_buf_p;

}

Tokenizer::~Tokenizer()
{
    free( data );
}

static inline int yyerror( const char *str ) {
//    assert( 0 );
#ifdef CSS_DEBUG
    kDebug( 6080 ) << "CSS parse error " << str;
#else
    Q_UNUSED( str );
#endif
    return 1;
}

int Tokenizer::token() const
{
    return yyTok;
}

int Tokenizer::tokenBegin() const
{
    return (yytext - data);
}

int Tokenizer::tokenEnd() const
{
    return tokenBegin() + yyleng - 1;
}

int Tokenizer::lex()
{
    int token = _lex();
    for (unsigned short *d = yytext; d < yytext + yyleng; ++d) {
        if (*d == '\n') {
            m_tokenStream->locationTable()->newline(d - data);
        }
    }
    return token;
}


// When we reach the end of the input we switch over
// the lexer to this alternative buffer and keep it stuck here.
// (and as it contains nulls, flex will keep on reporting
//  end of buffer, and we will keep reseting the input
//  pointer to the beginning of this).
static unsigned short postEofBuf[2];

#define YY_DECL int Tokenizer::_lex()
#define yyconst const
typedef int yy_state_type;
typedef unsigned int YY_CHAR;
// this line makes sure we treat all Unicode chars correctly.
#define YY_SC_TO_UI(c) (c > 0xff ? 0xff : c)
#define YY_DO_BEFORE_ACTION \
        yytext = yy_bp; \
        yyleng = (int) (yy_cp - yy_bp); \
        yy_hold_char = *yy_cp; \
        *yy_cp = 0; \
        yy_c_buf_p = yy_cp;
#define YY_BREAK break;
#define ECHO qDebug( "%s", QString( (QChar *)yytext, yyleng ).toLatin1().constData() )
#define YY_RULE_SETUP
#define INITIAL 0
#define YY_STATE_EOF(state) (YY_END_OF_BUFFER + state + 1)
#define YY_START ((yy_start - 1) / 2)
#define yyterminate()\
    do { \
        if (yy_act == YY_END_OF_BUFFER) { \
            yy_c_buf_p = postEofBuf; \
            yy_hold_char = 0; /* first char of the postEndOf to 'restore' */ \
        } \
        yyTok = END; return yyTok; \
     } while (0)
#define YY_FATAL_ERROR(a) qFatal(a)
#define BEGIN yy_start = 1 + 2 *
#define COMMENT 1

#include "generated/tokenizer.cpp"
// kate: indent-width 4; replace-tabs on; tab-width 4; space-indent on; hl c++;
