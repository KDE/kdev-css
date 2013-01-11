#include "parsejobtest.h"

#include <qtest_kde.h>

#include <tests/autotestshell.h>
#include <tests/testcore.h>
#include <interfaces/ilanguagecontroller.h>
#include <language/backgroundparser/backgroundparser.h>
#include <tests/testproject.h>
#include <tests/testfile.h>
#include <language/duchain/duchainlock.h>
#include <language/duchain/duchain.h>
#include <language/duchain/parsingenvironment.h>

QTEST_KDEMAIN(Css::TestParseJob, GUI)

namespace Css {

void TestParseJob::initTestCase()
{
    KDevelop::AutoTestShell::init();
    KDevelop::TestCore* core = KDevelop::TestCore::initialize(KDevelop::Core::NoUi);
    m_projectController = new KDevelop::TestProjectController(core);
    core->setProjectController(m_projectController);
}

void TestParseJob::cleanupTestCase()
{
    KDevelop::TestCore::shutdown();
}

void TestParseJob::testSimple()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    KDevelop::TestFile f("a { color: red; }", "css", project);
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testSimpleHtml()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    KDevelop::TestFile f("<html><style>a { color: red; }</style></html>", "html", project);
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 1);
}

void TestParseJob::testHtmlTwoStyleElements()
{
    KDevelop::TopDUContext::Features features = KDevelop::TopDUContext::VisibleDeclarationsAndContexts;

    KDevelop::TestProject* project = new KDevelop::TestProject;
    m_projectController->clearProjects();
    m_projectController->addProject(project);

    KDevelop::TestFile f("<html><style>a { color: red; }</style><style>h1 { font-weight:normal; }</style></html>",
                         "html", project);
    f.parse(features);

    KDevelop::ReferencedTopDUContext top = f.topContext();

    KDevelop::DUChainReadLocker lock(KDevelop::DUChain::lock());
    QVERIFY(top);
    QVERIFY(top->parsingEnvironmentFile()->language() == KDevelop::IndexedString("Css"));
    QCOMPARE(top->childContexts().count(), 2);
}


}

#include "parsejobtest.moc"
