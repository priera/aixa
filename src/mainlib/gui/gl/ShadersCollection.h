#ifndef AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H
#define AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H

namespace ShadersCollection {
namespace Vertex {
static constexpr auto TEXTURED_PLANE = "./shaders/textured_plane.vert";
static constexpr auto FRONT_CHARACTER = "./shaders/front_character.vert";
}  // namespace Vertex

namespace Fragment {
static constexpr auto TEXTURE_2D = "./shaders/2d_texture.frag";
static constexpr auto CHARACTER = "./shaders/character.frag";
}  // namespace Fragment
}  // namespace ShadersCollection

#endif  // AIXA_SRC_MAINLIB_GUI_GL_SHADERSCOLLECTION_H
