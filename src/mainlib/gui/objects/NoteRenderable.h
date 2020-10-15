#ifndef AIXA_NOTERENDERABLE_H
#define AIXA_NOTERENDERABLE_H

#include <mutex>

#include <QMatrix4x4>

#include <mainlib/gui/scene/RenderableObject.h>
#include <mainlib/gui/CharTextureProvider.h>

class NoteRenderable : public RenderableObject {
public:
    NoteRenderable(QOpenGLShaderProgram &program, Dimensions dim);

    void setCharacter(const CharTextureProvider::Character & charTex);

protected:
    bool readyToInitialize() override;
    void init() override;

    void doMyRender() override;

private:
    void updateOnCharData();

    std::mutex charUpdateMutex;

    const CharTextureProvider::Character *character;
    float charPixelRatio;
    unsigned int VBO, VAO;
};


#endif //AIXA_NOTERENDERABLE_H
