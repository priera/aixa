#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H

#include <mainlib/stream/in/BasicBitReader.h>

#include <memory>
#include <vector>

class ByteReservoir {
public:
    class ByteReservoirOperations {
    public:
        explicit ByteReservoirOperations(ByteReservoir &reservoir,
                                         unsigned int startPos,
                                         unsigned int toRead) :
            reservoir(reservoir),
            currentPos(startPos), toRead(toRead), processed(0) {}

        std::streamsize readNBytes(char *ptr, long n) {
            for (long i = 0; i < n; i++) {
                ptr[i] = reservoir.reservoir[currentPos];
                currentPos = (currentPos + 1) % RESERVOIR_SIZE;
            }

            processed += n;
            return n;
        }

        void skipNBytes(long n) {
            currentPos = (currentPos + n) % RESERVOIR_SIZE;
            processed += n;
        }

        bool ended() const { return processed >= toRead; }

    private:
        ByteReservoir &reservoir;
        unsigned int currentPos;
        unsigned int toRead;

        unsigned int processed;
    };

    explicit ByteReservoir();
    virtual ~ByteReservoir() noexcept = default;

    void append(unsigned int remainingBytes, BitInputReader &reader);
    std::unique_ptr<BitInputReader> readerForPast(unsigned int nBytes);

private:
    static constexpr unsigned int RESERVOIR_SIZE = 512;

    std::vector<char> reservoir;
    std::vector<char> readBuffer;
    unsigned int currentPos;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
