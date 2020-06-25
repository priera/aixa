#include "AudioWorker.h"

#include <iostream>

#include <mainlib/stream/StreamTypes.h>

std::unique_ptr<AudioWorker> AudioWorker::runWith(const std::string &streamPath) {
    bool ok = true;

    try {
        auto stream = StreamTypes::getMatchingStreamFactory(streamPath)->probe();
    } catch (std::exception &e){
        ok = false;
        std::cerr << "Could not initialize AudioWorker due to error: " << e.what() << std::endl;
    }

    if (!ok) {
        return std::unique_ptr<AudioWorker>(nullptr);
    }
}
