#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H

#include <vector>

struct Bitmap {
    unsigned int rows, columns;
    int glStorage;
    std::vector<unsigned char> bytes;
    void *data;
};

#endif//AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H
