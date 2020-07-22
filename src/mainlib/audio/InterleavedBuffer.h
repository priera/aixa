#ifndef AIXA_BUFFERS_H
#define AIXA_BUFFERS_H

#include <vector>

#include <alsa/asoundlib.h>

class InterleavedBuffer {
public:
    InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format);

    virtual ~InterleavedBuffer();

    char *raw() const { return charFrame; }
    size_t dataSize() const { return m_dataSize; }

    //Notice following function only works if system is little endian
    short *samples() const { return reinterpret_cast<short *>(&charFrame[0]); };
    size_t samplesCount() const { return m_frameSize * channels; };
    bool isLittleEndian() const { return little_endian; }

private:
    int channels;
    size_t currentChannel;
    snd_pcm_uframes_t m_frameSize;
    size_t m_dataSize;

    //TODO: use a vector of char. Thank you
    char *charFrame;

    int format_bits;
    int bps;
    int phys_bps;
    bool little_endian;
    bool to_unsigned;

};

#endif //AIXA_BUFFERS_H
