#ifndef AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H
#define AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H

#include <mainlib/gui/bitmap/BitmapsProvider.h>

#include <QtGui/QOpenGLFunctions>

class TextureCollection : public QOpenGLFunctions {
public:
    struct Texture {
        unsigned int id;
        Bitmap bitmap;
    };

    explicit TextureCollection(BitmapsProvider &bitmapsProvider);
    virtual ~TextureCollection() noexcept = default;

    Texture &getCharacterTexture(char c, unsigned int pixelSize);

private:
    using CharKey = std::pair<char, unsigned int>;

    struct PairHash {
        // TODO this should be elsewhere

        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2> &pair) const {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
    };

    Texture buildTextureForCharacter(char c, unsigned int pixelSize);

    BitmapsProvider *bitmapsProvider;

    std::unordered_map<CharKey, Texture, PairHash> charTextures;
};

#endif  // AIXA_SRC_MAINLIB_GUI_TEXTURE_TEXTURECOLLECTION_H
