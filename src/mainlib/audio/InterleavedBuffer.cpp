#include "InterleavedBuffer.h"

InterleavedBuffer::InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format) :
    channels(channels), currentChannel(0), m_frameSize(frame_size) {
    format_bits = snd_pcm_format_width(format);
    bytesPerSample = format_bits / 8;
    phys_bps = snd_pcm_format_physical_width(format) / 8;
    little_endian = snd_pcm_format_big_endian(format) != 1;
    to_unsigned = snd_pcm_format_unsigned(format) == 1;
    m_dataSize = m_frameSize * channels * bytesPerSample;

    charFrame = static_cast<char *>(calloc(m_frameSize * channels, bytesPerSample));
    if (charFrame == nullptr) {
        throw std::bad_alloc();
    }
}

InterleavedBuffer::~InterleavedBuffer() { free(charFrame); }

std::vector<short> InterleavedBuffer::channel(unsigned int chan) const {
    assert(chan >= 0 && chan < channels);

    if (channels == 1) {
        return std::vector<short>(samples(), samples() + samplesCount());
    } else {
        std::vector<short> ret(m_frameSize);
        auto allSamples = samples();
        std::size_t sample = chan;
        for (std::size_t i = 0; i < m_frameSize; i++) {
            ret[i] = allSamples[sample];
            sample += channels;
        }

        return ret;
    }
}