#ifndef AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
#define AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H

#include <memory>
#include <map>
#include <vector>

#include "Observer.h"

template <class Entity>
class Observable {
public:
    using MyObserver = Observer<Entity>;

    virtual ~Observable() noexcept = default;

    void addObserver(std::shared_ptr<MyObserver> observer) {
        if (!observer) return;
        observers.emplace(std::make_pair(++nextId, observer));
    }

    void notifyObservers(const Entity& newValue) {
        std::vector<unsigned int> toRemove;
        for (auto& observerWeak: observers) {
            auto observer = observerWeak.second.lock();
            if (observer) {
                observer->notifyNewValue(newValue);
            } else {
                toRemove.push_back(observerWeak.first);
            }
        }

        for (auto goneObserver: toRemove) {
            observers.erase(goneObserver);
        }
    }

private:
    unsigned int nextId{0};
    std::map<unsigned int, std::weak_ptr<MyObserver>> observers;
};


#endif //AIXA_SRC_MAINLIB_OBSERVER_OBSERVABLE_H
