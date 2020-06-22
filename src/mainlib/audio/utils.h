
#ifndef AIXA_UTILS_H
#define AIXA_UTILS_H

#include <string>

#include <QJsonArray>

class InterleavedBuffer;

namespace audioUtils {
    void dumpSignal(const InterleavedBuffer &buffers, QJsonArray &out);
    void writeSamplesTo(const QJsonArray &samples, const std::string &filename);
}

#endif //AIXA_UTILS_H
