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
        //kDebug() << model->data(model->index(i, CodeCompletionModel::Name), Qt::DisplayRole).toString();
    }
    return completions.contains(text);
}

void ModelTest::completionItems_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<int>("positionRow");
    QTest::addColumn<int>("positionLine");
    QTest::addColumn<QStringList>("result");

    QTest::newRow("completion element")
          //012345678901234567890123
        << "body{font-w:normal;}"
        << 0 << 10
        << (QStringList() << "font-weight");

    QTest::newRow("field")
          //012345678901234567890123
        << "body{font-weight:normal;}"
        << 0 << 17
        << (QStringList() << "normal" << "bold");

    QTest::newRow("element second line")
          //01234567890123456 012345678901234567890
        << "body{color:red;}\nbody{font-w:normal;}"
        << 1 << 9
        << (QStringList() << "font-weight");

    QTest::newRow("selector at start")
          //0123456
        << "body{}"
        << 0 << 0
        << (QStringList() << "body" << "a");

    QTest::newRow("selector")
          //0123456
        << "body{}"
        << 0 << 6
        << (QStringList() << "body" << "a");

    QTest::newRow("selector second line")
          //012345678901234567890123456 0123456789012345678901234
        << "body{font-weight: bolder;}\nbody{font-weight: asdf;}"
        << 0 << 26
        << (QStringList() << "body" << "a");

    QTest::newRow("selector with space")
          //0123456
        << "body{} "
        << 0 << 7
        << (QStringList() << "body" << "a");
}

void ModelTest::completionItems()
{
    KTextEditor::Document* doc = KTextEditor::EditorChooser::editor()->createDocument(0);

    QFETCH(QString, text);
    doc->setText(text);

    KTextEditor::View* view = doc->createView(0);
    CodeCompletionModel* model = new CodeCompletionModel(doc);

    QFETCH(int, positionRow);
    QFETCH(int, positionLine);
    KTextEditor::Cursor position(positionRow, positionLine);
    QCOMPARE(model->rowCount(), 0);
    model->completionInvoked(view, model->completionRange(view, position), KTextEditor::CodeCompletionModel::ManualInvocation);
    QStringList completions;
    for (int i=0; i < model->rowCount(); ++i) {
        completions << model->data(model->index(i, CodeCompletionModel::Name), Qt::DisplayRole).toString();
    }
    QFETCH(QStringList, result);
    foreach (const QString &i, result) {
        QVERIFY(completions.contains(i));
    }

    delete doc;
}

}

QTEST_KDEMAIN(Css::ModelTest, GUI)

#include "modeltest.moc"
