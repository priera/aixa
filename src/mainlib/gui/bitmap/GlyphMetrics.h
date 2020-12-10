#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H

struct GlyphMetrics {
    GlyphMetrics(unsigned int top, unsigned int left, unsigned int width, unsigned int height,
                 long advanceX) :
        top(top), left(left), width(width), height(height), advanceX(advanceX) {}

    unsigned int top, left, width, height, advanceX;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H
