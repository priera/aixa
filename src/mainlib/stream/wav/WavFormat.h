#ifndef AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H
#define AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H

struct WavFormat {
    short compressionCode;
    short channels;
    unsigned int samplingRate;
    unsigned int bytesPerSecond;
    short blockAlign;
    short bitsPerSample;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H
