#ifndef ALSAPLAYGROUND_BUFFERS_H
#define ALSAPLAYGROUND_BUFFERS_H

#include <vector>

#include <alsa/asoundlib.h>

class Buffers {
public:
    Buffers(int channels, snd_pcm_sframes_t frame_size, snd_pcm_format_t format);

    void startNewFrame();
    void storeNextSample(short sample);

    void *frame() const { return samples; }
    size_t dataSize() const { return frameSize * channels * bps; }

private:
    int channels;
    snd_pcm_sframes_t frameSize;

    signed short *samples; //sizeof(short) == 2
    std::vector<unsigned char *> ptrToChanelSample;
    snd_pcm_channel_area_t *areas;
    std::vector<int> steps;

    int format_bits;
    int bps;
    int phys_bps;
    bool big_endian;
    bool to_unsigned;

};

#endif //ALSAPLAYGROUND_BUFFERS_H
