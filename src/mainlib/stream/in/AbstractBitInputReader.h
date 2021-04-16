#ifndef AIXA_SRC_MAINLIB_STREAM_IN_ABSTRACTBITINPUTREADER_H
#define AIXA_SRC_MAINLIB_STREAM_IN_ABSTRACTBITINPUTREADER_H

#include "BitInputReader.h"

class AbstractBitInputReader : public BitInputReader {
public:
    unsigned int nextWord() override {
        unsigned int dummy;
        tryExtractWord(dummy);
        return dummy;
    }

    short nextShort() override {
        short dummy;
        tryExtractShort(dummy);
        return dummy;
    }

    unsigned char nextByte() override {
        unsigned char dummy;
        tryExtractByte(dummy);
        return dummy;
    }

    unsigned short nextNBits(unsigned char n) override {
        unsigned short dummy;
        tryExtractNBits(n, dummy);
        return dummy;
    }
};

#endif  // AIXA_SRC_MAINLIB_STREAM_IN_ABSTRACTBITINPUTREADER_H
