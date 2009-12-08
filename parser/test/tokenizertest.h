
#ifndef CSS_TOKENIZERTEST_H
#define CSS_TOKENIZERTEST_H

#include <QObject>

namespace Css
{
class TestTokenizer : public QObject
{
    Q_OBJECT

public:
    TestTokenizer();

private Q_SLOTS:
    void firstTest();

};

}

#endif
