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

    char *frame() const { return charFrame; }
    signed short *samplesFrame() const { return m_samplesFrame; }
    size_t dataSize() const { return m_dataSize; }
    size_t samplesCount() const { return m_frameSize * channels; };
    bool isLittleEndian() const { return little_endian; }

private:
    int channels;
    size_t currentChannel;
    snd_pcm_uframes_t m_frameSize;
    size_t m_dataSize;

    char *charFrame;
    signed short *m_samplesFrame; //sizeof(short) == 2
    std::vector<unsigned char *> ptrToChanelSample;
    snd_pcm_channel_area_t *areas;
    std::vector<int> steps;

    int format_bits;
    int bps;
    int phys_bps;
    bool little_endian;
    bool to_unsigned;

};

#endif //AIXA_BUFFERS_H
