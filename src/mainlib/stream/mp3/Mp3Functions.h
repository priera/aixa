#ifndef AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H
#define AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H

#include <mainlib/stream/FileReader.h>

#include "FrameHeader.h"

namespace mp3Functions {
FrameHeader skipID3data(FileReader& f);

FrameHeader decodeHeader(FileReader& reader, unsigned char secondByte);
}  // namespace mp3Functions

#endif  // AIXA_SRC_MAINLIB_STREAM_MP3_MP3FUNCTIONS_H
