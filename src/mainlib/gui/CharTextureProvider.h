#ifndef AIXA_NOTEPROVIDER_H
#define AIXA_NOTEPROVIDER_H

#include <map>

#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>

class RenderableObject;

class CharTextureProvider : protected QOpenGLExtraFunctions {
public:
    struct Character {
        GLuint textureID;   // ID handle of the glyph texture
        std::vector<unsigned int> size;    // Size of glyph
        std::vector<int> bearing;  // Offset from baseline to left/top of glyph
    };

    CharTextureProvider();

    Character &generateChar(char c);

private:
    std::map<char, Character> characters;
};


#endif //AIXA_NOTEPROVIDER_H
