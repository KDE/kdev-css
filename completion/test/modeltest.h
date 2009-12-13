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
};

}

#endif // MODELTEST_H
