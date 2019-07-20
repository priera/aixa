#ifndef ALSAPLAYGROUND_NOTERENDERABLE_H
#define ALSAPLAYGROUND_NOTERENDERABLE_H

#include "mainlib/gui/RenderableObject.h"

#include <QMatrix4x4>

#include "mainlib/gui/bitmap/CharacterBitmapProvider.h"

class NoteRenderable : public RenderableObject {
public:
    struct Character {
        GLuint textureID;   // ID handle of the glyph texture
        std::vector<unsigned int> size;    // Size of glyph
        std::vector<int> bearing;  // Offset from baseline to left/top of glyph
    };

    NoteRenderable(Character & character, const QMatrix4x4 &projectionMatrix);

protected:
    void doMyRender() override;

private:
    Character character;
    float charw, charh, charPixelRatio;
    unsigned int VBO, VAO;
};


#endif //ALSAPLAYGROUND_NOTERENDERABLE_H
