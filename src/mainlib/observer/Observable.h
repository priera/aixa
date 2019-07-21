#ifndef ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
#define ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVABLE_H

#include <vector>

#include "mainlib/observer/Observer.h"

template <class Entity>
class Observable {
public:
    virtual ~Observable() = default;

    void addObserver(Observer<Entity> &observer) {
        observers.push_back(&observer);
    }

    void removeObserver(Observer<Entity> *observer) {
        auto it = observers.find(observer);
        if (it == observers.end())
            return;

        observers.erase(it);
    }

    void notifyObservers(const Entity &newValue) {
        for (auto observer: observers) {
            observer->notifyNewValue(newValue);
        }
    }

private:
    std::vector<Observer<Entity> *> observers;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
