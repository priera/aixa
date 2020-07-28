#ifndef AIXA_SRC_MAINLIB_MATH_TEST_MATRIXFOURIERTRANSFORM_UTEST_H
#define AIXA_SRC_MAINLIB_MATH_TEST_MATRIXFOURIERTRANSFORM_UTEST_H

#include <QtCore/QObject>

#include <filesystem>

namespace aixa::math {

    //Not so much a test class, but rather some utilities to ease development
    class MatrixFourierTransform_UTest : public QObject {
    Q_OBJECT

    private slots:
        void initTestCase();

        void test_printBasisVectors();
        void test_simpleDFT();
        void test_simpleDFT2();
        void test_simpleDFT3();
        void test_DFT_A4_1();
        void test_DFT_A4_2();

    private:
        std::filesystem::path storeAt;
    };
}

#endif //AIXA_SRC_MAINLIB_MATH_TEST_MATRIXFOURIERTRANSFORM_UTEST_H
