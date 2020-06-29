#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H

#include <mainlib/stream/Stream.h>

#include <string>

#include <mainlib/stream/wav/wavparts/WavFormat.h>
#include <mainlib/stream/FileReader.h>

class WavStream : public Stream {
public:
    WavStream(const std::string &filePath, WavFormat format) :
            f(filePath),
            format(format) {
        if (format.bitsPerSample != 16) {
            throw std::runtime_error("Not allowed WAV stream format");
        }
    }

    ~WavStream() override = default;

    AudioStreamParameters getParameters() const override;
    void storeSamples(InterleavedBuffer &buffer) override;
    void prepareForFirstRead() override;

private:
    FileReader f;
    WavFormat format;
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H
