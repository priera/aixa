#include "WavStreamFactory.h"

#include <mainlib/stream/in/FileReader.h>

#include <filesystem>

#include "WavFunctions.h"
#include "WavStream.h"

namespace fs = std::filesystem;

std::shared_ptr<Stream> WavStreamFactory::probe() {
    const unsigned int PROBING_BYTES = 24;

    auto realFileSize = fs::file_size(filePath);
    if (realFileSize < PROBING_BYTES) throw std::runtime_error("Invalid input file");

    auto f = std::make_unique<FileReader>(filePath);

    unsigned int size;
    WavFunctions::readHeader(*f, size);
    if (realFileSize < size) throw std::runtime_error("Invalid input file");

    std::string idTag;
    f->nextChunkInfo(idTag, size);

    // Actually, fmt chunk is not necessarily the first in a proper WAV file, but it is in my test cases
    if (idTag != "fmt ") throw std::runtime_error("Invalid WAV file");

    WavFormat format{};
    WavFunctions::readFormat(*f, size, format);

    if (format.bitsPerSample != 16) throw std::runtime_error("Not supported WAV sample format");

    return std::make_shared<WavStream>(filePath, format);
}
