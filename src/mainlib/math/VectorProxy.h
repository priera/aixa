#ifndef AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H
#define AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H

#include <cassert>

#include "Vector.h"

namespace aixa::math {

    template<typename T, class TypeAxioms>
    class VectorProxy : public Vector<T, TypeAxioms> {
    public:
        VectorProxy(Vector<T, TypeAxioms> &parent,
                    std::vector<T> &parentContent,
                    std::size_t offset,
                    std::size_t count) :
                Vector<T, TypeAxioms>(count, std::false_type()),
                parent(parent),
                parentContent(parentContent),
                offset(offset),
                count(count) { };

        const T &operator[](std::size_t n) const override {
            assert(n < count);

            return parentContent[offset + n];
        }

        T &operator[](std::size_t n) override {
            assert(n < count);

            return parentContent[offset + n];
        }

    private:
        Vector<T, TypeAxioms> &parent;
        std::vector<T> &parentContent;
        std::size_t offset;
        std::size_t count;
    };

}

#endif // AIXA_SRC_MAINLIB_DSP_VECTORPROXY_H
