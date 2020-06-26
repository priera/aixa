#include "WavStream.h"

void WavStream::read() {

}

bool WavStream::finished() {
    return false;
}

AudioStreamParameters WavStream::getParameters() const {
    return AudioStreamParameters{SND_PCM_FORMAT_U16,
                                 format.samplingRate,
                                 format.channels,
                                 format.bytesPerSecond,
                                 format.bitsPerSample};
}
