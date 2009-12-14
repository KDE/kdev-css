/*
   This file is part of KDevelop
   Copyright 2009 Niko Sams <niko.sams@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "parsertest.h"

#include <QTest>

#include "../parsesession.h"
#include "cssdebugvisitor.h"

QTEST_MAIN(Css::TestParser)

namespace Css
{

TestParser::TestParser()
{
}

void parse(const QString &contents)
{
}

void TestParser::parser_data()
{
    QTest::addColumn<QString>("contents");

    QTest::newRow("one declaration without semicolon")     << "body{color:red}";
    QTest::newRow("one declaration with semicolon") << "body{color:red;}";
    QTest::newRow("selector element") << "body{}";
    QTest::newRow("selector class") << ".foo{}";
    QTest::newRow("selector element.class") << "body.foo{}";
    QTest::newRow("selector star") << "*{}";
    QTest::newRow("selector star.class") << "*.foo{}";
    QTest::newRow("selector element attribute1") << "body[foo]{}";
    QTest::newRow("selector element attribute2") << "body[foo~=bar]{}";
    QTest::newRow("selector element attribute3") << "body[foo|=bar]{}";
    QTest::newRow("selector element pseudoclass") << "body:hover{}";
    QTest::newRow("selector element pseudoclass function") << "body:lang(no){}";
    QTest::newRow("selector element pseudoclass nth-child1") << "body:nth-child(even){}";
    QTest::newRow("selector element pseudoclass nth-child2") << "body:nth-child(4){}";
    QTest::newRow("selector element pseudoclass nth-child3") << "body:nth-child(5n+2){}";
    QTest::newRow("selector element pseudoclass selection") << "body::selection{}";
    QTest::newRow("declaration with px term") << "body{width:100px;}";
    QTest::newRow("declaration with em term") << "body{width:5em;}";
    QTest::newRow("declaration with none term") << "body{border:none;}";
    QTest::newRow("declaration with negative px term") << "body{width:-100px;}";
    QTest::newRow("declaration with positive px term") << "body{width:+100px;}";
    QTest::newRow("declaration with url term1") << "body{background-image:url(http://example.com/foo.png);}";
    QTest::newRow("declaration with url term2") << "body{background-image:url(/foo.png);}";
    QTest::newRow("declaration hex color term1") << "body{color:#ff0000;}";
    QTest::newRow("declaration hex color term2") << "body{color:#f00;}";
    QTest::newRow("declaration with multipe px terms") << "body{border-width:1px 2px 3px 4px;}";
    QTest::newRow("charset") << "@charset \"utf-8\"; body{}";
    QTest::newRow("import url") << "@import url(\"foo.css\"); body{}";
    QTest::newRow("import string") << "@import \"foo.css\"; body{}";
    QTest::newRow("two rules") << "h1{}h2{}";
    QTest::newRow("two rules whitespace") << "h1{} h2{}";
}

void TestParser::parser()
{
    QFETCH(QString, contents);

    ParseSession session;
    session.setContents(contents);
    StartAst* ast = 0;
    QVERIFY(session.parse(&ast));
    DebugVisitor debugVisitor(session.tokenStream(), session.contents());
    debugVisitor.visitStart(ast);
}

}

#include "parsertest.moc"
