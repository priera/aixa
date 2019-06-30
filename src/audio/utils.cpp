#include "audio/utils.h"

#include <QJsonDocument>
#include <QFile>

#include "Buffers.h"

namespace audioUtils {
    void dumpSignal(const Buffers &buffers, QJsonArray &out) {
        short sample;
        auto data = static_cast<unsigned char*>(buffers.frame());

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
