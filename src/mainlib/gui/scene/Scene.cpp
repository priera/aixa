#include "Scene.h"

Scene::Scene(int w, int h) {
    // projection.perspective(45.0f,  (float) w / (float)h, 0.1f, 100.0f);
    projection.ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
    QMatrix4x4 view;
    view.lookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    projection *= view;
}

void Scene::update() {
    for (auto &object : objects) {
        object->update();
    }
}

void Scene::draw() {
    for (auto &object : objects) {
        object->render(projection);
    }
}
