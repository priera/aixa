#ifndef AIXA_SRC_MAINLIB_MATH_VECTORUTILS_H
#define AIXA_SRC_MAINLIB_MATH_VECTORUTILS_H

#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "Complex.h"
#include "Vector.h"

#include <platform/aixa_export.h>

namespace aixa::math {
    void LIB_EXPORT printVector(const std::vector<Complex> & v);

    template<typename T, class ZeroComparer>
    void printVector(const Vector<T, ZeroComparer>& v) {
        for (size_t i = 0; i < v.size(); i++) {
            std::cout << v(i) << " ";
        }

        std::cout << std::endl;
    }

    template<typename T, class ZeroComparer>
    void storeVector(const std::string &name,
                     const std::filesystem::path& dir,
                     const Vector<T, ZeroComparer>& v) {
        auto fileName = dir / (name + std::string(".txt"));
        std::ofstream f(fileName);

        for (size_t i = 0; i < v.size(); i++) {
            f << v[i] << " ";
        }

        f << std::endl;
        f.close();
    }

    void LIB_EXPORT storeVector(const std::string& name, const std::filesystem::path& dir, const std::vector<Complex>& v);
}

#endif //AIXA_SRC_MAINLIB_MATH_VECTORUTILS_H
