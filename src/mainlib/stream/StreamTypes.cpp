#include "StreamTypes.h"

#include <map>

#include <mainlib/stream/wav/WavStreamFactory.h>

std::shared_ptr<StreamFactory> StreamTypes::getMatchingStreamFactory(const std::string &streamPath) {
    auto pos = streamPath.rfind('.');

    if (pos == std::string::npos)
        throw std::runtime_error("Not recognized streamPath");

    auto extension = streamPath.substr(pos);

    if (extension == ".wav") {
        return std::make_shared<WavStreamFactory>(streamPath);
    } else {
        throw std::runtime_error("Not recognized stream format");
    }
}
