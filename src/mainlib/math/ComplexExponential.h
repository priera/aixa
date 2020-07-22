#ifndef AIXA_SRC_MAINLIB_MATH_COMPLEXEXPONENTIAL_H
#define AIXA_SRC_MAINLIB_MATH_COMPLEXEXPONENTIAL_H

#include <cmath>
#include <sstream>

namespace aixa::math {
    class ComplexExponential {
    public:
        ComplexExponential() : arg(0), mag(0) {}
        ComplexExponential(double arg, double mag = 1) : arg(arg), mag(mag) {}

        double re() const { return mag * std::cos(arg); }
        double im() const { return mag * std::sin(arg); }
        double phase() const { return arg; }
        double magnitude() const { return mag; }

        ComplexExponential& operator=(const ComplexExponential& rhs) = default;

        //conjugate
        ComplexExponential operator~() const {
            return ComplexExponential(arg * -1, mag);
        }

        ComplexExponential& operator+=(const ComplexExponential& other) {
            auto cRe = re() + other.re();
            auto cIm = im() + other.im();

            arg = std::atan2(cIm, cRe);
            mag = std::sqrt((cRe * cRe) + (cIm * cIm));

            return *this;
        }

    private:
        double arg;
        double mag;
    };

    static std::ostream& operator<<(std::ostream& os, const ComplexExponential& obj) {
        os << "[" << obj.magnitude() << ", " << obj.phase() << "] ";
        return os;
    }

    static ComplexExponential operator*(const ComplexExponential &lhs, const ComplexExponential &rhs) {
        return ComplexExponential(lhs.phase() + rhs.phase(), lhs.magnitude() * rhs.magnitude());
    }

}



#endif //AIXA_SRC_MAINLIB_MATH_COMPLEXEXPONENTIAL_H
