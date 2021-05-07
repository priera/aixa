#ifndef AIXA_SRC_MAINLIB_MATH_SCALE_LINEARSCALE_H
#define AIXA_SRC_MAINLIB_MATH_SCALE_LINEARSCALE_H

#include "Scale.h"

namespace aixa::math {
    class LinearScale : public Scale {
    public:
        explicit LinearScale(std::size_t size) : Scale(size) {}

        void insert(std::size_t pos, double elem) override;
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_SCALE_LINEARSCALE_H
