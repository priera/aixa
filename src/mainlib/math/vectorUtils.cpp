#include "vectorUtils.h"

namespace aixa::math {
    void printVector(const std::vector<Complex> & v) {
        for (auto &c: v) {
            std::cout << c << " ";
        }

        std::cout << std::endl;
    }

    void storeVector(const std::string& name, const std::filesystem::path& dir, const std::vector<Complex>& v) {
        auto fileName = dir / (name + std::string(".txt"));
        std::ofstream f(fileName);

        for (auto &c: v) {
            f << c << " ";
        }

        f << std::endl;
        f.close();
    }
}