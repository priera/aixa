#include "InterleavedBuffer.h"

#include <sstream>

InterleavedBuffer::InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format) :
        channels(channels),
        currentChannel(0),
        m_frameSize(frame_size) {

    format_bits = snd_pcm_format_width(format);
    bps = format_bits / 8;
    phys_bps = snd_pcm_format_physical_width(format) / 8;
    little_endian = snd_pcm_format_big_endian(format) != 1;
    to_unsigned = snd_pcm_format_unsigned(format) == 1;
    m_dataSize = m_frameSize * channels * bps;

    charFrame = static_cast<char *>(malloc(m_dataSize));
    if (charFrame == nullptr) {
        throw std::bad_alloc();
    }
}

InterleavedBuffer::~InterleavedBuffer() {
    free(charFrame);
}
