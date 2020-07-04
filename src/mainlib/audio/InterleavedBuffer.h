#ifndef AIXA_BUFFERS_H
#define AIXA_BUFFERS_H

#include <vector>

#include <alsa/asoundlib.h>

class InterleavedBuffer {
public:
    InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format);

    virtual ~InterleavedBuffer();

    char *frame() const { return charFrame; }
    size_t dataSize() const { return m_dataSize; }
    size_t samplesCount() const { return m_frameSize * channels; };
    bool isLittleEndian() const { return little_endian; }

private:
    int channels;
    size_t currentChannel;
    snd_pcm_uframes_t m_frameSize;
    size_t m_dataSize;

    char *charFrame;

    int format_bits;
    int bps;
    int phys_bps;
    bool little_endian;
    bool to_unsigned;

};

#endif //AIXA_BUFFERS_H