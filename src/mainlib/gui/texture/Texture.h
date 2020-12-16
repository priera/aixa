#ifndef AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURE_H
#define AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURE_H

#include <mainlib/gui/bitmap/Bitmap.h>
#include <mainlib/gui/bitmap/BitmapProvider.h>

#include <QOpenGLFunctions>

class Texture : public QOpenGLFunctions {
public:
    // Do not actually call this constructor. Is required for compiling, but has no other purpose
    Texture() : id(0) {}

    Texture(unsigned int id, Bitmap bmp);

    virtual ~Texture() = default;

    void use();
    void done();

    const Bitmap &getBitmap() const { return bitmap; }

protected:
    virtual void update(){};

    unsigned int id;
    Bitmap bitmap;
};

#endif  // AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURE_H
