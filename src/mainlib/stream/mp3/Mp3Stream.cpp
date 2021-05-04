#include "Mp3Stream.h"

QAudioFormat Mp3Stream::getParameters() const {
    QAudioFormat ret;
    ret.setSampleRate(streamDefinition.samplingFreq);
    ret.setChannelCount(streamDefinition.channels());
    ret.setSampleSize(16);
    ret.setCodec("audio/pcm");
    ret.setByteOrder(QAudioFormat::LittleEndian);
    ret.setSampleType(QAudioFormat::SignedInt);
    return ret;
}

void Mp3Stream::prepareForFirstRead() { decoder->resetToBeginning(); }

bool Mp3Stream::ended() { return alreadyEnded; }

void Mp3Stream::storeSamples(InterleavedBuffer& buffer) {
    auto samplesCount = buffer.samplesCount();
    auto data = buffer.samples();

    const auto samplesPerFrame = streamDefinition.channels() * NR_FRAME_SAMPLES;
    std::size_t samplesCopied = 0;
    if (previousIterCopied) {
        copyFrameSamples(samplesPerFrame, data, previousIterCopied / streamDefinition.channels());
        samplesCopied = samplesPerFrame - previousIterCopied;
    }

    std::size_t iterationsToDo = (samplesCount - samplesCopied) / samplesPerFrame;
    for (std::size_t i = 0; i < iterationsToDo; i++) {
        alreadyEnded = !decoder->seekToNextFrame();
        if (alreadyEnded)
            break;

        decoder->decodeFrame(samples);
        copyFrameSamples(samplesPerFrame, &data[samplesCopied]);
        samplesCopied += samplesPerFrame;
    }

    std::size_t remainder = samplesCount - samplesCopied;
    if (remainder > 0 && !alreadyEnded) {
        alreadyEnded = !decoder->seekToNextFrame();
    }

    previousIterCopied = 0;
    if (alreadyEnded) {
        std::fill(&data[samplesCopied], &data[samplesCount], 0);
    } else if (remainder > 0) {
        decoder->decodeFrame(samples);
        copyFrameSamples(remainder, &data[samplesCopied]);
        previousIterCopied = remainder;
    }
}

void Mp3Stream::copyFrameSamples(std::size_t count, short* to, std::size_t startOffset) const {
    if (streamDefinition.channels() == 1) {
        std::copy(samples.channel1.begin() + startOffset, samples.channel1.begin() + count, to);
    } else {
        for (std::size_t i = startOffset, destInd = 0; destInd < count; i++, destInd += 2) {
            to[destInd] = samples.channel1[i];
            to[destInd + 1] = samples.channel2[i];
        }
    }
}
