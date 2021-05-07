#ifndef AIXA_SRC_MAINLIB_MATH_COMPLEX_H
#define AIXA_SRC_MAINLIB_MATH_COMPLEX_H

#include <ostream>
#include <iomanip>
#include "ComplexExponential.h"

namespace aixa::math {
    class Complex;
    Complex operator*(const Complex &lhs, const Complex &rhs);
    Complex operator+(const Complex &lhs, const Complex &rhs);
    Complex operator-(const Complex &lhs, const Complex &rhs);

    class Complex {
    public:
        Complex() : x(0), y(0) {}
        Complex(double x, double y = 0) : x(x), y(y) {}

        double re() const { return x; }
        double im() const { return y; }
        double magnitude() const {
            auto cRe = re();
            auto cIm = im();

            return std::sqrt((cRe * cRe) + (cIm * cIm));
        }

        //conjugate
        Complex operator~() const {
            return Complex(x, -y);
        }

        Complex& operator+=(const Complex& other) {
            (*this) = (*this) + other;
            return *this;
        }

        Complex& operator-=(const Complex& other) {
            (*this) = (*this) - other;
            return *this;
        }

        Complex& operator*=(const Complex& other) {
            (*this) = (*this) * other;
            return *this;
        }

        operator ComplexExponential() const {
            auto cRe = re();
            auto cIm = im();

            double arg = std::atan2(cIm, cRe);
            double mag = std::sqrt((cRe * cRe) + (cIm * cIm));

            return ComplexExponential(arg, mag);
        }

    private:
        double x;
        double y;
    };

    inline std::ostream& operator<<(std::ostream& os, const Complex& obj) {
        os << std::setprecision(4) << std::fixed <<  "[" << obj.re() << ", " << obj.im() << "i] ";
        return os;
    }

    inline Complex operator*(const Complex &lhs, const Complex &rhs) {
        auto x = lhs.re();
        auto y = lhs.im();
        auto u = rhs.re();
        auto v = rhs.im();

        auto newX = (x * u) - (y * v);
        auto newY = (x * v) + (y * u);

        return Complex(newX, newY);
    }

    inline Complex operator+(const Complex &lhs, const Complex &rhs) {
        auto x = lhs.re();
        auto y = lhs.im();
        auto u = rhs.re();
        auto v = rhs.im();

        auto newX = x + u;
        auto newY = y + v;

        return Complex(newX, newY);
    }

    inline Complex operator-(const Complex &lhs, const Complex &rhs) {
        auto x = lhs.re();
        auto y = lhs.im();
        auto u = rhs.re();
        auto v = rhs.im();

        auto newX = x - u;
        auto newY = y - v;

        return Complex(newX, newY);
    }

    inline bool operator==(const Complex &lhs, const Complex &rhs) {
        auto realEq = std::abs(lhs.re() - rhs.re()) < 1e-4;
        auto imEq = std::abs(lhs.im() - rhs.im()) < 1e-4;

        return realEq && imEq;
    }

    inline bool operator!=(const Complex &lhs, const Complex &rhs) {
        return !(lhs == rhs);
    }
}

#endif //AIXA_SRC_MAINLIB_MATH_COMPLEX_H
