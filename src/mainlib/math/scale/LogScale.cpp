#include "LogScale.h"

namespace aixa::math {
    void LogScale::computeFreeBuckets() {
        std::size_t prevBucket = 0;
        for (std::size_t i = 0; i < size; i++) {
            auto newBucket = getBucketOfPos(i);
            if (newBucket > prevBucket) {
                for (std::size_t j = prevBucket + 1; j < newBucket; j++) {
                    freeBuckets.insert(j);
                }
                prevBucket = newBucket;
            }
        }
    }

    void LogScale::insert(std::size_t pos, double elem) {
        buckets[getBucketOfPos(pos)].push_back(elem);
    }

    std::size_t LogScale::getBucketOfPos(std::size_t pos) const {
        double linPos = linMin + ((static_cast<double>(pos) / size) * (linMax - linMin));
        double logPos = (std::log10(linPos) - logMin) / den;
        return size * logPos;
    }

    void LogScale::prepareData() {
        double prevSample;
        for (std::size_t i = 0; i < size; i++) {
            if (!isFreeBucket(i)) {
                prevSample = computeAvg(buckets[i]);
                buckets[i].clear();
            }
            data[i] = prevSample;
        }
    }

    bool LogScale::isFreeBucket(size_t pos) const {
        return freeBuckets.find(pos) != freeBuckets.end();
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
