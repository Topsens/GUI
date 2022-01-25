#pragma once

#include "GLObject.h"
#include "GLBuffer.h"
#include "GLTexture.h"
#include <vector>

class GLCloud : public GLObject
{
public:
    GLCloud();

    bool Vertices(const Vertex* vertices, size_t count);
    bool Normals(const Normal* normals, size_t count);
    bool TexCoords(const Coordinate* coordinates, size_t count);
    void Release();

    GLTexture& Texture();

    float PointSize() const;
    void  PointSize(float size);

    virtual void Render();

protected:
    virtual size_t ApplyVertices();
    virtual size_t ApplyNormals();
    virtual size_t ApplyTexCoords();
    virtual void   RevokeVertices();
    virtual void   RevokeNormals();
    virtual void   RevokeTexCoords();

protected:
    float pointSize;
    GLBuffer vbo, nbo, cbo;
    GLTexture texture;
};