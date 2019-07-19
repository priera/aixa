#ifndef ALSAPLAYGROUND_NOTEDRAWER_H
#define ALSAPLAYGROUND_NOTEDRAWER_H

#include <vector>
#include <map>
#include <memory>

#include <QOpenGLExtraFunctions>

#include "mainlib/audio/note/Note.h"

class QOpenGLShaderProgram;

class NoteDrawer : protected QOpenGLExtraFunctions {
public:
    void init();

    void drawNote(char note, const QMatrix4x4 &projection);

private:
    struct Character {
        GLuint textureID;   // ID handle of the glyph texture
        std::vector<unsigned int> size;    // Size of glyph
        std::vector<int> bearing;  // Offset from baseline to left/top of glyph
        long int advance;    // Horizontal offset to advance to next glyph
    };

    unsigned int VBO, VAO;

    std::map<char, Character> characters;

    std::unique_ptr<QOpenGLShaderProgram> program;
};


#endif //ALSAPLAYGROUND_NOTEDRAWER_H
