#ifndef AIXA_SRC_MAINLIB_MATH_SCALE_SCALE_H
#define AIXA_SRC_MAINLIB_MATH_SCALE_SCALE_H

#include <vector>

namespace aixa::math {
    class Scale {
    public:
        virtual ~Scale() noexcept = default;

        virtual void insert(std::size_t pos, double elem) = 0;

        void moveDataTo(std::vector<double>& destiny) {
            prepareData();

            data.swap(destiny);
            data.resize(size);
        }

    protected:
        Scale(std::size_t size) :
            size(size),
            data(size) {}

        virtual void prepareData();

        std::size_t size;
        std::vector<double> data;
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_SCALE_SCALE_H
