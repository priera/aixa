#include "AudioProcessingThread.h"

AudioProcessingThread::AudioProcessingThread(
    std::shared_ptr<QAudioOutput> audioOutput,
    std::shared_ptr<SamplesRing> samplesRing,
    std::unique_ptr<VolumeManager> volumeManager,
    std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer) :
    QThread(),
    audioOutput(std::move(audioOutput)), samplesRing(std::move(samplesRing)),
    volumeManager(std::move(volumeManager)), spectrogramComputer(std::move(spectrogramComputer)),
    toWrite(this->samplesRing->getBufferSize()) {
    this->audioOutput->moveToThread(this);
}

void AudioProcessingThread::run() {
    auto device = audioOutput->start();
    while (samplesRing->moreBuffers()) {
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
