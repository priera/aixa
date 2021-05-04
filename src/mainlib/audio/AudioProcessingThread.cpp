#include "AudioProcessingThread.h"

#include <iostream>

AudioProcessingThread::AudioProcessingThread(
    QAudioFormat format,
    std::shared_ptr<SamplesRing> samplesRing,
    std::unique_ptr<VolumeManager> volumeManager,
    std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer) :
    QThread(),
    format(format), samplesRing(std::move(samplesRing)), volumeManager(std::move(volumeManager)),
    spectrogramComputer(std::move(spectrogramComputer)), toWrite(this->samplesRing->getBufferSize()) {}

void AudioProcessingThread::run() {
    auto physicalDevice = QAudioDeviceInfo::defaultOutputDevice();
    if (!physicalDevice.isFormatSupported(format)) {
        std::cerr << "Your device does not support required stream format" << std::endl;
        format = physicalDevice.nearestFormat(format);
    }

    auto audioOutput = std::make_unique<QAudioOutput>(physicalDevice, format);

    auto device = audioOutput->start();
    while (samplesRing->moreBuffers() && !this->isInterruptionRequested()) {
        auto buffer = samplesRing->nextReadBuffer();

        auto firstChannel = buffer->channel(0);
        auto doubleVector = aixa::math::DoubleVector(firstChannel.begin(), firstChannel.size());
        spectrogramComputer->computeOn(doubleVector);

        volumeManager->applyTo(*buffer);
        auto written = device->write(buffer->raw(), toWrite);
        while (written < toWrite) {
            msleep(SLEEP_BETWEEN_WRITES);
            written += device->write(buffer->raw() + written, toWrite - written);
        }
    }

    audioOutput->stop();
}
