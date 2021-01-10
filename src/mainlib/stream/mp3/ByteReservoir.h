#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H

#include <mainlib/stream/in/BasicByteReader.h>

#include <memory>
#include <vector>

class ByteReservoir {
public:
    class ByteReservoirOperations {
    public:
        explicit ByteReservoirOperations(ByteReservoir &reservoir, unsigned int startPos,
                                         unsigned int toRead) :
            reservoir(reservoir),
            currentPos(startPos),
            toRead(toRead),
            processed(0) {}

        std::streamsize readNBytes(char *ptr, long n) {
            for (long i = 0; i < n; i++) {
                ptr[i] = reservoir.reservoir[currentPos];
                currentPos = (currentPos + 1) % RESERVOIR_SIZE;
            }

            processed += n;
            reservoir.capacity -= n;
            return n;
        }

        void skipNBytes(long n) {
            currentPos = (currentPos + n) % RESERVOIR_SIZE;
            reservoir.capacity -= n;
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

    void append(unsigned int remainingBytes, ByteReader &reader);
    void advance(unsigned int nBytes);
    std::vector<char> extract(unsigned int nBytes);
    std::unique_ptr<ByteReader> readerForPast(unsigned int nBytes);
    bool consumed() const { return capacity == 0; }

private:
    static constexpr unsigned int RESERVOIR_SIZE = 512;

    std::vector<char> reservoir;
    std::vector<char> readBuffer;
    unsigned int currentPos;
    unsigned int capacity;
};

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_BYTERESERVOIR_H
