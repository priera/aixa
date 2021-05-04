#ifndef AIXA_SRC_MAINLIB_THREADING_BUFFERSRING_H
#define AIXA_SRC_MAINLIB_THREADING_BUFFERSRING_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>
#include <vector>

template <class T>
class BuffersRing {
public:
    class BuffersWatcher {
    public:
        using Done = std::function<void()>;

        BuffersWatcher(std::shared_ptr<T> buff, Done done) : buff(std::move(buff)), done(std::move(done)) {}

        BuffersWatcher(BuffersWatcher&& other) noexcept {
            buff = other.buff;
            other.buff = nullptr;
            done = other.done;
        }

        virtual ~BuffersWatcher() {
            if (buff) {
                done();
            }
        }

        T& operator*() const noexcept { return *buff; }

        T* operator->() const noexcept { return buff.get(); }

    private:
        std::shared_ptr<T> buff;
        Done done;
    };

    using ElemGenerator = std::function<std::shared_ptr<T>()>;

    BuffersRing(size_t count,
                ElemGenerator generator,
                std::size_t bufferCount,
                std::size_t bufferSize,
                std::chrono::microseconds bufferPeriod) :
        count(count),
        bufferCount(bufferCount), bufferSize(bufferSize), bufferPeriod(bufferPeriod),
        fDoneReading(std::bind(&BuffersRing::bufferConsumed, this)),
        fDoneWriting(std::bind(&BuffersRing::bufferWritten, this)) {
        for (size_t i = 0; i < count; i++) {
            elems.push_back(generator());
        }
    }

    virtual ~BuffersRing() = default;

    std::size_t getBufferCount() const noexcept { return bufferCount; }

    std::size_t getBufferSize() const noexcept { return bufferSize; }

    std::chrono::microseconds getBufferPeriod() const noexcept { return bufferPeriod; }

    BuffersWatcher nextReadBuffer() {
        checkReadHalt();

        return BuffersWatcher(elems[readPos], fDoneReading);
    }

    BuffersWatcher nextWriteBuffer() { return BuffersWatcher(elems[writePos], fDoneWriting); }

    void doneWriting() { setStateTo(State::FLUSHING); }

    bool moreBuffers() { return state != State::FINISHED; }

protected:
    enum class State
    {
        READY,
        RUNNING,
        FLUSHING,
        FINISHED
    };

    void bufferConsumed() {
        readPos = (readPos + 1) % count;

        if (consumed < (count - 1))
            addToConsumed(1);

        if (consumed == (count - 1) && state == State::FLUSHING)
            setStateTo(State::FINISHED);
    }

    void bufferWritten() {
        if (state == State::READY)
            setStateTo(State::RUNNING);

        auto nextWritePos = (writePos + 1) % count;
        if (nextWritePos == readPos)
            writeHalt();

        writePos = nextWritePos;

        if (consumed > 0)
            addToConsumed(-1);
    }

private:
    void checkReadHalt() {
        switch (state) {
            case State::READY: {
                std::mutex m;
                std::unique_lock<std::mutex> lock(m);

                cvState.wait(lock, [this] { return state == State::RUNNING; });
            } break;
            case State::RUNNING:  // Buffer under-run
                if (readPos == writePos)
                    readPos = (readPos - 1) % count;
                break;
            case State::FINISHED:
                throw std::runtime_error("Attempting read of flushed buffer");
            default:
                break;
        }
    }

    void setStateTo(State newState) {
        state = newState;
        cvState.notify_one();
    }

    void addToConsumed(std::size_t incValue) {
        consumed += incValue;
        cvConsumed.notify_one();
    }

    void writeHalt() {
        std::mutex m;
        std::unique_lock<std::mutex> lock(m);

        cvConsumed.wait(lock, [this] { return consumed >= (count / 2); });
    }

    std::size_t count;
    std::size_t bufferCount;
    std::size_t bufferSize;
    std::chrono::microseconds bufferPeriod;

    std::vector<std::shared_ptr<T>> elems;
    std::size_t readPos{0}, writePos{0};
    std::atomic_size_t consumed{0};
    volatile State state{State::READY};
    std::condition_variable cvState, cvConsumed;
    typename BuffersWatcher::Done fDoneReading, fDoneWriting;
};

#endif  // AIXA_SRC_MAINLIB_THREADING_BUFFERSRING_H
