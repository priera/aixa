#ifndef ALSAPLAYGROUND_NOTERENDERABLE_H
#define ALSAPLAYGROUND_NOTERENDERABLE_H

#include <mutex>

#include <QMatrix4x4>

#include "mainlib/gui/RenderableObject.h"
#include "mainlib/gui/CharTextureProvider.h"


class NoteRenderable : public RenderableObject {
public:
    NoteRenderable(CharTextureProvider::Character & character, const QMatrix4x4 &projectionMatrix, QOpenGLShaderProgram &program);

    void setCharacter(const CharTextureProvider::Character & character);

protected:
    void doMyRender() override;

private:
    void updateOnCharData();

    std::mutex charUpdateMutex;

    CharTextureProvider::Character character;
    float charw, charh, charPixelRatio;
    unsigned int VBO, VAO;
};


#endif //ALSAPLAYGROUND_NOTERENDERABLE_H
