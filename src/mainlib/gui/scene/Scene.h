#ifndef AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
#define AIXA_SRC_MAINLIB_GUI_GL_SCENE_H

#include <QtGui/QMatrix4x4>
#include <condition_variable>

#include "ShadedRenderableObject.h"

class Scene {
public:
    Scene(int w, int h);

    virtual ~Scene() = default;

    void update();
    void draw();
    void clear();
    void addObject(std::shared_ptr<ShadedRenderableObject> object);

private:
    void waitUntilModificationDone();

    QMatrix4x4 projection;
    std::condition_variable cvModifyingCollection;
    bool beingModified;

    std::vector<std::shared_ptr<ShadedRenderableObject>> objects;
};

#endif  // AIXA_SRC_MAINLIB_GUI_GL_SCENE_H
