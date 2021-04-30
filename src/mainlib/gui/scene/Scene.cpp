#include "Scene.h"

Scene::Scene(int w, int h) : beingModified(false) {
    // projection.perspective(45.0f,  (float) w / (float)h, 0.1f, 100.0f);
    projection.ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.0f);
    QMatrix4x4 view;
    view.lookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    projection *= view;
}

void Scene::add(std::shared_ptr<ShadedRenderableObject> object) {
    beingModified = true;
    objects.push_back(std::move(object));
    beingModified = false;
    cvModifyingCollection.notify_all();
}

void Scene::update() {
    waitUntilModificationDone();

    for (auto &object : objects) {
        object->update();
    }
}

void Scene::draw() {
    waitUntilModificationDone();

    for (auto &object : objects) {
        object->render(projection);
    }
}

void Scene::clear() {
    beingModified = true;
    objects.clear();
    beingModified = false;

    cvModifyingCollection.notify_all();
}

void Scene::waitUntilModificationDone() {
    std::mutex m;
    std::unique_lock<std::mutex> l(m);
    cvModifyingCollection.wait(l, [this]() { return !beingModified; });
}

void Scene::remove(const std::shared_ptr<ShadedRenderableObject> &object) {
    beingModified = true;
    std::remove(objects.begin(), objects.end(), object);
    beingModified = false;
}
