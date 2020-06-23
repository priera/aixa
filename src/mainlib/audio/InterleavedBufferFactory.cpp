#include "InterleavedBufferFactory.h"

BuffersRing<InterleavedBuffer>::ElemGenerator InterleavedBufferFactory::generator() {
    return [this] { return std::make_shared<InterleavedBuffer>(channels, frameSize, format); };
}
