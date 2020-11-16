#include "LinearScale.h"

namespace aixa::math {
    void LinearScale::insert(std::size_t pos, double elem) {
        data[pos] = elem;
    }
}
