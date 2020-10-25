#ifndef AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H
#define AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H

#include <vector>

#include <mainlib/observer/Observer.h>
#include <mainlib/observer/Observable.h>

namespace aixa::math {
    struct SpectrogramFragment {
        std::vector<std::vector<double>> slices;
    };

    using SpectrogramListener = Observer<SpectrogramFragment>;
    using SpectrogramProvider = Observable<SpectrogramFragment>;
}

#endif //AIXA_SRC_MAINLIB_MATH_DFT_SPECTROGRAMFRAGMENT_H
