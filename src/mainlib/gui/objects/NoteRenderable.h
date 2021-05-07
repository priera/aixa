#ifndef AIXA_NOTERENDERABLE_H
#define AIXA_NOTERENDERABLE_H

#include <mainlib/gui/scene/RenderableObject.h>
#include <mainlib/gui/texture/Texture.h>

#include <QMatrix4x4>
#include <mutex>

class NoteRenderable : public RenderableObject {
public:
    NoteRenderable(QOpenGLShaderProgram &program, Dimensions dim);

    void setCharacterText(Texture &charTex);

protected:
    bool readyToInitialize() override;
    void init() override;

    void doMyRender() override;

private:
    void updateOnCharData();

    std::mutex charUpdateMutex;

    std::shared_ptr<Texture> charText;
    unsigned int VBO, VAO;
};

#endif  // AIXA_NOTERENDERABLE_H
