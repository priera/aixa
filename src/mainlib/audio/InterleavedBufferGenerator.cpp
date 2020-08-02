#include "InterleavedBufferGenerator.h"

BuffersRing<InterleavedBuffer>::ElemGenerator InterleavedBufferGenerator::generator() {
    return [this] { return std::make_shared<InterleavedBuffer>(channels, frameSize, format); };
}
