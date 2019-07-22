#ifndef ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVER_H
#define ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVER_H

template <class Entity>
class Observer {
public:
    virtual void notifyNewValue(const Entity &e) = 0;
};

#endif //ALSAPLAYGROUND_SRC_MAINLIB_OBSERVER_OBSERVER_H
