#ifndef AIXA_SRC_MAINLIB_MATH_DFT_TEST_COOLEYTUKEYFFT_UTEST_H
#define AIXA_SRC_MAINLIB_MATH_DFT_TEST_COOLEYTUKEYFFT_UTEST_H

#include <platform/aixa_export.h>
#include <QtCore/QObject>

#include <mainlib/math/SineGenerator.h>

namespace aixa::math {
    class CooleyTukeyFFT_UTest : public QObject {
    Q_OBJECT

    private slots:
        void test_FFT_data();
        void test_FFT();
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_DFT_TEST_COOLEYTUKEYFFT_UTEST_H
