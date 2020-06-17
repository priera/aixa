#ifndef ALSAPLAYGROUND_SINEGENERATOR_H
#define ALSAPLAYGROUND_SINEGENERATOR_H

#include <math.h>

class InterleavedBuffer;

class SineGenerator {
public:
    SineGenerator(InterleavedBuffer &buffer, int frameSize, double rate);

    void fillFrame(double freq, unsigned int scaleFactor);

private:
    static constexpr double MAX_PHASE = 2 * M_PI;

    InterleavedBuffer * buffers;
    int frameSize;
    double rate;

    double phase;
};


#endif //ALSAPLAYGROUND_SINEGENERATOR_H
