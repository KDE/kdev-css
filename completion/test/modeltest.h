#ifndef MODELTEST_H
#define MODELTEST_H

#include <QObject>
namespace Css
{
int debugArea();

class ModelTest : public QObject
{
    Q_OBJECT
public:
    ModelTest();

private slots:
    void testCompletionRange();
    void testCompletionRangeSecondLine();
    void testCompletionElement();
    void testCompletionField();
    void testCompletionElementSecondLine();
};

}

#endif // MODELTEST_H
