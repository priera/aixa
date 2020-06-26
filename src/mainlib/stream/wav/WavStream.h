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
            format(format) {}

    ~WavStream() override = default;

    void read() override;

    bool finished() override;

private:
    FileReader f;
    WavFormat format;
};


#endif //AIXA_SRC_MAINLIB_STREAM_WAV_WAVSTREAM_H
