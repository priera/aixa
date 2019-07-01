#ifndef ALSAPLAYGROUND_CIRCULARBUFFER_H
#define ALSAPLAYGROUND_CIRCULARBUFFER_H

#include <atomic>
#include <vector>
#include <chrono>
#include <mutex>

#include <iostream>

template<class T>
class CircularBuffer {
public:
    struct Elem {
        T value;
        time_t pts;
    };

    static double timeDiffInMs(const time_t &pts1, const time_t &pts2) {
        return (pts1 - pts2) * CLOCK_PERIOD * 1000;
    }

    CircularBuffer(size_t size)  :
            readPos(0),
            writePos(0),
            elemsToConsume(0),
            size(size) {
        buf.resize(size);
    }

    void insert(const T& v) {
        {
            std::lock_guard<std::mutex> l(readPosMutex);

            if (elemsToConsume + 1 == size) {
                std::cout << "Circular buffer overwrite" << std::endl;
                elemsToConsume = 1;
                readPos = ((readPos) ? readPos : size ) -1;
            } else {
                elemsToConsume++;
            }
        }


        std::chrono::steady_clock::time_point t = std::chrono::steady_clock::now();
        auto pts = t.time_since_epoch().count();

        auto & el = buf[writePos];
        el.value = v;
        el.pts = pts;

        writePos = (writePos + 1) % size;
    }

    bool read(T &v, time_t &pts) {
        if (elemsToConsume == 0) return false;
        elemsToConsume--;

        size_t currentReadPos;

        {
            std::lock_guard<std::mutex> l(readPosMutex);
            currentReadPos = readPos;
            readPos = (readPos + 1) % size;
        }

        auto & el = buf[currentReadPos];
        v = el.value;
        pts = el.pts;

        return true;
    }

private:
    static constexpr double CLOCK_PERIOD = ((double)std::chrono::steady_clock::period::num ) /std::chrono::steady_clock::period::den;

    std::vector<Elem> buf;

    size_t writePos, readPos;
    std::atomic<size_t> elemsToConsume;
    size_t size;

    std::mutex readPosMutex;
};


#endif //ALSAPLAYGROUND_CIRCULARBUFFER_H
