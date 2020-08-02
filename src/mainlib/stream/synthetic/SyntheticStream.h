#ifndef AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H
#define AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H

#include <mainlib/stream/Stream.h>

#include <chrono>

#include <mainlib/math/SineGenerator.h>

class SyntheticStream : public Stream {
public:
    SyntheticStream() :
        Stream(),
        _ended(false),
        signal(4410, 1.0 / 44100, 440, 200) {}

    AudioStreamParameters getParameters() const override;
    void prepareForFirstRead() override;
    bool ended() override;
    void storeSamples(InterleavedBuffer &buffer) override;

private:
    using Clock = std::chrono::steady_clock;
    std::chrono::time_point<Clock> begin;

    bool _ended;

    aixa::math::SineGenerator signal;
};


#endif //AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H
