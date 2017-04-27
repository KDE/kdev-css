#include "parsejobtest.h"

#include <tests/autotestshell.h>
#include <tests/testcore.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/backgroundparser/backgroundparser.h>
#include <tests/testproject.h>
#include <tests/testfile.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/parsingenvironment.h>

#include <QtTest>

QTEST_MAIN(Css::TestParseJob)

using namespace KDevelop;

namespace Css {

void TestParseJob::initTestCase()
{
    AutoTestShell::init();
    TestCore* core = TestCore::initialize(Core::NoUi);
    m_projectController = new TestProjectController(core);
    core->setProjectController(m_projectController);
}

void TestParseJob::cleanupTestCase()
{
    TestCore::shutdown();
}

void TestParseJob::testSimple()
{
    TopDUContext::Features features = TopDUContext::VisibleDeclarationsAndContexts;

    TestProject* project = new TestProject;
    m_projectController->closeAllProjects();
    m_projectController->addProject(project);

    TestFile f("a { color: red; }", "css", project);
    f.parse(features);

    ReferencedTopDUContext top = f.topContext();

    DUChainReadLocker lock;
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testSimpleHtml()
{
    TopDUContext::Features features = TopDUContext::VisibleDeclarationsAndContexts;

    TestProject* project = new TestProject;
    m_projectController->closeAllProjects();
    m_projectController->addProject(project);

    TestFile f("<html><style>a { color: red; }</style></html>", "html", project);
    f.parse(features);

    ReferencedTopDUContext top = f.topContext();

    DUChainReadLocker lock;
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testHtmlTwoStyleElements()
{
    TopDUContext::Features features = TopDUContext::VisibleDeclarationsAndContexts;

    TestProject* project = new TestProject;
    m_projectController->closeAllProjects();
    m_projectController->addProject(project);

    TestFile f("<html><style>a { color: red; }</style><style>h1 { font-weight:normal; }</style></html>",
                         "html", project);
    f.parse(features);

    ReferencedTopDUContext top = f.topContext();

    DUChainReadLocker lock;
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 2);
}


}

