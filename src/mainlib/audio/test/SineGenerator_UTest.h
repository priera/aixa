#ifndef ALSAPLAYGROUND_SINEGENERATOR_UTEST_H
#define ALSAPLAYGROUND_SINEGENERATOR_UTEST_H

#include <QTest>
#include <QObject>

class SineGenerator_UTest : public QObject {
    Q_OBJECT

private slots:
    void test_f1();
};

#endif //ALSAPLAYGROUND_SINEGENERATOR_UTEST_H