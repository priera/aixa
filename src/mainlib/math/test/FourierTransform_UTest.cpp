#include "FourierTransform_UTest.h"

#include <iostream>
#include <fstream>
#include <cmath>

#include <QtTest/QTest>
#include <QtCore/QtGlobal>

#include <mainlib/math/FourierTransform.h>

namespace aixa::math {

    static void printVector(const std::vector<Complex> & v) {
        for (auto &c: v) {
            std::cout << c << " ";
        }

        std::cout << std::endl;
    }

    template<typename T>
    static void printVector(const Vector<T>& v) {
        for (size_t i = 0; i < v.size(); i++) {
            std::cout << v(i) << " ";
        }

        std::cout << std::endl;
    }

    template<typename T>
    static void storeVector(const std::string &name, const std::filesystem::path& dir, const Vector<T>& v) {
        auto fileName = dir / (name + std::string(".txt"));
        std::ofstream f(fileName);

        for (size_t i = 0; i < v.size(); i++) {
            f << v(i) << " ";
        }

        f << std::endl;
        f.close();
    }

    static void storeVector(const std::string& name, const std::filesystem::path& dir, const std::vector<Complex>& v) {
        auto fileName = dir / (name + std::string(".txt"));
        std::ofstream f(fileName);

        for (auto &c: v) {
            f << c << " ";
        }

        f << std::endl;
        f.close();
    }

    static DoubleVector signalOfFreq(size_t dimensionality, double om, double amp = 1, double phase = 0) {
        DoubleVector ret(dimensionality);
        for (size_t i = 0; i < dimensionality; i++) {
            double freq = (om * i) + phase;
            ret(i) = amp * std::cos(freq);
        }

        return ret;
    }

    void FourierTransform_UTest::initTestCase() {
        storeAt = std::filesystem::path(qgetenv("storeAt").toStdString());
    }

    void FourierTransform_UTest::test_printBasisVectors() {
        auto transform = FourierTransform::prepare(400);
        auto basis = transform.basis;

        auto v0 = basis.vector(0);
        auto v1 = basis.vector(1);
        auto v2 = basis.vector(2);
        auto v3 = basis.vector(3);
        auto v4 = basis.vector(4);
        auto v16 = basis.vector(16);
        auto v20 = basis.vector(20);
        auto v31 = basis.vector(31);
        auto v32 = basis.vector(32);
        auto v60 = basis.vector(60);

        storeVector("v0", storeAt, v0);
        storeVector("v1", storeAt, v1);
        storeVector("v2", storeAt, v2);
        storeVector("v3", storeAt, v3);
        storeVector("v4", storeAt, v4);
        storeVector("v16", storeAt, v16);
        storeVector("v20", storeAt, v20);
        storeVector("v31", storeAt, v31);
        storeVector("v60", storeAt, v60);
    }

    void FourierTransform_UTest::test_simpleDFT() {
        DoubleVector signal = signalOfFreq(64, 2 * M_PI / 16, 3);

        auto transform = FourierTransform::prepare(64);
        auto &result = transform.applyTo(signal);

        storeVector("dft1", storeAt, result);
    }

    void FourierTransform_UTest::test_simpleDFT2() {
        DoubleVector signal = signalOfFreq(64, 2 * M_PI / 16, 3, M_PI / 3);

        auto transform = FourierTransform::prepare(64);
        auto &result = transform.applyTo(signal);

        //Should be: 0.0981748 (rad)
        //689.062 (Hz)
        std::cout << transform.baseDiscreteFreq() << std::endl;
        std::cout << transform.baseContinuousFreq(1 / 44100.0) << std::endl;

        storeVector("dft2", storeAt, result);
    }

    void FourierTransform_UTest::test_simpleDFT3() {
        DoubleVector signal = signalOfFreq(64, 2 * M_PI / 10, 3);

        auto transform = FourierTransform::prepare(64);
        auto &result = transform.applyTo(signal);

        storeVector("dft3", storeAt, result);
    }

    void FourierTransform_UTest::test_DFT_A4_1() {
        double realFreq = 440.0; //A4
        size_t n = 128;
        double ts = 1.0 / 44100;
        double discreteFreq = realFreq * 2 * M_PI * ts;

        DoubleVector signal = signalOfFreq(n, discreteFreq);

        auto transform = FourierTransform::prepare(n);
        auto &result = transform.applyTo(signal);

        storeVector("dft_A4_1", storeAt, result);
    }

    void FourierTransform_UTest::test_DFT_A4_2() {
        double realFreq = 440.0; //A4
        size_t n = 400;
        double ts = 1.0 / 44100;
        double discreteFreq = realFreq * 2 * M_PI * ts;

        DoubleVector signal = signalOfFreq(n, discreteFreq);

        auto transform = FourierTransform::prepare(n);
        auto &result = transform.applyTo(signal);

        storeVector("dft_A4_2", storeAt, result);
    }
}

QTEST_MAIN(aixa::math::FourierTransform_UTest)
