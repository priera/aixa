#ifndef AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H
#define AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H

#include <cassert>

#include "Vector.h"

namespace aixa::math {

template <typename T, class TypeAxioms>
class ConstVectorProxy : public Vector<T, TypeAxioms> {
public:
    ConstVectorProxy(const std::vector<T> &parentContent, std::size_t offset, std::size_t count) :
        Vector<T, TypeAxioms>(count, std::false_type()), parentContent(parentContent), offset(offset),
        count(count){};

    const T &operator[](std::size_t n) const override {
        assert(n < count);

        return parentContent[offset + n];
    }

    Vector<T, TypeAxioms> detach() const {
        auto newContent =
            std::vector<T>(parentContent.begin() + offset, parentContent.begin() + offset + count);
        return Vector<T, TypeAxioms>(newContent.begin(), newContent.size());
    }

private:
    const std::vector<T> &parentContent;
    std::size_t offset;
    std::size_t count;
};

}  // namespace aixa::math

#endif  // AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H
