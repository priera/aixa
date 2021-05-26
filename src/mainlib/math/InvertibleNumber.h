#ifndef AIXA_AIXA_SRC_MAINLIB_MATH_INVERTIBLENUMBER_H
#define AIXA_AIXA_SRC_MAINLIB_MATH_INVERTIBLENUMBER_H

#include <concepts>

template<typename Inv>
concept InvertibleNumber =
    requires(Inv inv) {
        typename Inv::type;
        { std::is_arithmetic_v<typename Inv::type> };
        { inv() } -> std::convertible_to<typename Inv::type>;
};

#endif //AIXA_AIXA_SRC_MAINLIB_MATH_INVERTIBLENUMBER_H
