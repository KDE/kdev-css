#include "modeltest.h"

#include <qtest_kde.h>
#include <KDebug>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/View>

#include "../model.h"

namespace Css {

int debugArea() { static int s_area = KDebug::registerArea("kdevcsssupport"); return s_area; }

ModelTest::ModelTest()
{
}

void ModelTest::testCompletionRange()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{font-w:normal;}");
                //01234567890123456789 
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(0, 9);
    KTextEditor::Range range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));

    position = KTextEditor::Cursor(0, 10);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));


    position = KTextEditor::Cursor(0, 11);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 11));
    QCOMPARE(doc->text(range), QString("font-w"));

    doc->setText("body{font-:normal;}");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 9);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 10));
    QCOMPARE(doc->text(range), QString("font-"));

    position = KTextEditor::Cursor(0, 10);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 5, 0, 10));
    QCOMPARE(doc->text(range), QString("font-"));

    doc->setText("font-");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 5);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 0, 0, 5));
    QCOMPARE(doc->text(range), QString("font-"));

    doc->setText("font-weight:normal");
                //01234567890123456789

    position = KTextEditor::Cursor(0, 12);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 12, 0, 18));
    QCOMPARE(doc->text(range), QString("normal"));

    position = KTextEditor::Cursor(0, 15);
    range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(0, 12, 0, 18));
    QCOMPARE(doc->text(range), QString("normal"));
    delete doc;
}

void ModelTest::testCompletionRangeSecondLine()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{color:red;}\nbody{font-w:normal;}");
                //                  01234567890123456789
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(1, 9);
    KTextEditor::Range range = model->completionRange(view, position);
    kDebug() << range << doc->text(range);
    QCOMPARE(range, KTextEditor::Range(1, 5, 1, 11));
    QCOMPARE(doc->text(range), QString("font-w"));
    delete doc;
}

bool containsCompletion(CodeCompletionModel* model, QString text)
{
    QStringList completions;
    for (int i=0; i < model->rowCount(); ++i) {
        completions << model->data(model->index(i, CodeCompletionModel::Name), Qt::DisplayRole).toString();
        kDebug() << model->data(model->index(i, CodeCompletionModel::Name), Qt::DisplayRole).toString();
    }
    return completions.contains(text);
}

void ModelTest::testCompletionElement()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{font-w:normal;}");
                //01234567890123456789
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(0, 10);
    QCOMPARE(model->rowCount(), 0);
    model->completionInvoked(view, model->completionRange(view, position), KTextEditor::CodeCompletionModel::ManualInvocation);
    QVERIFY(containsCompletion(model, "font-weight"));
}

void ModelTest::testCompletionField()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{font-weight:normal;}");
                //01234567890123456789
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(0, 17);
    QCOMPARE(model->rowCount(), 0);
    model->completionInvoked(view, model->completionRange(view, position), KTextEditor::CodeCompletionModel::ManualInvocation);
    QVERIFY(containsCompletion(model, "normal"));
    QVERIFY(containsCompletion(model, "bold"));
}

void ModelTest::testCompletionElementSecondLine()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);
    doc->setText("body{color:red;}\nbody{font-w:normal;}");
                //01234567890       01234567890123456789
    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    KTextEditor::Cursor position(1, 9);
    QCOMPARE(model->rowCount(), 0);
    model->completionInvoked(view, model->completionRange(view, position), KTextEditor::CodeCompletionModel::ManualInvocation);
    QVERIFY(containsCompletion(model, "font-weight"));
}


}

QTEST_KDEMAIN(Css::ModelTest, GUI)

#include "modeltest.moc"
