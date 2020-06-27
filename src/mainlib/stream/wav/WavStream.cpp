#include "WavStream.h"

bool WavStream::finished() {
    return false;
}

AudioStreamParameters WavStream::getParameters() const {
    return AudioStreamParameters{SND_PCM_FORMAT_S16,
                                 format.samplingRate,
                                 format.channels,
                                 format.bytesPerSecond,
                                 format.bitsPerSample};
}

short WavStream::nextSample() {
    return 0;
}
