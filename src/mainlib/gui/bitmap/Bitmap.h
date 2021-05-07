#ifndef AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H
#define AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H

#include <memory>
#include <vector>

struct Bitmap {
    unsigned int rows, columns;
    int glStorage;
    std::vector<unsigned char> bytes;
    std::shared_ptr<void> data;
};

#endif  // AIXA_SRC_MAINLIB_GUI_BITMAP_BITMAP_H
