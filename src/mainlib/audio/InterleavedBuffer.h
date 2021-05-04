#ifndef AIXA_BUFFERS_H
#define AIXA_BUFFERS_H

#include <alsa/asoundlib.h>

#include <vector>

class InterleavedBuffer {
public:
    InterleavedBuffer(int channels, std::size_t frame_size, snd_pcm_format_t format);

    virtual ~InterleavedBuffer() = default;

    const char *raw() const { return charFrame.data(); }
    size_t dataSize() const { return m_dataSize; }

    // Notice following functions only works if system is little endian
    short *samples() { return reinterpret_cast<short *>(charFrame.data()); };
    const short *samples() const { return reinterpret_cast<const short *>(charFrame.data()); };
    size_t samplesCount() const { return m_frameSize * channels; };
    bool isLittleEndian() const { return little_endian; }

    std::vector<short> channel(unsigned int chan) const;

private:
    int channels;
    size_t m_frameSize;
    size_t m_dataSize;

    std::vector<char> charFrame;

    int format_bits;
    int bytesPerSample;
    bool little_endian;
};

#endif  // AIXA_BUFFERS_H
