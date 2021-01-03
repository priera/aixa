#include "Mp3Functions.h"

#include <unordered_map>
#include <vector>

namespace mp3Functions {

static std::unordered_map<unsigned char, unsigned int> BIT_RATE_DICTIONARY = {
    {1, 32},  {2, 40},  {3, 48},   {4, 56},   {5, 64},   {6, 80},   {7, 96},
    {8, 112}, {9, 128}, {10, 160}, {11, 192}, {12, 224}, {13, 256}, {14, 320}};

static std::vector<unsigned int> SAMPLING_FREQS = {44100, 48000, 32000};

FrameHeader skipID3data(FileReader& f) {
    // Actually, I do not check for any ID3 stuff. Just skip bytes until a valid header is found
    // A byte-alignment for the header is assumed.

    unsigned char b;
    bool headerFound = false;
    bool headerStartRead = false;
    while (!headerFound && !f.ended()) {
        b = f.nextByte();

        if (headerStartRead) {
            if (b & 0xF0) {
                headerFound = true;
            } else {
                headerStartRead = false;
            }
        } else if (b == 0xFF) {
            headerStartRead = true;
        }
    }

    if (f.ended()) throw std::runtime_error("Invalid mp3 file");

    return decodeHeader(f, b);
}

FrameHeader decodeHeader(FileReader& reader, unsigned char secondByte) {
    auto ret = FrameHeader();

    ret.version = (secondByte & 0x08) ? FrameHeader::Version::MPEG_1 : FrameHeader::Version::MPEG_2;
    ret.layer = static_cast<FrameHeader::Layer>((secondByte & 0x06) >> 1);
    ret.usesCRC = !(secondByte & 0x01);

    if (!(ret.version == FrameHeader::Version::MPEG_1 && ret.layer == FrameHeader::Layer::LAYER_3))
        throw std::runtime_error("Not supported MP3 format");

    unsigned char b = reader.nextByte();

    unsigned char bitRateIndex = (b & 0xF0) >> 4;
    if (bitRateIndex == 0 || bitRateIndex >= 15) throw std::runtime_error("Not supported MP3 format");
    ret.bitrate = BIT_RATE_DICTIONARY[bitRateIndex];

    std::size_t freqIndex = (b & 0x0A) >> 2;
    if (freqIndex > 2) throw std::runtime_error("Not supported MP3 format");
    ret.samplingFreq = SAMPLING_FREQS[freqIndex];

    ret.isPadded = b & 0x02;

    b = reader.nextByte();
    ret.mode = static_cast<FrameHeader::Mode>(b >> 6);

    ret.msStereo = b & 0x20;
    ret.intensityStereo = b & 0x10;

    return ret;
}

}  // namespace mp3Functions
