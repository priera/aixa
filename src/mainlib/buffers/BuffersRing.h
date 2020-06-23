#ifndef AIXA_SRC_MAINLIB_BUFFERS_BUFFERSRING_H
#define AIXA_SRC_MAINLIB_BUFFERS_BUFFERSRING_H

#include <utility>
#include <vector>
#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>

template<class T>
class BuffersRing {
public:
    class BuffersWatcher {
    public:
        using Done = std::function<void()>;

        BuffersWatcher(std::shared_ptr<T> buff, Done done) :
                buff(std::move(buff)),
                done(std::move(done))
        { }

        BuffersWatcher(BuffersWatcher &&other) noexcept {
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

    BuffersRing(size_t count, ElemGenerator generator)
    : count(count),
    fDoneReading(std::bind(&BuffersRing::bufferConsumed, this)),
    fDoneWriting(std::bind(&BuffersRing::bufferWritten, this)) {
        for (size_t i = 0; i < count; i++) {
            elems.push_back(generator());
        }
    }

    virtual ~BuffersRing() = default;

    BuffersWatcher nextReadBuffer() {
        if (readPos == writePos) {
            readHalt();
        }

        BuffersWatcher ret(elems[readPos], fDoneReading);

        return std::move(ret);
    }

    BuffersWatcher nextWriteBuffer() {
        if (state == State::RUNNING && writePos == readPos) {
            writeHalt();
        }

        BuffersWatcher ret(elems[writePos], fDoneWriting);

        return std::move(ret);
    }

    void doneWriting() {
        state = State::FLUSHING;
    }

    bool moreBuffers() {
        return state != State::FINISHED;
    }

protected:
    enum class State {
        READY,
        RUNNING,
        FLUSHING,
        FINISHED
    };

    void bufferConsumed() {
        readPos = (readPos + 1) % count;
        {
            std::lock_guard<std::mutex> lock(consumedMutex);

            if (consumed < (count - 1)) consumed++;
        }
    }

    void bufferWritten() {
        if (state == State::READY) state = State::RUNNING;

        writePos = (writePos + 1) % count;

        {
            std::lock_guard<std::mutex> lock(consumedMutex);

            if (consumed > 0) consumed--;
        }
    }

private:
    void readHalt() {
        switch (state) {
            case State::READY:
                {
                    std::mutex m;
                    std::unique_lock<std::mutex> lock(m);
                    std::condition_variable cv;

                    cv.wait(lock, [this] { return state == State::RUNNING; });
                }
                break;
            case State::RUNNING: //Buffer under-run
                readPos = (readPos - 1) % count;
                break;
            case State::FLUSHING:
                state = State::FINISHED;
                break;
            case State::FINISHED:
                throw std::runtime_error("Attempting read of flushed buffer");
        }
    }

    void writeHalt() {
        std::mutex m;
        std::unique_lock<std::mutex> lock(m);
        std::condition_variable cv;

        cv.wait(lock, [this] { return consumed >= (count / 2); });
    }

    std::vector<std::shared_ptr<T>> elems;
    size_t count, readPos{0}, writePos{0}, consumed{0};
    std::mutex consumedMutex;
    State state{State::READY};
    typename BuffersWatcher::Done fDoneReading, fDoneWriting;

};


#endif //AIXA_SRC_MAINLIB_BUFFERS_BUFFERSRING_H
