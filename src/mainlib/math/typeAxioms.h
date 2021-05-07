#ifndef AIXA_SRC_MAINLIB_MATH_TYPEAXIOMS_H
#define AIXA_SRC_MAINLIB_MATH_TYPEAXIOMS_H

#include <cmath>
#include "Complex.h"

namespace aixa::math {
    class DoubleTypeAxioms {
    public:
        bool isZero(const double& elem) {
            return std::abs(elem) < 1e-4;
        }

        double inverter() { return -1.0; }
    };

    class ComplexTypeAxioms {
    public:
        bool isZero(const Complex& elem) {
            auto absRe = std::abs(elem.re());
            auto absIm = std::abs(elem.im());

            return (absRe < 1e-4 && absIm < 1e-4);
        }

        Complex inverter() { return Complex(-1.0, -1.0); }
    };
}


#endif //AIXA_SRC_MAINLIB_MATH_TYPEAXIOMS_H
