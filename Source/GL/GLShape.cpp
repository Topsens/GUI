#include "GLShape.h"
#include <cstring>
#include <vector>

using namespace std;

GLShape::GLShape()
    : vbo(0), cbo(0), nbo(0), vertexDirty(false), normalDirty(false), coordinateDirty(false)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

GLShape::~GLShape()
{
    this->Release();
}

void GLShape::Vertices(const Vertex* vertices, int count)
{
    this->vertices.clear();

    if (vertices && count > 0)
    {
        this->vertices.resize(count);
        memcpy(&this->vertices[0], vertices, count * sizeof(vertices[0]));
    }

    this->vertexDirty = true;
}

void GLShape::Vertices(const Vertex* vertices, const GLuint* indices, int indexCount)
{
    this->vertices.clear();

    if (vertices && indices && indexCount > 0)
    {
        this->vertices.reserve(indexCount);

        for (auto i = 0; i < indexCount; i++)
        {
            this->vertices.push_back(vertices[indices[i]]);
        }
    }

    this->vertexDirty = true;
}

void GLShape::Normals(const Normal* normals, int count)
{
    this->normals.clear();

    if (normals && count > 0)
    {
        this->normals.resize(count);
        memcpy(&this->normals[0], normals, count * sizeof(normals[0]));
    }

    this->normalDirty = true;
}

void GLShape::Normals(const Normal* normals, const GLuint* indices, int indexCount)
{
    this->normals.clear();

    if (normals && indices && indexCount > 0)
    {
        this->normals.reserve(indexCount);

        for (auto i = 0; i < indexCount; i++)
        {
            this->normals.push_back(normals[indices[i]]);
        }
    }

    this->normalDirty = true;
}

void GLShape::TexCoords(const Coordinate* coords, int count)
{
    this->coordinates.clear();

    if (coords && count > 0)
    {
        this->coordinates.resize(count);
        memcpy(&this->coordinates[0], coords, count * sizeof(coords));
    }

    this->coordinateDirty = true;
}

void GLShape::TexCoords(const Coordinate* coords, const GLuint* indices, int indexCount)
{
    this->coordinates.clear();

    if (coords && indices && indexCount > 0)
    {
        this->coordinates.reserve(indexCount);

        for (auto i = 0; i < indexCount; i++)
        {
            this->coordinates.push_back(coords[indices[i]]);
        }
    }

    this->coordinateDirty = true;
}

GLTexture& GLShape::Texture()
{
    return this->texture;
}

GLMaterial& GLShape::Material()
{
    return this->material;
}

void GLShape::Render()
{
    auto count = this->ApplyVertices() / 3;
    this->ApplyNormals();
    this->ApplyCoordinates();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->texture.Apply();
    this->material.Apply();

    glPushMatrix();

    glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
    glRotatef(this->Rotation[0], 1.f, 0.f, 0.f);
    glRotatef(this->Rotation[1], 0.f, 1.f, 0.f);
    glRotatef(this->Rotation[2], 0.f, 0.f, 1.f);
    glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

    glDrawArrays(GL_TRIANGLES, 0, count);

    glPopMatrix();

    this->material.Revoke();
    this->texture.Revoke();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLShape::Release()
{
    if (this->nbo)
    {
        glDeleteBuffers(1, &this->nbo);
        this->nbo = 0;
    }

    if (this->cbo)
    {
        glDeleteBuffers(1, &this->cbo);
        this->cbo = 0;
    }

    if (this->vbo)
    {
        glDeleteBuffers(1, &this->vbo);
        this->vbo = 0;
    }
}

GLint GLShape::ApplyVertices()
{
    if (!this->vbo)
    {
        glGenBuffers(1, &this->vbo);
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    if (this->vertexDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices[0]) * this->vertices.size(), this->vertices.size() ? this->vertices.data() : 0, GL_STATIC_DRAW);
        this->vertices.clear();
        this->vertexDirty = false;
    }

    GLint count;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &count);

    if (count)
    {
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
    }

    return count;
}

GLint GLShape::ApplyNormals()
{
    if (!this->nbo)
    {
        glGenBuffers(1, &this->nbo);
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->nbo);
    if (this->normalDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->normals[0]) * this->normals.size(), this->normals.size() ? this->normals.data() : 0, GL_STATIC_DRAW);
        this->normals.clear();
        this->normalDirty = false;
    }

    GLint count;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &count);

    if (count)
    {
        glNormalPointer(GL_FLOAT, 0, 0);
        glEnableClientState(GL_NORMAL_ARRAY);
    }

    return count;
}

GLint GLShape::ApplyCoordinates()
{
    if (!this->cbo)
    {
        glGenBuffers(1, &this->cbo);
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->cbo);
    if (this->coordinateDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->coordinates[0]) * this->coordinates.size(), this->coordinates.size() ? this->coordinates.data() : 0, GL_STATIC_DRAW);
        this->coordinates.clear();
        this->coordinateDirty = false;
    }

    GLint count;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &count);

    if (count)
    {
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    return count;
}