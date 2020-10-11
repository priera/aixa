#ifndef AIXA_NOTERENDERABLE_H
#define AIXA_NOTERENDERABLE_H

#include <mutex>

#include <QMatrix4x4>

#include "mainlib/gui/scene/RenderableObject.h"
#include "mainlib/gui/CharTextureProvider.h"


class NoteRenderable : public RenderableObject {
public:
    explicit NoteRenderable(QOpenGLShaderProgram &program);

    void setCharacter(const CharTextureProvider::Character & charTex);

protected:
    void doMyRender() override;

private:
    void updateOnCharData();
    void init();

    std::mutex charUpdateMutex;

    const CharTextureProvider::Character *character;
    float charPixelRatio;
    unsigned int VBO, VAO;

    bool initialized;
};


#endif //AIXA_NOTERENDERABLE_H
