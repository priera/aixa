#ifndef AIXA_SRC_MAINLIB_OBSERVER_RECEIVER_H
#define AIXA_SRC_MAINLIB_OBSERVER_RECEIVER_H

template <class Entity>
class Receiver {
public:
    virtual ~Receiver() = default;

    virtual void sendNewValue(Entity&& e) = 0;
};

#endif //AIXA_SRC_MAINLIB_OBSERVER_RECEIVER_H
