#ifndef AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H
#define AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H

struct WavFormat {
    unsigned int compressionCode;
    unsigned int channels;
    unsigned int samplingRate;
    unsigned int bytesPerSecond;
    unsigned int blockAlign;
    unsigned int bitsPerSample;
};

#endif //AIXA_SRC_MAINLIB_STREAM_WAVFORMAT_H
