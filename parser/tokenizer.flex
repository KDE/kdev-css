%option noyywrap
%option case-insensitive
%option noyywrap
%option 8bit
%option stack
%s mediaquery at_rule block

h               [0-9a-fA-F]
nonascii        [\200-\377]
unicode         \\{h}{1,6}[ \t\r\n\f]?
escape          {unicode}|\\[ -~\200-\377]
nmstart         [_a-zA-Z]|{nonascii}|{escape}
nmchar          [_a-zA-Z0-9-]|{nonascii}|{escape}
string1         \"([\t !#$%&(-~]|\\{nl}|\'|{nonascii}|{escape})*\"
string2         \'([\t !#$%&(-~]|\\{nl}|\"|{nonascii}|{escape})*\'
hexcolor        {h}{3}|{h}{6}

ident           -?{nmstart}{nmchar}*
name            {nmchar}+
num             [0-9]+|[0-9]*"."[0-9]+
intnum          [0-9]+
string          {string1}|{string2}
url             ([!#$%&*-~]|{nonascii}|{escape})*
w               [ \t\r\n\f]*
nl              \n|\r\n|\r|\f
range           \?{1,6}|{h}(\?{0,5}|{h}(\?{0,4}|{h}(\?{0,3}|{h}(\?{0,2}|{h}(\??|{h})))))
nth             (-?[0-9]*n[\+-][0-9]+)|(-?[0-9]*n)

%%

\/\*[^*]*\*+([^/*][^*]*\*+)*\/  /* ignore comments */

[ \t\r\n\f]+            {yyTok = Parser::Token_WHITESPACE; return yyTok;}

"<!--"                  {yyTok = Parser::Token_SGML_CD; return yyTok;}
"-->"                   {yyTok = Parser::Token_SGML_CD; return yyTok;}
"~="                    {yyTok = Parser::Token_INCLUDES; return yyTok;}
"|="                    {yyTok = Parser::Token_DASHMATCH; return yyTok;}
"^="                    {yyTok = Parser::Token_BEGINSWITH; return yyTok;}
"$="                    {yyTok = Parser::Token_ENDSWITH; return yyTok;}
"*="                    {yyTok = Parser::Token_CONTAINS; return yyTok;}
<mediaquery>"not"       {yyTok = Parser::Token_MEDIA_NOT; return yyTok;}
<mediaquery>"only"      {yyTok = Parser::Token_MEDIA_ONLY; return yyTok;}
<mediaquery>"and"       {yyTok = Parser::Token_MEDIA_AND; return yyTok;}

{string}                {yyTok = Parser::Token_STRING; return yyTok;}
{ident}                 {yyTok = Parser::Token_IDENT; return yyTok;}
{nth}                   {yyTok = Parser::Token_NTH; return yyTok;}


<block>"#"{hexcolor}           {yyTok = Parser::Token_HEXCOLOR; return yyTok;}
"#"{ident}              {yyTok = Parser::Token_HASH; return yyTok;}
 /* @rule tokens surrounding css declaration blocks with { } braces must start a BEGIN(at_rule) context */
"@import"               {BEGIN(mediaquery); yyTok = Parser::Token_IMPORT_SYM; return yyTok;}
"@page"                 {BEGIN(at_rule); yyTok = Parser::Token_PAGE_SYM; return yyTok;}
"@media"                {BEGIN(mediaquery); yyTok = Parser::Token_MEDIA_SYM; return yyTok;}
"@font-face"            {BEGIN(at_rule); yyTok = Parser::Token_FONT_FACE_SYM; return yyTok;}
"@charset"              {BEGIN(at_rule); yyTok = Parser::Token_CHARSET_SYM; return yyTok;}
"@namespace"        {BEGIN(at_rule); yyTok = Parser::Token_NAMESPACE_SYM; return yyTok; }

"!"{w}"important"         {yyTok = Parser::Token_IMPORTANT_SYM; return yyTok;}

{num}em                 {yyTok = Parser::Token_EMS; return yyTok;}
{num}__qem              {yyTok = Parser::Token_QEMS; return yyTok;} /* quirky ems */
{num}ex                 {yyTok = Parser::Token_EXS; return yyTok;}
{num}px                 {yyTok = Parser::Token_PXS; return yyTok;}
{num}cm                 {yyTok = Parser::Token_CMS; return yyTok;}
{num}mm                 {yyTok = Parser::Token_MMS; return yyTok;}
{num}in                 {yyTok = Parser::Token_INS; return yyTok;}
{num}pt                 {yyTok = Parser::Token_PTS; return yyTok;}
{num}pc                 {yyTok = Parser::Token_PCS; return yyTok;}
{num}deg                {yyTok = Parser::Token_DEGS; return yyTok;}
{num}rad                {yyTok = Parser::Token_RADS; return yyTok;}
{num}grad               {yyTok = Parser::Token_GRADS; return yyTok;}
{num}ms                 {yyTok = Parser::Token_MSECS; return yyTok;}
{num}s                  {yyTok = Parser::Token_SECS; return yyTok;}
{num}Hz                 {yyTok = Parser::Token_HERZ; return yyTok;}
<mediaquery>{num}dpi    {yyTok = Parser::Token_DPI; return yyTok;}
<mediaquery>{num}dpcm   {yyTok = Parser::Token_DPCM; return yyTok;}
{num}kHz                {yyTok = Parser::Token_KHERZ; return yyTok;}
{num}{ident}            {yyTok = Parser::Token_DIMEN; return yyTok;}
{num}%                  {yyTok = Parser::Token_PERCENTAGE; return yyTok;}
{intnum}                {yyTok = Parser::Token_INTEGER; return yyTok;}
{num}                   {yyTok = Parser::Token_FLOAT; return yyTok;}


"not("                  {yyTok = Parser::Token_NOTFUNCTION; return yyTok;}
"url("{w}{string}{w}")" {yyTok = Parser::Token_URI; return yyTok;}
"url("{w}{url}{w}")"    {yyTok = Parser::Token_URI; return yyTok;}
{ident}"("              {yyTok = Parser::Token_FUNCTION; return yyTok;}

U\+{range}              {yyTok = Parser::Token_UNICODERANGE; return yyTok;}
U\+{h}{1,6}-{h}{1,6}    {yyTok = Parser::Token_UNICODERANGE; return yyTok;}

<INITIAL>"{"            {BEGIN(block); yyTok = Parser::Token_LBRACE; return yyTok;}
<at_rule>"{"            {BEGIN(block); yyTok = Parser::Token_LBRACE; return yyTok;}
<at_rule>";"            {BEGIN(block); yyTok = Parser::Token_SEMICOLON; return yyTok;}
<block>"}"              {BEGIN(block); yyTok = Parser::Token_RBRACE; return yyTok;}
<mediaquery>"{"         {BEGIN(block); yyTok = Parser::Token_LBRACE; return yyTok;}
<mediaquery>";"         {BEGIN(block); yyTok = Parser::Token_SEMICOLON; return yyTok;}

,                       {BEGIN(block); yyTok = Parser::Token_COMMA; return yyTok;}
\(                      {BEGIN(block); yyTok = Parser::Token_LPAREN; return yyTok;}
\)                      {BEGIN(block); yyTok = Parser::Token_RPAREN; return yyTok;}
\{                      {BEGIN(block); yyTok = Parser::Token_LBRACE; return yyTok;}
\}                      {BEGIN(block); yyTok = Parser::Token_RBRACE; return yyTok;}
\[                      {BEGIN(block); yyTok = Parser::Token_LBRACKET; return yyTok;}
\]                      {BEGIN(block); yyTok = Parser::Token_RBRACKET; return yyTok;}
:                       {BEGIN(block); yyTok = Parser::Token_COLON; return yyTok;}
\*                      {BEGIN(block); yyTok = Parser::Token_STAR; return yyTok;}
;                       {BEGIN(block); yyTok = Parser::Token_SEMICOLON; return yyTok;}
\.                      {BEGIN(block); yyTok = Parser::Token_DOT; return yyTok;}
=                       {BEGIN(block); yyTok = Parser::Token_EQUALS; return yyTok;}
\+                      {BEGIN(block); yyTok = Parser::Token_PLUS; return yyTok;}
\-                      {BEGIN(block); yyTok = Parser::Token_MINUS; return yyTok;}
\/                       {BEGIN(block); yyTok = Parser::Token_DIVIDE; return yyTok;}

 /* add all tokens that match here above */
.                       {BEGIN(block); yyTok = Parser::Token_INVALID; return yyTok;}
%%

namespace Css
{

Lexer::Lexer( Parser *parser, char *contents )
{
    restart( parser, contents );
}

void Lexer::restart( Parser *parser, char *contents )
{
    m_parser = parser;
    m_locationTable = parser->tokenStream->locationTable();
    m_contents = contents;
    m_tokenBegin = m_tokenEnd = 0;
    m_currentOffset = 0;

    // check for and ignore the UTF-8 byte order mark
    unsigned char *ucontents = (unsigned char *) m_contents;
    if ( ucontents[0] == 0xEF && ucontents[1] == 0xBB && ucontents[2] == 0xBF )
    {
        m_tokenBegin = m_tokenEnd = 3;
        m_currentOffset = 3;
    }

    yyrestart(NULL);
    BEGIN(INITIAL); // is not set automatically by yyrestart()
}

// reads a character, and returns 1 as the number of characters read
// (or 0 when the end of the string is reached)
int Lexer::LexerInput( char *buf, int /*max_size*/ )
{
    int c = m_contents[ m_currentOffset++ ];

    switch(c)
    {
    case '\r':
        c = '\n'; // only have one single line break character: '\n'
        if ( m_contents[m_currentOffset + 1] == '\n' )
        {
            m_currentOffset++;
            m_tokenEnd++;
        }

        // fall through
    case '\n':
        m_locationTable->newline( m_currentOffset - 1 );
        break;

    default:
        break;
    }

    return (c == 0) ? 0 : (buf[0] = c, 1);
}

} // end of namespace Css
