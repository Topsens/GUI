#pragma once
#include "GLObject.h"
#include "GLTexture.h"
#include "GLMaterial.h"
#include <vector>

class GLShape : public GLObject
{
public:
    GLShape();
   ~GLShape();

    bool Indices(const GLuint* indices, int count);
    bool Vertices(const Vertex* vertices, int count);
    bool Normals(const Normal* normals, int count);
    bool TexCoords(const Coordinate* coords, int count);

    GLTexture&  Texture();
    GLMaterial& Material();

    virtual void Render();
    virtual void Release();

protected:
    GLint ApplyIndices();
    GLint ApplyVertices();
    GLint ApplyNormals();
    GLint ApplyTexCoords();

    void RevokeIndices();
    void RevokeVertices();
    void RevokeNormals();
    void RevokeTexCoords();

    int textureMode;
    GLuint ibo, vbo, nbo, cbo;
    GLTexture  texture;
    GLMaterial material;
};
