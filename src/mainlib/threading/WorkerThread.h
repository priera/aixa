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
        stateCv.notify_one();
    }

    void stopAndDetach() {
        state = State::STOPPING;
        stateCv.notify_one();
        t.detach();
    }

private:
    enum class State : unsigned int {
        PREPARED = 0,
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

        stateCv.wait(lk, [this]() { return state != State::PREPARED; });
    }

    WorkingClass &worker;

    State state{State::PREPARED};
    std::condition_variable stateCv;
    std::thread t;
};


#endif //AIXA_SRC_MAINLIB_THREADING_WORKERTHREAD_H
