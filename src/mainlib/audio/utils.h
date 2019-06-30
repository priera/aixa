
#ifndef ALSAPLAYGROUND_UTILS_H
#define ALSAPLAYGROUND_UTILS_H

#include <string>

#include <QJsonArray>

class Buffers;

namespace audioUtils {
    void dumpSignal(const Buffers &buffers, QJsonArray &out);
    void writeSamplesTo(const QJsonArray &samples, const std::string &filename);
}

#endif //ALSAPLAYGROUND_UTILS_H
