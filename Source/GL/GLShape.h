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

    bool Indices(const Index* indices, int count);
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
    virtual GLint ApplyVertices();
    virtual GLint ApplyNormals();
    virtual GLint ApplyTexCoords();
    virtual void  ApplyTexture();
    virtual void  ApplyProgram();

    virtual void RevokeVertices();
    virtual void RevokeNormals();
    virtual void RevokeTexCoords();
    virtual void RevokeTexture();

    GLenum mode;
    GLBuffer ebo, vbo, nbo, cbo;
    GLTexture  texture;
    GLMaterial material;

    GLShape* parent;
    std::vector<GLShape*> children;
};
