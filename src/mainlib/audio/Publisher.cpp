#include "Publisher.h"

#include <iostream>
#include <thread>

bool Publisher::exec() {
    bool done = false;
    waitForStream();

    if (samplesRing->moreBuffers()) {
        auto buffer = samplesRing->nextReadBuffer();

        //0.1 seconds buffer = 4410 samples. Closest power of two: 4096
        //1024 for fast matrix testing
        //This is ugly as hell, but it is what it is
        //FIXME: do not omit samples on the DFT
        auto doubleVector = aixa::math::DoubleVector(buffer->samples(), 1024);
        auto result = fourierTransform->applyTo(doubleVector);

        volumeManager->applyTo(*buffer);
        int err = snd_pcm_writei(alsaEnv.handle, buffer->raw(), alsaEnv.frame_size);

        if (err < 0)
            attemptStreamRecovery(err);
    } else {
        done = true;
    }

    return done;
}

void Publisher::attemptStreamRecovery(int err) {
    //Other possible error codes than under-run not considered yet
    if (err == -EPIPE) {    /* under-run */
        err = snd_pcm_prepare(alsaEnv.handle);
        if (err < 0) {
            std::stringstream s;
            s << snd_strerror(err);
            throw std::runtime_error(s.str());
        }
    }
}

void Publisher::waitForStream() {
    snd_pcm_uframes_t framesAvailable, frameSize;

    frameSize = alsaEnv.frame_size;
    do {
        framesAvailable = snd_pcm_avail_update(alsaEnv.handle);

        if (framesAvailable < frameSize) {
            if (framesAvailable < 0) {
                std::cout << "Error when checking stream state: " << snd_strerror(framesAvailable) << std::endl;
                attemptStreamRecovery(framesAvailable);
            } else {
                std::this_thread::sleep_for(sleepTime);
            }
        }
    } while (framesAvailable < frameSize);
}
