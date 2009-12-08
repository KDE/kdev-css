
#include "tokenizertest.h"

#include <QTest>

#include "../cssparser.h"

QTEST_MAIN(Css::TestTokenizer)


Css::TestTokenizer::TestTokenizer()
{
}


void Css::TestTokenizer::firstTest()
{
    CSSParser p;
    QString str("a {\n"
                "    font-weight: bold;\n"
                "    color: red;\n"
                "}");
    p.parse(str);
    
}


//#include "tokenizertest.moc"
