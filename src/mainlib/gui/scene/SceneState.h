#ifndef AIXA_SRC_MAINLIB_GUI_SCENE_SCENESTATE_H
#define AIXA_SRC_MAINLIB_GUI_SCENE_SCENESTATE_H

#include <mainlib/gui/texture/TextureCollection.h>

#include "Scene.h"

class SceneState {
public:
    SceneState(Scene& scene, TextureCollection& textureCollection, BitmapBuilders& bitmapBuilders);
    virtual ~SceneState() noexcept = default;

    void showUXControls();
    void showAudioVisualizations();

private:
    Scene* scene;
    TextureCollection* textureCollection;
    BitmapBuilders* bitmapBuilders;
};

#endif  // AIXA_SRC_MAINLIB_GUI_SCENE_SCENESTATE_H
