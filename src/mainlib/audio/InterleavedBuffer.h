#ifndef AIXA_BUFFERS_H
#define AIXA_BUFFERS_H

#include <vector>

class InterleavedBuffer {
public:
    InterleavedBuffer(int channels, std::size_t frame_size, unsigned int bytesPerSample);

    virtual ~InterleavedBuffer() = default;

    const char *raw() const { return charFrame.data(); }
    char *raw() { return charFrame.data(); }
    size_t dataSize() const { return m_dataSize; }

    // Notice following functions only work if system is little endian
    short *samples() { return reinterpret_cast<short *>(charFrame.data()); };
    const short *samples() const { return reinterpret_cast<const short *>(charFrame.data()); };
    size_t samplesCount() const { return m_frameSize * channels; };

    std::vector<short> channel(unsigned int chan) const;

private:
    int channels;
    size_t m_frameSize;
    size_t m_dataSize;

    std::vector<char> charFrame;

    unsigned int bytesPerSample;
};

#endif  // AIXA_BUFFERS_H
