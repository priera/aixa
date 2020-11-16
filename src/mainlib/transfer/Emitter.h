#ifndef AIXA_SRC_MAINLIB_OBSERVER_EMITTER_H
#define AIXA_SRC_MAINLIB_OBSERVER_EMITTER_H

#include <memory>

#include "Receiver.h"

template <class Entity>
class Emitter {
public:
    using MyReceiver = Receiver<Entity>;

    Emitter() = default;

    virtual ~Emitter() noexcept = default;

    void setReceiver(std::shared_ptr<MyReceiver> r) {
        if (!r) return;
        receiver = r;
    }

    void sendToReceiver(Entity&& newValue) {
        auto sharedReceiver = receiver.lock();
        if (sharedReceiver) {
            sharedReceiver->sendNewValue(std::forward<Entity>(newValue));
        }
    }

private:
    std::weak_ptr<MyReceiver> receiver;
};


#endif //AIXA_SRC_MAINLIB_OBSERVER_EMITTER_H
