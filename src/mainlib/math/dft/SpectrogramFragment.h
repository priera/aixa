#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H

#include <vector>

#include <mainlib/transfer/Emitter.h>
#include <mainlib/transfer/Receiver.h>

namespace aixa::math {
    struct SpectrogramFragment {
        explicit SpectrogramFragment(std::size_t slicesCount) : slices(slicesCount) {}

        std::vector<std::vector<double>> slices;
    };

    using SpectrogramConsumer = Receiver<SpectrogramFragment>;
    using SpectrogramGenerator = Emitter<SpectrogramFragment>;
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H
