#ifndef AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H
#define AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H

#include <mainlib/stream/Stream.h>

#include <chrono>

#include <mainlib/math/SineGenerator.h>
#include <mainlib/audio/note/Chord.h>

class SyntheticStream : public Stream {
public:
    using Clock = std::chrono::steady_clock;

    struct StreamStep {
        int volume;
        Chord chord;
        std::chrono::time_point<Clock> startTime;
    };

    SyntheticStream(std::size_t signalSize,
                    double samplePeriod,
                    std::vector<StreamStep> streamSteps);

    AudioStreamParameters getParameters() const override;
    void prepareForFirstRead() override;
    bool ended() override;
    void storeSamples(InterleavedBuffer &buffer) override;

private:
    std::chrono::time_point<Clock> begin;

    std::size_t signalSize;
    std::vector<StreamStep> streamSteps;
    std::vector<StreamStep>::const_iterator currentStep;
    aixa::math::SineGenerator sineGenerator;
};


#endif //AIXA_SRC_MAINLIB_STREAM_SYNTHETIC_SYNTHETICSTREAM_H
