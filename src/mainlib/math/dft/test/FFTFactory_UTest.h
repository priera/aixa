#ifndef AIXA_SRC_MAINLIB_MATH_DFT_TEST_FFTFACTORY_UTEST_H
#define AIXA_SRC_MAINLIB_MATH_DFT_TEST_FFTFACTORY_UTEST_H

#include <QtCore/QObject>

namespace aixa::math {
    class FFTFactory_UTest : public QObject {
        Q_OBJECT

    private slots:
        void test_WeightsGeneration();
    };
}



#endif //AIXA_SRC_MAINLIB_MATH_DFT_TEST_FFTFACTORY_UTEST_H
