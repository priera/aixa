#ifndef AIXA_SRC_MAINLIB_MATH_COMPLEX_H
#define AIXA_SRC_MAINLIB_MATH_COMPLEX_H

#include <ostream>
#include <iomanip>

namespace aixa::math {
    class Complex {
    public:
        Complex() : x(0), y(0) {}
        Complex(double x, double y = 0) : x(x), y(y) {}

        double re() const { return x; }
        double im() const { return y; }

        //conjugate
        Complex operator~() const {
            return Complex(x, -y);
        }

        Complex& operator+=(const Complex& other) {
            auto u = other.re();
            auto v = other.im();

            x = x + u;
            y = y + v;

            return *this;
        }

    private:
        double x;
        double y;
    };

    static std::ostream& operator<<(std::ostream& os, const Complex& obj) {
        os << std::setprecision(4) << std::fixed <<  "[" << obj.re() << ", " << obj.im() << "i] ";
        return os;
    }

    static Complex operator*(const Complex &lhs, const Complex &rhs) {
        auto x = lhs.re();
        auto y = lhs.im();
        auto u = rhs.re();
        auto v = rhs.im();

        auto newX = (x * u) - (y * v);
        auto newY = (x * v) - (y * u);

        return Complex(newX, newY);
    }
}

#endif //AIXA_SRC_MAINLIB_MATH_COMPLEX_H
