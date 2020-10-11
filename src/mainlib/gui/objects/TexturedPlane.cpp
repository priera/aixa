#include "TexturedPlane.h"

TexturedPlane::TexturedPlane(std::filesystem::path texturePath) :
    ShadedRenderableObject("./shaders/textured_plane.vert", "/shaders/2d_texture.frag"),
    texturePath(std::move(texturePath)) { }

void TexturedPlane::init() {
    //TODO
}

void TexturedPlane::doMyRender() {
    //TODO
}

