#include "Mp3Stream.h"

AudioStreamParameters Mp3Stream::getParameters() const {
    auto bytesPerSecond =
        static_cast<unsigned int>(streamDefinition.samplingFreq * streamDefinition.channels() * 2);
    return {SND_PCM_FORMAT_S16,
            snd_pcm_format_little_endian(SND_PCM_FORMAT_S16) == 1,
            streamDefinition.samplingFreq,
            static_cast<unsigned int>(streamDefinition.channels()),
            bytesPerSecond,
            16};
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
