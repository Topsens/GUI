#include "GLShape.h"
#include <cstring>
#include <vector>

using namespace std;

GLShape::GLShape() : ibo(0), vbo(0), nbo(0), cbo(0), mode(GL_TRIANGLES), parent(nullptr)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

GLShape::~GLShape()
{
    this->Release();
}

bool GLShape::Indices(const GLuint* indices, int count)
{
    if (!indices || count <= 0)
    {
        return false;
    }

    if (this->ibo)
    {
        glDeleteBuffers(1, &this->ibo);
    }

    glGenBuffers(1, &this->ibo);
    if (!this->ibo)
    {
        return false;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(*indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return true;
}

bool GLShape::Vertices(const Vertex* vertices, int count)
{
    if (!vertices || count <= 0)
    {
        return false;
    }

    if (this->vbo)
    {
        glDeleteBuffers(1, &this->vbo);
    }

    glGenBuffers(1, &this->vbo);
    if (!this->vbo)
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(*vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

bool GLShape::Normals(const Normal* normals, int count)
{
    if (!normals || count <= 0)
    {
        return false;
    }

    if (this->nbo)
    {
        glDeleteBuffers(1, &this->nbo);
    }

    glGenBuffers(1, &this->nbo);
    if (!this->nbo)
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->nbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(*normals), normals, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

bool GLShape::TexCoords(const Coordinate* coords, int count)
{
    if (!coords || count <= 0)
    {
        return false;
    }

    if (this->cbo)
    {
        glDeleteBuffers(1, &this->cbo);
    }

    glGenBuffers(1, &this->cbo);
    if (!this->cbo)
    {
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->cbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(*coords), coords, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

GLenum GLShape::Mode()
{
    return this->mode;
}

void GLShape::Mode(GLenum mode)
{
    switch (mode)
    {
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
            this->mode = mode;
        default:
            break;
    }
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
    glPushMatrix();

    glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
    glRotatef(this->Rotation[3], this->Rotation[0], this->Rotation[1], this->Rotation[2]);
    glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

    auto vc = this->ApplyVertices();
    if (vc)
    {
        this->ApplyNormals();
        this->ApplyTexCoords();
        this->ApplyTexture();

        this->material.Apply();

        auto ic = this->ApplyIndices();
        if (ic)
        {
            glDrawElements(this->mode, ic, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(this->mode, 0, vc);
        }

        this->material.Revoke();
    
        this->RevokeIndices();
        this->RevokeTexture();
        this->RevokeTexCoords();
        this->RevokeNormals();
        this->RevokeVertices();
    }

    for (auto child : this->children)
    {
        child->Render();
    }

    glPopMatrix();
}

void GLShape::Release()
{
    for (auto& child : this->children)
    {
        child->Release();
    }
    this->children.clear();

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

    if (this->ibo)
    {
        glDeleteBuffers(1, &this->ibo);
        this->ibo = 0;
    }
}

bool GLShape::HasChild() const
{
    return !this->children.empty();
}

bool GLShape::HasChild(const GLShape* child) const
{
    for (auto c : this->children)
    {
        if (c == child)
        {
            return true;
        }
    }

    return false;
}

void GLShape::AddChild(GLShape* child)
{
    if (child)
    {
        child->parent = this;
        this->children.push_back(child);
    }
}

void GLShape::RemoveChild(const GLShape* child)
{
    this->children.erase(std::find(this->children.begin(), this->children.end(), child));
}

GLint GLShape::ApplyIndices()
{
    if (this->ibo)
    {
        GLint size;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        return size / sizeof(GLuint);
    }

    return 0;
}

GLint GLShape::ApplyVertices()
{
    if (this->vbo)
    {
        GLint size;
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return size / sizeof(Vertex);
    }

    return 0;
}

GLint GLShape::ApplyNormals()
{
    if (this->nbo)
    {
        GLint size;
        glBindBuffer(GL_ARRAY_BUFFER, this->nbo);
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glNormalPointer(GL_FLOAT, 0, 0);
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return size / sizeof(Normal);
    }

    return 0;
}

GLint GLShape::ApplyTexCoords()
{
    if (this->cbo)
    {
        GLint size;
        glBindBuffer(GL_ARRAY_BUFFER, this->cbo);
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return size / sizeof(Coordinate);
    }

    return 0;
}

void GLShape::ApplyTexture()
{
    if (this->texture)
    {
        this->texture.Apply();
    }
    else if (this->parent)
    {
        this->parent->ApplyTexture();
    }
}

void GLShape::RevokeIndices()
{
    if (this->ibo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void GLShape::RevokeVertices()
{
    if (this->vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glDisableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLShape::RevokeNormals()
{
    if (this->nbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->nbo);
        glDisableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLShape::RevokeTexCoords()
{
    if (this->cbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, this->cbo);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLShape::RevokeTexture()
{
    if (this->texture)
    {
        this->texture.Revoke();
    }
    else if (this->parent)
    {
        this->parent->RevokeTexture();
    }
}