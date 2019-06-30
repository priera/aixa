#include "mainlib/buffers/CircularBuffer.h"

#include <iostream>

template<class T>
CircularBuffer<T>::CircularBuffer(size_t size) :
    readPos(0),
    writePos(0),
    elemsToConsume(0),
    size(size) {
    buf.resize(size);
}

template<class T>
void CircularBuffer<T>::insert(const T& v) {
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
    el.v = v;
    el.pts = pts;

    std::cout << pts << std::endl;

    writePos = (writePos + 1) % size;
}

template<class T>
bool CircularBuffer<T>::read(T &v, time_t &pts) {
    if (elemsToConsume == 0) return false;
    elemsToConsume--;

    size_t currentReadPos;

    {
        std::lock_guard<std::mutex> l(readPosMutex);
        currentReadPos = readPos;
        readPos = (readPos + 1) % size;
    }

    auto & el = buf[currentReadPos];
    v = el.v;
    pts = el.pts;

    return true;
}
