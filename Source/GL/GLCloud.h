#pragma once

#include "GLObject.h"
#include "GLTexture.h"
#include <vector>

class GLCloud : public GLObject
{
public:
    GLCloud();
    virtual ~GLCloud();

    void Cloud(const Vertex* vertices, int count);
    void TexCoords(const Coordinate* coordinates, int count);

    GLTexture& Texture();

    float PointSize() const;
    void  PointSize(float size);

    virtual void Render();

protected:
    GLint ApplyCloud();
    GLint ApplyCoordinates();

protected:
    float pointSize;
    GLuint vbo, cbo;
    GLTexture texture;
    std::vector<Vertex> cloud;
    std::vector<Coordinate> coordinates;

    bool cloudDirty;
    bool coordinateDirty;
};