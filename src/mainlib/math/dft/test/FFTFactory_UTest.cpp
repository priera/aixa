#include "FFTFactory_UTest.h"

#include <iostream>

#include <QtTest/QTest>

#include <mainlib/math/dft/FFTFactory.h>

namespace aixa::math {
    void FFTFactory_UTest::test_WeightsGeneration() {
        auto factory = new FFTFactory();

        auto W4 = factory->computeWeights(4);
        auto W8 = factory->computeWeights(8);
        auto W16 = factory->computeWeights(16);
        auto W32 = factory->computeWeights(32);

        for (size_t i = 0; i < W4.size(); i++) {
            QVERIFY(W4[i] == W32[8 * i]);
        }

        for (size_t i = 0; i < W8.size(); i++) {
            QVERIFY(W8[i] == W32[4 * i]);
        }

        for (size_t i = 0; i < W16.size(); i++) {
            QVERIFY(W16[i] == W32[2 * i]);
        }
    }

}

QTEST_MAIN(aixa::math::FFTFactory_UTest)