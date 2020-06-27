#ifndef AIXA_SRC_MAINLIB_THREADING_WORKERTHREAD_H
#define AIXA_SRC_MAINLIB_THREADING_WORKERTHREAD_H


#include <thread>
#include <mutex>
#include <condition_variable>

template<class WorkingClass>
class WorkerThread {
public:
    explicit WorkerThread(WorkingClass &worker) :
            worker(worker) {
        auto entryPoint = [this]() {
            workerLoop();
        };

        t = std::thread(entryPoint);
    }

    virtual ~WorkerThread() {
        if (t.joinable()) {
            state = State::STOPPING;
            t.join();
        }
    }

    void start() {
        state = State::RUNNING;
    }

    void stopAndDetach() {
        state = State::STOPPING;
        t.detach();
    }

private:
    enum class State {
        PREPARED,
        RUNNING,
        STOPPING
    };

    void workerLoop() {
        waitUntilStarted();

        if (state == State::STOPPING)
            return;

        while (state == State::RUNNING) {
            worker.exec();
        }
    }

    void waitUntilStarted() {
        std::mutex m;
        std::unique_lock<std::mutex> lk(m);
        std::condition_variable cv;

        cv.wait(lk, [this]() { return state != State::PREPARED; });
    }

    WorkingClass &worker;

    //The kind of operations done on state do not involve temporaries. So no need of atomics
    State state{State::PREPARED};
    std::thread t;
};


#endif //AIXA_SRC_MAINLIB_THREADING_WORKERTHREAD_H
