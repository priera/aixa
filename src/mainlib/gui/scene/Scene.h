#ifndef AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
#define AIXA_SRC_MAINLIB_GUI_GL_SCENE_H

#include <QtGui/QMatrix4x4>

#include "ShadedRenderableObject.h"

class Scene {
public:
    Scene(int w, int h);

    virtual ~Scene() = default;

    void setMainObject(ShadedRenderableObject *object) {
        mainObject = object;
    }

    void update();
    void draw();

private:
    QMatrix4x4 projection;
    ShadedRenderableObject *mainObject;
};


#endif //AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
