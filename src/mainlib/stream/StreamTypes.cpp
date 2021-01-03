#include "StreamTypes.h"

#include <mainlib/stream/mp3/Mp3StreamFactory.h>
#include <mainlib/stream/synthetic/SyntheticStreamFactory.h>
#include <mainlib/stream/wav/WavStreamFactory.h>

std::unique_ptr<StreamFactory> StreamTypes::getMatchingStreamFactory(const std::string &streamPath) {
    if (streamPath.empty()) throw std::runtime_error("Invalid stream path");

    if (streamPath.size() >= 2 && streamPath.substr(0, 2) == "??") {
        return std::make_unique<SyntheticStreamFactory>(streamPath);
    }

    auto fileExtension = validateFileStreamPath(streamPath);

    if (fileExtension == ".wav") {
        return std::make_unique<WavStreamFactory>(streamPath);
    } else if (fileExtension == ".mp3") {
        return std::make_unique<Mp3StreamFactory>(streamPath);
    } else {
        throw std::runtime_error("Not recognized stream format");
    }
}

std::string StreamTypes::validateFileStreamPath(const std::string &streamPath) {
    auto pos = streamPath.rfind('.');

    if (pos == std::string::npos) throw std::runtime_error("Not recognized streamPath");

    return streamPath.substr(pos);
}
