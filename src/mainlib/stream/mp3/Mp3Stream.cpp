#include "Mp3Stream.h"

AudioStreamParameters Mp3Stream::getParameters() const {
    return {SND_PCM_FORMAT_S16,
            snd_pcm_format_little_endian(SND_PCM_FORMAT_S16) == 1,
            streamDefinition.samplingFreq,
            static_cast<unsigned int>(streamDefinition.channels()),
            streamDefinition.bitrate / S_BYTE,
            16};
}

void Mp3Stream::prepareForFirstRead() { decoder->resetToBeginning(); }

bool Mp3Stream::ended() { return alreadyEnded || !decoder->seekToNextFrame(); }

void Mp3Stream::storeSamples(InterleavedBuffer& buffer) {
    auto samplesCount = buffer.samplesCount();
    auto data = buffer.samples();
    if (previousIterCopied) {
        std::copy(samples.channel1.begin() + previousIterCopied, samples.channel1.end(), data);
    }

    std::size_t iterationsToDo = (samplesCount - previousIterCopied) / NR_FRAME_SAMPLES;
    std::size_t samplesCopied = previousIterCopied;
    for (std::size_t i = 0; i < iterationsToDo; i++) {
        decoder->decodeFrame(samples);
        std::copy(samples.channel1.begin(), samples.channel1.end(), &data[samplesCopied]);
        samplesCopied += NR_FRAME_SAMPLES;

        alreadyEnded = !decoder->seekToNextFrame();
        if (alreadyEnded)
            break;
    }

    if (alreadyEnded) {
        std::fill(&data[samplesCopied], &data[samplesCount], 0);
    } else {
        std::size_t remainder = samplesCount - samplesCopied;
        decoder->decodeFrame(samples);
        std::copy(samples.channel1.begin(), samples.channel1.begin() + remainder, &data[samplesCopied]);
        previousIterCopied = remainder;
    }
}
