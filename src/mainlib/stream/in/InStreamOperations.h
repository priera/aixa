#ifndef AIXA_SRC_MAINLIB_STREAM_IN_INSTREAMOPERATIONS_H
#define AIXA_SRC_MAINLIB_STREAM_IN_INSTREAMOPERATIONS_H

#include <fstream>

class InStreamOperations {
public:
    explicit InStreamOperations(const std::string &path) {
        in.open(path.c_str(), std::ios_base::binary);
        if (!in.is_open()) {
            throw std::runtime_error("Failed to open stream");
        }
    }

    std::streamsize readNBytes(char *ptr, long n) { return in.read(ptr, n).gcount(); }

    void skipNBytes(long n) { in.seekg(in.tellg() + n); }

    bool ended() const {
        bool fileOk = static_cast<bool>(in);
        return !fileOk || in.eof();
    }

private:
    std::ifstream in;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_IN_INSTREAMOPERATIONS_H
