#include "CooleyTukeyFFT_UTest.h"

#include <QtTest/QTest>
#include <mainlib/math/dft/FourierTransformFactory.h>

namespace aixa::math {
    void CooleyTukeyFFT_UTest::test_FFT_data() {
        QTest::addColumn<unsigned int>("N");

        QTest::newRow("4")   <<   4u;
        QTest::newRow("8")   <<   8u;
        QTest::newRow("16")  <<  16u;
        QTest::newRow("32")  <<  32u;
        QTest::newRow("128") << 128u;
        QTest::newRow("256") << 256u;
    }

    void CooleyTukeyFFT_UTest::test_FFT() {
        QFETCH(unsigned int, N);

        auto sineGenerator = SineGenerator(N, 1.0/44100, 440);
        auto& v = sineGenerator.nextSignal();

        auto matrixTransformP = getFourierTransformFactory(FourierTransformFactory::Implementations::MATRIX).build(N);
        auto fftTransformP = getFourierTransformFactory(FourierTransformFactory::Implementations::FFT).build(N);

        auto matrixTransform = std::unique_ptr<FourierTransform>(matrixTransformP);
        auto fftTransform = std::unique_ptr<FourierTransform>(fftTransformP);

        auto& baseResult = matrixTransform->applyTo(v);
        auto& fftResult = fftTransform->applyTo(v);

        QVERIFY(baseResult == fftResult);
    }
}

QTEST_MAIN(aixa::math::CooleyTukeyFFT_UTest)