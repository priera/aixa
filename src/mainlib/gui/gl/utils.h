#ifndef AIXA_SRC_MAINLIB_GUI_GL_UTILS_H
#define AIXA_SRC_MAINLIB_GUI_GL_UTILS_H

#include <QOpenGLFunctions>

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif //AIXA_SRC_MAINLIB_GUI_GL_UTILS_H
