#ifndef AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
#define AIXA_SRC_MAINLIB_GUI_GL_SCENE_H

#include <QtGui/QMatrix4x4>

#include "ShadedRenderableObject.h"

class Scene {
public:
    Scene(int w, int h);

    virtual ~Scene() = default;

    void addObject(std::shared_ptr<ShadedRenderableObject> object) { objects.push_back(std::move(object)); }

    void update();
    void draw();

private:
    QMatrix4x4 projection;

    std::vector<std::shared_ptr<ShadedRenderableObject>> objects;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
