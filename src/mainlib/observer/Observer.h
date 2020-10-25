#ifndef AIXA_SRC_MAINLIB_OBSERVER_OBSERVER_H
#define AIXA_SRC_MAINLIB_OBSERVER_OBSERVER_H

template <class Entity>
class Observer {
public:
    virtual ~Observer() = default;

    virtual void notifyNewValue(const Entity& e) = 0;
};

#endif //AIXA_SRC_MAINLIB_OBSERVER_OBSERVER_H
