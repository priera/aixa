#ifndef AIXA_SRC_MAINLIB_AUDIO_AUDIOPROCESSINGTHREAD_H
#define AIXA_SRC_MAINLIB_AUDIO_AUDIOPROCESSINGTHREAD_H

#include <mainlib/math/dft/SpectrogramComputer.h>

#include <QAudioFormat>
#include <QThread>

#include "AudioDefinitions.h"
#include "VolumeManager.h"

class AudioProcessingThread : public QThread {
    Q_OBJECT
public:
    AudioProcessingThread(QAudioFormat format,
                          std::shared_ptr<SamplesRing> samplesRing,
                          std::unique_ptr<VolumeManager> volumeManager,
                          std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer);

    void run() override;

    void increaseVolume() { volumeManager->increaseVolume(); }

    void decreaseVolume() { volumeManager->decreaseVolume(); }

    aixa::math::SpectrogramGenerator &getSpectrogramGenerator() { return *spectrogramComputer; }

private:
    static constexpr unsigned long SLEEP_BETWEEN_WRITES = 7;

    QAudioFormat format;
    std::shared_ptr<SamplesRing> samplesRing;
    std::unique_ptr<VolumeManager> volumeManager;
    std::unique_ptr<aixa::math::SpectrogramComputer> spectrogramComputer;

    const std::size_t toWrite;
};

#endif  // AIXA_SRC_MAINLIB_AUDIO_AUDIOPROCESSINGTHREAD_H
