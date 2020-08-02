#include "StreamTypes.h"

#include <map>

#include <mainlib/stream/wav/WavStreamFactory.h>
#include <mainlib/stream/synthetic/SyntheticStreamFactory.h>

std::unique_ptr<StreamFactory> StreamTypes::getMatchingStreamFactory(const std::string &streamPath) {
    if (streamPath.empty())
        throw std::runtime_error("Invalid stream path");

    if (streamPath.size() >= 2 && streamPath.substr(0, 2) == "??") {
        return std::make_unique<SyntheticStreamFactory>(streamPath);
    } else {
        validateFileStreamPath(streamPath);
        return std::make_unique<WavStreamFactory>(streamPath);
    }
}

void StreamTypes::validateFileStreamPath(const std::string &streamPath) {
    auto pos = streamPath.rfind('.');

    if (pos == std::string::npos)
        throw std::runtime_error("Not recognized streamPath");

    auto extension = streamPath.substr(pos);

    if (extension != ".wav")
        throw std::runtime_error("Not recognized stream format");
}
