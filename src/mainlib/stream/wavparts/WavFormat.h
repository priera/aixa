#ifndef ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVFORMAT_H
#define ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVFORMAT_H

struct WavFormat {
    unsigned int compressionCode;
    unsigned int channels;
    unsigned int samplingRate;
    unsigned int bytesPerSecond;
    unsigned int blockAlign;
    unsigned int bitsPerSample;
};

#endif //ALSAPLAYGROUND_SRC_MAINLIB_STREAM_WAVFORMAT_H
