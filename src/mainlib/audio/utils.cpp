#include "utils.h"

#include <QJsonDocument>
#include <QFile>

#include "InterleavedBuffer.h"

namespace audioUtils {
    void dumpSignal(const InterleavedBuffer &buffers, QJsonArray &out) {
        short sample;
        auto data = buffers.raw();

        for (int i = 0; i < buffers.dataSize(); i +=2 ) {
         sample = data[i] + (data[i + 1] << 8);
         out << sample;
        }

    }

    void writeSamplesTo(const QJsonArray &samples, const std::string &filename) {
        QFile f(QString::fromStdString(filename));

        if (!f.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Could not open file for writing");
        }

        QJsonDocument doc(samples);
        f.write(doc.toJson());
    }
}
