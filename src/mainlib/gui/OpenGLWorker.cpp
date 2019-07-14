#include "mainlib/gui/OpenGLWorker.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QImage>
#include <QtGui/QMatrix4x4>

#include <iostream>
#include <chrono>
#include <cmath>

OpenGLWorker::OpenGLWorker(QOpenGLContext &context) :
    QOpenGLExtraFunctions(),
    context(&context),
    surface(nullptr),
    w(0),
    h(0) {
    QImage original("./data/container.jpg");
    textureImage = std::make_unique<QImage>(original.convertToFormat(QImage::Format_RGB888));

    QImage original2("./data/awesomeface.png");
    happyImage = std::make_unique<QImage>(original2.convertToFormat(QImage::Format_RGB888).mirrored(false, true));
}

OpenGLWorker::~OpenGLWorker() { }

void OpenGLWorker::bindToSurface(QSurface *surface, int w, int h) {
    this->surface = surface;
    this->w = w;
    this->h = h;

    context->makeCurrent(surface);

    initializeOpenGLFunctions();

    program = std::make_unique<QOpenGLShaderProgram>();
    
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, "./src/mainlib/gui/shaders/vertex.glsl");
    program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, "./src/mainlib/gui/shaders/fragment.glsl");

    program->link();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    vertices = {
        // positions           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
    };

    indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    textures.resize(2);
    glGenTextures(2, &textures[0]);

    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureImage->width(), textureImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->constBits());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, happyImage->width(), happyImage->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, happyImage->constBits());
    glGenerateMipmap(GL_TEXTURE_2D);

    program->bind();

    glUniform1i(program->uniformLocation("texture1"), 0);
    glUniform1i(program->uniformLocation("texture2"), 1);

    program->release();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glViewport(0, 0, w, h);
}

void OpenGLWorker::setSize(int w, int h) {
    this->w = w;
    this->h = h;

    glViewport(0, 0, w, h);
}


void OpenGLWorker::draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    program->bind();

    QMatrix4x4 transform;
    transform.translate(0.5f, -0.5f, 0.0f);
    transform.rotate((float)m_frame, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("transform", transform);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    program->release();

    ++m_frame;

    context->swapBuffers(surface);
}