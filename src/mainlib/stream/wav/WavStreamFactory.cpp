#include "WavStreamFactory.h"

#include <mainlib/stream/FileReader.h>

#include "WavFunctions.h"
#include "WavStream.h"

std::shared_ptr<Stream> WavStreamFactory::probe() {
    FileReader f(filePath);

    unsigned int size;
    WavFunctions::readHeader(f, size);

    std::string idTag;
    f.nextChunkInfo(idTag, size);

    //Actually, fmt chunk is not necessarily the first in a proper WAV file, but it is in my test cases
    if (idTag != "fmt ")
        throw std::runtime_error("Invalid WAV file");

    WavFormat format{};
    WavFunctions::readFormat(f, size, format);

    return std::make_shared<WavStream>(filePath, format);
}
