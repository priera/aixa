#ifndef AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H
#define AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H

#include <mainlib/gui/bitmap/BitmapsProvider.h>

#include <QtGui/QOpenGLFunctions>

class TextureCollection : public QOpenGLFunctions {
public:
    explicit TextureCollection(BitmapsProvider &bitmapsProvider);
    virtual ~TextureCollection() noexcept = default;

    unsigned int getCharacterTexture(char c);

private:
    unsigned int buildTextureForCharacter(char c);

    BitmapsProvider *bitmapsProvider;

    std::unordered_map<char, unsigned int> charTextures;
};

#endif  // AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H
