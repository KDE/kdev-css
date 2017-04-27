/* This file is part of KDevelop
    Copyright 2006 Hamish Rodda <rodda@kde.org>
    Copyright 2008 Niko Sams <niko.sams@gmail.com>

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

#include "duchaintestbase.h"

#include <QtTest/QtTest>

#include <KComponentData>

#include <language/duchain/duchaindumper.h>
#include <language/duchain/parsingenvironment.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/topducontext.h>
#include <serialization/indexedstring.h>
#include <language/codegen/coderepresentation.h>
#include <tests/autotestshell.h>
#include <tests/testcore.h>

#include "../../parser/parsesession.h"
#include "../../parser/editorintegrator.h"
#include "cssdebugvisitor.h"
#include "../builders/declarationbuilder.h"

using namespace Css;
using namespace KDevelop;

DUChainTestBase::DUChainTestBase()
{
}

void DUChainTestBase::initTestCase()
{
    AutoTestShell::init();
    TestCore::initialize(Core::NoUi);

    DUChain::self()->disablePersistentStorage();
    CodeRepresentation::setDiskChangesForbidden(true);
}

void DUChainTestBase::cleanupTestCase()
{
    TestCore::shutdown();
}

TopDUContext* DUChainTestBase::parse(const QByteArray& unit, DumpAreas dump, QString url)
{
    if (dump)
        qDebug() << "==== Beginning new test case...:" << endl << unit;

    ParseSession* session = new ParseSession();
    session->setContents(unit);
    StartAst* ast = 0;
    if (!session->parse(&ast)) {
        qDebug() << "Parse failed";
        return 0;
    }

    if (dump & DumpAST) {
        qDebug() << "===== AST:";
        DebugVisitor debugVisitor(session->tokenStream(), session->contents());
        debugVisitor.visitNode(ast);
    }

    static int testNumber = 0;
    if (url.isEmpty()) url = QString("file:///internal/%1").arg(testNumber++);

    EditorIntegrator editor;
    editor.setParseSession(session);
    DeclarationBuilder builder(&editor);
    TopDUContext* top = builder.build(IndexedString(url), ast);

    if (dump & DumpDUChain) {
        qDebug() << "===== DUChain:";

        DUChainReadLocker lock;
        DUChainDumper dumper;
        dumper.dump(top);
    }

    if (dump)
        qDebug() << "===== Finished test case.";

    delete session;

    return top;
}

