#ifndef ALSAPLAYGROUND_NOTERENDERABLE_H
#define ALSAPLAYGROUND_NOTERENDERABLE_H

#include <mutex>

#include <QMatrix4x4>

#include "mainlib/gui/RenderableObject.h"
#include "mainlib/gui/CharTextureProvider.h"


class NoteRenderable : public RenderableObject {
public:
    NoteRenderable(CharTextureProvider::Character & character, QOpenGLShaderProgram &program);

    void setCharacter(const CharTextureProvider::Character & character);

protected:
    void doMyRender() override;

private:
    void updateOnCharData();
    void init();

    std::mutex charUpdateMutex;

    CharTextureProvider::Character character;
    float charPixelRatio;
    unsigned int VBO, VAO;

    bool initialized;
};


#endif //ALSAPLAYGROUND_NOTERENDERABLE_H
