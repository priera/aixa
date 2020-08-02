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

void SyntheticStream::prepareForFirstRead() {
    begin = Clock::now();
}

bool SyntheticStream::ended() {
    return _ended;
}

void SyntheticStream::storeSamples(InterleavedBuffer &buffer) {
    auto current = Clock::now();

    auto passed = current - begin;

    if (passed > 9000ms) {
        _ended = true;
    } else if (passed > 6000ms) {
        signal.setScaleFactor(400);
        signal.setFrequency(200);

    } else if (passed > 5000ms) {
        signal.setScaleFactor(0);

    } else if (passed > 3000ms) {
        signal.setScaleFactor(500);
        signal.setFrequency(800);

    } else if (passed > 2000ms) {
        signal.setScaleFactor(0);
    }

    auto& newSamples = signal.nextSignal();
    auto buffSamples = buffer.samples();

    for (std::size_t i = 0; i < newSamples.size(); i++) {
        buffSamples[i] = static_cast<short>(newSamples[i]);
    }
}
