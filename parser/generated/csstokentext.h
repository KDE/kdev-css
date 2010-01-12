// THIS FILE IS GENERATED
// WARNING! All changes made in this file will be lost!

#ifndef CSS_TOKEN_TEXT_H_INCLUDED
#define CSS_TOKEN_TEXT_H_INCLUDED

#include "cssparser.h"

namespace Css
{

QString tokenText(int token)
{
    switch (token)
    {
    case Parser::Token_BEGINSWITH:
        return "^=";
    case Parser::Token_CHARSET_SYM:
        return "@charset";
    case Parser::Token_CMS:
        return "cm";
    case Parser::Token_COLON:
        return ":";
    case Parser::Token_COMMA:
        return ",";
    case Parser::Token_CONTAINS:
        return "*=";
    case Parser::Token_DASHMATCH:
        return "|=";
    case Parser::Token_DEGS:
        return "deg";
    case Parser::Token_DIMEN:
        return "dimen";
    case Parser::Token_DIVIDE:
        return "/";
    case Parser::Token_DOT:
        return ".";
    case Parser::Token_DPCM:
        return "dpcm";
    case Parser::Token_DPI:
        return "dpi";
    case Parser::Token_EMS:
        return "em";
    case Parser::Token_ENDSWITH:
        return "$=";
    case Parser::Token_EOF:
        return "end of file";
    case Parser::Token_EQUALS:
        return "=";
    case Parser::Token_EXS:
        return "ex";
    case Parser::Token_FLOAT:
        return "float";
    case Parser::Token_FONT_FACE_SYM:
        return "@font-face";
    case Parser::Token_FUNCTION:
        return "function";
    case Parser::Token_GRADS:
        return "grad";
    case Parser::Token_HERZ:
        return "Hz";
    case Parser::Token_HEXCOLOR:
        return "hex color";
    case Parser::Token_IDENT:
        return "identifier";
    case Parser::Token_IDSEL:
        return "id selector";
    case Parser::Token_IMPORTANT_SYM:
        return "!important";
    case Parser::Token_IMPORT_SYM:
        return "@import";
    case Parser::Token_INCLUDES:
        return "~=";
    case Parser::Token_INS:
        return "in";
    case Parser::Token_INTEGER:
        return "integer";
    case Parser::Token_INVALID:
        return "invalid token";
    case Parser::Token_KHERZ:
        return "kHz";
    case Parser::Token_LBRACE:
        return "{";
    case Parser::Token_LBRACKET:
        return "[";
    case Parser::Token_LPAREN:
        return "(";
    case Parser::Token_MEDIA_AND:
        return "media and";
    case Parser::Token_MEDIA_NOT:
        return "media not";
    case Parser::Token_MEDIA_ONLY:
        return "media only";
    case Parser::Token_MEDIA_SYM:
        return "@media";
    case Parser::Token_MINUS:
        return "-";
    case Parser::Token_MMS:
        return "mm";
    case Parser::Token_MSECS:
        return "ms";
    case Parser::Token_NAMESPACE_SYM:
        return "@namespace";
    case Parser::Token_NOTFUNCTION:
        return "not function";
    case Parser::Token_NTH:
        return "nth";
    case Parser::Token_PAGE_SYM:
        return "@page";
    case Parser::Token_PCS:
        return "pc";
    case Parser::Token_PERCENTAGE:
        return "%";
    case Parser::Token_PLUS:
        return "+";
    case Parser::Token_PTS:
        return "pt";
    case Parser::Token_PXS:
        return "ps";
    case Parser::Token_QEMS:
        return "__qem";
    case Parser::Token_RADS:
        return "rad";
    case Parser::Token_RBRACE:
        return "}";
    case Parser::Token_RBRACKET:
        return "]";
    case Parser::Token_RPAREN:
        return ")";
    case Parser::Token_SECS:
        return "s";
    case Parser::Token_SEMICOLON:
        return ";";
    case Parser::Token_SGML_CD:
        return "<!--/-->";
    case Parser::Token_STAR:
        return "*";
    case Parser::Token_STRING:
        return "string";
    case Parser::Token_UNICODERANGE:
        return "unicoderange";
    case Parser::Token_URI:
        return "uri";
    case Parser::Token_WHITESPACE:
        return "whitespace";
    default:
        return "unknown token";
    }
}
} // end of namespace Css

#endif

