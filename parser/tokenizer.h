/*
 * This file is part of the DOM implementation for KDE.
 *
 * Copyright 2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.
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
#ifndef _CSS_cssparser_h_
#define _CSS_cssparser_h_

#include <QtCore/QString>
#include <QtGui/QColor>
#include <QtCore/QVector>
#include "parserexport.h"

namespace KDevPG {
    class TokenStream;
}

namespace Css {

    class KDEVCSSPARSER_EXPORT Tokenizer
    {
    public:
        Tokenizer(KDevPG::TokenStream *tokenStream, const QString &string);
        ~Tokenizer();

    // tokenizer methods and data
    public:
        int token() const;
        int tokenBegin() const;
        int tokenEnd() const;

        int lex();

    private:
        int _lex();
        int yyparse();

        KDevPG::TokenStream *m_tokenStream;
        unsigned short *data;
        unsigned short *yytext;
        unsigned short *yy_c_buf_p;
        unsigned short yy_hold_char;
        int yy_last_accepting_state;
        unsigned short *yy_last_accepting_cpos;
        int yyleng;
        int yyTok;
        int yy_start;
    };

} // namespace
#endif
