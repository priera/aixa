#include "SyntheticStream.h"

using namespace std::chrono_literals;

QAudioFormat SyntheticStream::getParameters() const {
    QAudioFormat ret;
    ret.setSampleRate(44100);
    ret.setChannelCount(1);
    ret.setSampleSize(16);
    ret.setCodec("audio/pcm");
    ret.setByteOrder(QAudioFormat::LittleEndian);
    ret.setSampleType(QAudioFormat::SignedInt);
    return ret;
}

SyntheticStream::SyntheticStream(std::size_t signalSize,
                                 double samplePeriod,
                                 std::vector<StreamStep> streamSteps) :
    Stream(),
    signalSize(signalSize), streamSteps(std::move(streamSteps)), sineGenerator(signalSize, samplePeriod, 0) {
    currentStep = this->streamSteps.cbegin();
}

void SyntheticStream::prepareForFirstRead() { begin = Clock::now(); }

bool SyntheticStream::ended() { return currentStep == (streamSteps.end() - 1); }

void SyntheticStream::storeSamples(InterleavedBuffer &buffer) {
    auto current = Clock::now();

    auto passed = current - begin;
    auto nextStep = currentStep + 1;
    if (passed >= (nextStep->startTime - begin)) {
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
