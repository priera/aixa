#include "FourierTransformFactory.h"
#include "MatrixDFTFactory.h"
#include "FFTFactory.h"

#include <map>

namespace aixa::math {
    using FactoriesMap = std::map<
            FourierTransformFactory::Implementations,
            std::unique_ptr<FourierTransformFactory> >;

    static FactoriesMap factories;

    void initFactories();

    FourierTransformFactory& getFourierTransformFactory(FourierTransformFactory::Implementations impl) {
        if (factories.empty()) {
            initFactories();
        }

        return *(factories[impl]);
    }

    void initFactories() {
        factories.insert({ FourierTransformFactory::Implementations::MATRIX,
                           std::move(std::make_unique<MatrixDFTFactory>()) });

        factories.insert( {FourierTransformFactory::Implementations::FFT,
                           std::move(std::make_unique<FFTFactory>()) });

    }

}
