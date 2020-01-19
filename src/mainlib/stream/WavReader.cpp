#include "WavReader.h"

WavReader::WavReader(const std::string & path) : f(path) { }

void WavReader::run() {
    std::string s;
    s.reserve(4);

    bool ok = f.nextIdTag(s);

}
