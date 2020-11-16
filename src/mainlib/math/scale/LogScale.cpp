#include "LogScale.h"

namespace aixa::math {
    void LogScale::insert(std::size_t pos, double elem) {
        double linPos = linMin + ((static_cast<double>(pos) / size) * (linMax - linMin));
        double logPos = (std::log10(linPos) - logMin) / den;
        std::size_t bucket = size * logPos;
        buckets[bucket].push_back(elem);
    }

    void LogScale::prepareData() {
        for (std::size_t i = 0; i < size; i++) {
            data[i] = computeAvg(buckets[i]);
            buckets[i].clear();
        }
    }

    double LogScale::computeAvg(const std::vector<double> &bucket) {
        if (bucket.empty())
            return 0;

        double agg = 0;
        for (auto elem: bucket) {
            agg += elem;
        }

        return agg / bucket.size();
    }
}
