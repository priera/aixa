#ifndef ALSAPLAYGROUND_CIRCULARBUFFER_H
#define ALSAPLAYGROUND_CIRCULARBUFFER_H

#include <atomic>
#include <vector>
#include <chrono>
#include <mutex>


template<class T>
class CircularBuffer {
public:
    struct Elem {
        T value;
        time_t pts;
    };

    CircularBuffer(size_t size);

    void insert(const T& v);

    bool read(T &v, time_t &pts);

private:
    std::vector<Elem> buf;

    size_t writePos, readPos;
    std::atomic<size_t> elemsToConsume;
    size_t size;

    std::mutex readPosMutex;
};


#endif //ALSAPLAYGROUND_CIRCULARBUFFER_H
