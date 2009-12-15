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
    KTextEditor::Editor *editor = KTextEditor::EditorChooser::editor();
    KTextEditor::Document* doc = editor->createDocument(0);
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


}

QTEST_KDEMAIN(Css::ModelTest, GUI)

#include "modeltest.moc"
