#include "mainlib/gui/NoteDrawer.h"

#include <set>
#include <cctype>

#include <QOpenGLShaderProgram>

#include "mainlib/gui/bitmap/FreeTypeCharacterBitmapProvider.h"


void NoteDrawer::init() {
    FreeTypeCharacterBitmapProvider provider;

    std::set<char> pitchChars = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };

    program = std::make_unique<QOpenGLShaderProgram>();

    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "./src/mainlib/gui/shaders/vertex.glsl");
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "./src/mainlib/gui/shaders/fragment.glsl");

    program->link();

    for (auto &p: pitchChars) {
        auto bitmapData = provider.getCharacter(p);

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                bitmapData.width,
                bitmapData.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                bitmapData.buffer
        );

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
                texture,
                {bitmapData.width, bitmapData.rows},
                {bitmapData.bitmap_left, bitmapData.bitmap_top},
                bitmapData.advance
        };

        characters[p] = character;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void NoteDrawer::drawNote(char note, const QMatrix4x4 &projection) {
    char upperNote = toupper(note);

    auto it = characters.find(upperNote);
    if (it == characters.end())
        return;

    auto & ch = it->second;

    program->bind();

    program->setUniformValue("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    GLfloat xpos = ch.bearing[0];
    GLfloat ypos = (ch.size[1] - ch.bearing[1]);

    GLfloat w = ch.size[0];
    GLfloat h = ch.size[1];
    // Update VBO for each character

    GLfloat vertices[6][4] = {
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },

            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 },
            { xpos,     ypos + h,   0.0, 0.0 }
    };

    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    program->release();
}
