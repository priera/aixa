#ifndef AIXA_SRC_MAINLIB_MATH_SCALE_LOGSCALE_H
#define AIXA_SRC_MAINLIB_MATH_SCALE_LOGSCALE_H

#include "Scale.h"

#include <cmath>

namespace aixa::math {
    class LogScale : public Scale {
    public:
        LogScale(double linMin, double linMax, std::size_t positions) :
            Scale(positions),
            linMin(linMin),
            linMax(linMax),
            logMin(std::log10(linMin)),
            logMax(std::log10(linMax)),
            den(logMax - logMin) {
            buckets.resize(positions);
        }

        void insert(std::size_t pos, double elem) override;

    protected:
        void prepareData() override;

    private:
        double computeAvg(const std::vector<double> &bucket);

        double linMin;
        double linMax;
        double logMin;
        double logMax;
        double den;

        std::vector<std::vector<double>> buckets;
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_SCALE_LOGSCALE_H
