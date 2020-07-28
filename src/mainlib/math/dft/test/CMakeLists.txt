add_qtest(ID MatrixFourierTransform_UTest
        SOURCES MatrixFourierTransform_UTest.cpp MatrixFourierTransform_UTest.h
        SRC_FOLDER src/mainlib/math/dft/test)

add_qtest(ID FFTFactory_UTest
        SOURCES FFTFactory_UTest.cpp FFTFactory_UTest.h
        SRC_FOLDER src/mainlib/math/dft/test)

add_qtest(ID CooleyTukeyFFT_UTest
        SOURCES CooleyTukeyFFT_UTest.cpp CooleyTukeyFFT_UTest.h
        SRC_FOLDER src/mainlib/math/dft/test)