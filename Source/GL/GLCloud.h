#pragma once

#include "GLObject.h"
#include "GLBuffer.h"
#include "GLTexture.h"
#include <vector>

class GLCloud : public GLObject
{
public:
    GLCloud();

    bool Vertices(const Vertex* vertices, int count);
    bool TexCoords(const Coordinate* coordinates, int count);
    void ClearVertices();
    void ClearTexCoords();
    void Release();

    GLTexture& Texture();

    float PointSize() const;
    void  PointSize(float size);

    virtual void Render();

protected:
    virtual GLint ApplyVertices();
    virtual void  RevokeVertices();
    virtual GLint ApplyTexCoords();
    virtual void  RevokeTexCoords();

protected:
    float pointSize;
    GLBuffer vbo, cbo;
    GLTexture texture;
};