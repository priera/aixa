#include "SyntheticStream.h"

using namespace std::chrono_literals;

AudioStreamParameters SyntheticStream::getParameters() const {
    auto sampleFormat = SND_PCM_FORMAT_S16;
    const auto sampleRate = 44100;
    const auto bytesPerSample = 2;
    const auto channels = 1;
    const auto bytesPerSecond = sampleRate * bytesPerSample * channels;

    return AudioStreamParameters{sampleFormat,
                                 snd_pcm_format_little_endian(sampleFormat) == 1,
                                 sampleRate,
                                 channels,
                                 bytesPerSecond,
                                 bytesPerSample * 8};
}

SyntheticStream::SyntheticStream(std::size_t signalSize,
    double samplePeriod,
    std::vector<StreamStep> streamSteps) :
    Stream(),
    signalSize(signalSize),
    streamSteps(std::move(streamSteps)),
    sineGenerator(signalSize, samplePeriod, 0) {
    currentStep = this->streamSteps.cbegin();
}

void SyntheticStream::prepareForFirstRead() {
    begin = Clock::now();
}

bool SyntheticStream::ended() {
    return currentStep == (streamSteps.end() - 1);
}

void SyntheticStream::storeSamples(InterleavedBuffer &buffer) {
    auto current = Clock::now();

    auto passed = current - begin;
    auto nextStep = currentStep + 1;
    if (passed > (nextStep->startTime - begin)) {
        currentStep++;
    }

    auto finalSignal = aixa::math::DoubleVector(signalSize);

    auto freqsSize = currentStep->chord.getFrequencies().size();
    for (std::size_t i = 0; i < freqsSize; i++) {
        sineGenerator.setFrequency(currentStep->chord.getFrequencies()[i]);
        sineGenerator.setScaleFactor(currentStep->volume);

        finalSignal += sineGenerator.nextSignal();
    }

    auto buffSamples = buffer.samples();

    for (std::size_t i = 0; i < finalSignal.size(); i++) {
        buffSamples[i] = static_cast<short>(finalSignal[i]);
    }
}
