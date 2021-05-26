#include "Matrix2.h"

struct IntegerInvertible {
    using type = int;
    constexpr type operator()() noexcept {
        return -1;
    }
};

int main2(int argc, char **argv) {
    auto m2 = Matrix2<IntegerInvertible>(2, 2);
    m2.content()[1] = -1;
    m2.content()[2] = 2;
    auto m3 = Matrix2<IntegerInvertible>(2, 2);
    m3.content()[1] = -3;
    m3.content()[2] = 7;
    std::cout << (m2 * m3) << std::endl;

    return 0;
}