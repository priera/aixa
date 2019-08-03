#include "mainlib/gui/gl/Scene.h"

#include "mainlib/gui/object/ShadedRenderableObject.h"

Scene::Scene(ShadedRenderableObject &mainObject, int w, int h) :
    mainObject(&mainObject) {

    projection.perspective(45.0f,  (float) w / (float)h, 0.1f, 100.0f);
    QMatrix4x4 view;
    view.lookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    projection *= view;
}

void Scene::update() {
    mainObject->update();
}

void Scene::draw() {
    mainObject->render(projection);
}


