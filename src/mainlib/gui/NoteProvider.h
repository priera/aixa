#ifndef ALSAPLAYGROUND_NOTEPROVIDER_H
#define ALSAPLAYGROUND_NOTEPROVIDER_H

#include <map>

#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>

#include "mainlib/gui/NoteRenderable.h"

class RenderableObject;

class NoteProvider : protected QOpenGLExtraFunctions {
public:
    NoteProvider(const QMatrix4x4 &projectionMatrix);

    RenderableObject *generateNote(char note);

private:
    QMatrix4x4 projectionMatrix;

    std::map<char, NoteRenderable::Character> characters;
};


#endif //ALSAPLAYGROUND_NOTEPROVIDER_H
