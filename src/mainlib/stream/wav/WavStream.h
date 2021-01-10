#ifndef AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H
#define AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H

#include <mainlib/stream/Stream.h>
#include <mainlib/stream/in/FileReader.h>
#include <mainlib/stream/wav/WavFormat.h>

#include <string>

class WavStream : public Stream {
public:
    WavStream(const std::string &filePath, WavFormat format) :
        f(FileReader::buildForFile(filePath)),
        format(format) {
        if (format.bitsPerSample != 16) {
            throw std::runtime_error("Not allowed WAV stream format");
        }
    }

    ~WavStream() override = default;

    AudioStreamParameters getParameters() const override;
    bool ended() override;
    void storeSamples(InterleavedBuffer &buffer) override;
    void prepareForFirstRead() override;

private:
    std::unique_ptr<FileReader> f;
    WavFormat format;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H
