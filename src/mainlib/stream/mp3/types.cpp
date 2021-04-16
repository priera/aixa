#include "types.h"

std::map<std::size_t, BandIndexes> samplingFreqBandIndexes = {
    {44100,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 52, 62, 74, 90, 110, 134, 162, 196, 238, 288, 342, 418, 576},
      {0, 4, 8, 12, 16, 22, 30, 40, 52, 66, 84, 106, 136, 192}}},
    {48000,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 42, 50, 60, 72, 88, 106, 128, 156, 190, 230, 276, 330, 384, 576},
      {0, 4, 8, 12, 16, 22, 28, 38, 50, 64, 80, 100, 126, 192}}},
    {32000,
     {{0, 4, 8, 12, 16, 20, 24, 30, 36, 44, 54, 66, 82, 102, 126, 156, 194, 240, 296, 364, 448, 550, 576},
      {0, 4, 8, 12, 16, 22, 30, 42, 58, 78, 104, 138, 180, 192}}}};