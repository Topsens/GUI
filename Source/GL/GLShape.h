#pragma once
#include "GLObject.h"
#include "GLBuffer.h"
#include "GLTexture.h"
#include "GLMaterial.h"
#include <vector>

class GLShape : public GLObject
{
public:
    GLShape();
    virtual ~GLShape();

    bool Indices(const GLuint* indices, int count);
    bool Vertices(const Vertex* vertices, int count);
    bool Normals(const Normal* normals, int count);
    bool TexCoords(const Coordinate* coords, int count);

    GLenum Mode();
    void Mode(GLenum);

    GLTexture&  Texture();
    GLMaterial& Material();

    virtual void Render();
    virtual void Release();

    bool HasChild() const;
    bool HasChild(const GLShape* child) const;
    void AddChild(GLShape* child);
    void RemoveChild(const GLShape* child);

protected:
    GLint ApplyIndices();
    GLint ApplyVertices();
    GLint ApplyNormals();
    GLint ApplyTexCoords();
    void  ApplyTexture();

    void RevokeIndices();
    void RevokeVertices();
    void RevokeNormals();
    void RevokeTexCoords();
    void RevokeTexture();

    GLenum mode;
    GLBuffer ibo, vbo, nbo, cbo;
    GLTexture  texture;
    GLMaterial material;

    GLShape* parent;
    std::vector<GLShape*> children;
};
