#include "AudioWorkerFactory.h"

#include <mainlib/globals.h>
#include <mainlib/math/dft/FourierTransformFactory.h>
#include <mainlib/math/dft/SpectrogramBuilder.h>
#include <mainlib/stream/StreamTypes.h>

#include <QAudioDeviceInfo>
#include <iostream>

#include "InterleavedBufferGenerator.h"

using namespace aixa::math;
using namespace std::chrono_literals;

AudioWorker *AudioWorkerFactory::buildWithInputStream(const std::string &streamPath) {
    auto stream = tryToGetStream(streamPath);

    if (!stream) {
        return nullptr;
    }

    auto streamParams = stream->getParameters();
    auto environment = setupAudioEnvironment(streamParams);

    auto streamReader = std::make_unique<StreamReader>(stream, environment.samplesRing);
    auto volumeManager = std::make_unique<VolumeManager>();

    double sampleInterval = 1.0 / streamParams.rate;
    auto spectrogramComputer_p = SpectrogramBuilder(sampleInterval).build(USE_LOG_SCALES);
    auto spectrogramComputer = std::unique_ptr<SpectrogramComputer>(spectrogramComputer_p);

    auto processingThread =
        std::make_unique<AudioProcessingThread>(environment.output, environment.samplesRing,
                                                std::move(volumeManager), std::move(spectrogramComputer));

    return new AudioWorker(std::move(streamReader), std::move(processingThread));
}

std::shared_ptr<Stream> AudioWorkerFactory::tryToGetStream(const std::string &streamPath) {
    auto ret = std::shared_ptr<Stream>(nullptr);

    try {
        ret = StreamTypes::getMatchingStreamFactory(streamPath)->probe();
    } catch (std::exception &e) {
        std::cerr << "Could not obtain stream due to error: " << e.what() << std::endl;
    }

    return ret;
}

AudioEnvironment AudioWorkerFactory::setupAudioEnvironment(AudioStreamParameters &streamParams) {
    AlsaEnvironment alsaEnv;
    alsaEnv.params = AlsaParameters{"default", 500000, 100000us};

    if (!streamParams.littleEndianSamples)
        throw std::runtime_error("Stream format not supported: samples are not in little-endian");

    QAudioFormat format;
    format.setSampleRate(streamParams.rate);
    format.setChannelCount(streamParams.channels);
    format.setSampleSize(streamParams.bitsSample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    auto device = QAudioDeviceInfo::defaultOutputDevice();
    if (!device.isFormatSupported(format)) {
        std::cerr << "Your device does not support required stream format" << std::endl;
        format = device.nearestFormat(format);
    }

    const auto frameCount = computeFrameSize(streamParams.rate, PERIOD_TIME);
    auto frameSize = frameCount * (streamParams.bitsSample / 8) * streamParams.channels;
    auto bufferGenerator = InterleavedBufferGenerator(streamParams.channels, frameCount, streamParams.format);
    auto samplesRing = std::make_shared<SamplesRing>(BUFFERS_FOR_ONE_SECOND, bufferGenerator.generator(),
                                                     frameCount, frameSize, PERIOD_TIME);

    auto output = std::make_shared<QAudioOutput>(device, format);
    return AudioEnvironment(alsaEnv, format, output, samplesRing);
}
