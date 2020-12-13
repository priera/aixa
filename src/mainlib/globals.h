#ifndef AIXA_SRC_MAINLIB_GLOBALS_H
#define AIXA_SRC_MAINLIB_GLOBALS_H

static constexpr bool USE_LOG_SCALES = false;
static constexpr unsigned int FOURIER_TRANSFORM_SIZE = 2048;
static constexpr unsigned int SPECTROGRAM_SIZE = (FOURIER_TRANSFORM_SIZE / 2) + 1;

#endif  // AIXA_SRC_MAINLIB_GLOBALS_H
