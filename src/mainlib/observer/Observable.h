#ifndef AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
#define AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H

#include <vector>

#include "mainlib/observer/Observer.h"

template <class Entity>
class Observable {
public:
    using MyObserver = Observer<Entity>;

    virtual ~Observable() = default;

    void addObserver(std::shared_ptr<MyObserver> observer) {
        if (!observer) return;
        observers.emplace_back(observer);
    }

    void notifyObservers(const Entity &newValue) {
        for (auto& observerWeak: observers) {
            auto observer = observerWeak.lock();
            if (observer)
                observer->notifyNewValue(newValue);
        }
    }

private:
    std::vector<std::weak_ptr<MyObserver>> observers;
};


#endif //AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
