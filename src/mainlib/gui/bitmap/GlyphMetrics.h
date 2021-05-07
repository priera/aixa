#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H

struct GlyphMetrics {
    GlyphMetrics(int top, int left, int width, int height, long advanceX) :
        top(top), left(left), width(width), height(height), advanceX(advanceX) {}

    int top, left, width, height;
    long advanceX;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_GLYPHMETRICS_H
