#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H

#include <mainlib/stream/FileReader.h>

#include "FrameHeader.h"

namespace mp3Functions {
bool seekToNextFrame(FileReader& f, FrameHeader& header);

FrameHeader decodeHeader(FileReader& reader, unsigned char secondByte);

void decodeSideInformation(FileReader& reader, const FrameHeader& header);
}  // namespace mp3Functions

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H
