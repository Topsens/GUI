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

    void Vertices(const Vertex* vertices, int count);
    void Vertices(const Vertex* vertices, const GLuint* indices, int indexCount);
    void Normals(const Normal* normals, int count);
    void Normals(const Normal* normals, const GLuint* indices, int indexCount);
    void TexCoords(const Coordinate* coords, int count);
    void TexCoords(const Coordinate* coords, const GLuint* indices, int indexCount);

    GLTexture&  Texture();
    GLMaterial& Material();

    virtual void Render();
    virtual void Release();

protected:
    GLint ApplyVertices();
    GLint ApplyNormals();
    GLint ApplyCoordinates();

    int textureMode;
    GLuint vbo, cbo, nbo;
    GLTexture  texture;
    GLMaterial material;

    std::vector<Vertex>     vertices;
    std::vector<Normal>     normals;
    std::vector<Coordinate> coordinates;

    bool vertexDirty;
    bool normalDirty;
    bool coordinateDirty;
};
