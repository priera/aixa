#ifndef ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_SCENE_H
#define ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_SCENE_H

#include <QtGui/QMatrix4x4>

class ShadedRenderableObject;

class Scene {
public:
    Scene(ShadedRenderableObject &mainObject, int w, int h);
    virtual ~Scene() = default;

    void update();
    void draw();

private:
    QMatrix4x4 projection;
    ShadedRenderableObject *mainObject;
};


#endif //ALSAPLAYGROUND_SRC_MAINLIB_GUI_GL_SCENE_H
