#ifndef AIXA_BUFFERS_H
#define AIXA_BUFFERS_H

#include <vector>

#include <alsa/asoundlib.h>

class InterleavedBuffer {
public:
    InterleavedBuffer(int channels, snd_pcm_uframes_t frame_size, snd_pcm_format_t format);

    virtual ~InterleavedBuffer();

    void startNewFrame();
    void storeNextSample(short sample);

    void *frame() const { return m_frame; }
    size_t dataSize() const { return m_frameSize * channels * bps; }
    size_t samplesCount() const { return m_frameSize * channels; };

private:
    int channels;
    size_t currentChannel;
    snd_pcm_uframes_t m_frameSize;

    signed short *m_frame; //sizeof(short) == 2
    std::vector<unsigned char *> ptrToChanelSample;
    snd_pcm_channel_area_t *areas;
    std::vector<int> steps;

    int format_bits;
    int bps;
    int phys_bps;
    bool big_endian;
    bool to_unsigned;

};

#endif //AIXA_BUFFERS_H
