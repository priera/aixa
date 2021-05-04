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

    std::size_t samplesCopied = 0;
    if (previousIterCopied) {
        std::copy(samples.channel1.begin() + previousIterCopied, samples.channel1.end(), data);
        samplesCopied = NR_FRAME_SAMPLES - previousIterCopied;
    }

    std::size_t iterationsToDo = (samplesCount - samplesCopied) / NR_FRAME_SAMPLES;
    for (std::size_t i = 0; i < iterationsToDo; i++) {
        alreadyEnded = !decoder->seekToNextFrame();
        if (alreadyEnded)
            break;

        decoder->decodeFrame(samples);
        std::copy(samples.channel1.begin(), samples.channel1.end(), &data[samplesCopied]);
        samplesCopied += NR_FRAME_SAMPLES;
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
        std::copy(samples.channel1.begin(), samples.channel1.begin() + remainder, &data[samplesCopied]);
        previousIterCopied = remainder;
    }
}
