#ifndef AIXA_AUDIODEFINITIONS_H
#define AIXA_AUDIODEFINITIONS_H

#include <mainlib/threading/BuffersRing.h>

#include <QAudioOutput>
#include <string>
#include <utility>
#include <vector>

#include "InterleavedBuffer.h"

using SamplesRing = BuffersRing<InterleavedBuffer>;

struct AudioEnvironment {
    AudioEnvironment(QAudioFormat format,
                     std::shared_ptr<QAudioOutput> output,
                     std::shared_ptr<SamplesRing> ring) :
        format(format),
        output(std::move(output)), samplesRing(std::move(ring)) {}

    QAudioFormat format;
    std::shared_ptr<QAudioOutput> output;
    std::shared_ptr<SamplesRing> samplesRing;
};

#endif  // AIXA_AUDIODEFINITIONS_H
