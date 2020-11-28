#ifndef AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H
#define AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H

#include <string>

class ShadersCollection {
public:
    ShadersCollection() = delete;

    static constexpr auto VERTEX_TEXTURED_PLANE = "./shaders/textured_plane.vert";
    static constexpr auto FRAG_2D_TEXTURE = "./shaders/2d_texture.frag";
    static constexpr auto FRAG_CHARACTER = "./shaders/character.frag";
};

#endif //AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H
