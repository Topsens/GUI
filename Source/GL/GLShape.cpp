#include "GLShape.h"
#include <cstring>
#include <vector>

using namespace std;

GLShape::GLShape() : ebo(GL_ELEMENT_ARRAY_BUFFER), vbo(GL_ARRAY_BUFFER), nbo(GL_ARRAY_BUFFER), cbo(GL_ARRAY_BUFFER), mode(GL_TRIANGLES), parent(nullptr)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

GLShape::~GLShape()
{
    this->Release();
}

bool GLShape::Elements(const Element* elements, size_t count)
{
    if (!elements || !count)
    {
        return false;
    }

    return this->ebo.Data(elements, count * sizeof(*elements), GL_STATIC_DRAW);
}

bool GLShape::Vertices(const Vertex* vertices, size_t count)
{
    if (!vertices || !count)
    {
        return false;
    }

    return this->vbo.Data(vertices, count * sizeof(*vertices), GL_STATIC_DRAW);
}

bool GLShape::Normals(const Normal* normals, size_t count)
{
    if (!normals || !count)
    {
        return false;
    }

    return this->nbo.Data(normals, count * sizeof(*normals), GL_STATIC_DRAW);
}

bool GLShape::TexCoords(const Coordinate* coords, size_t count)
{
    if (!coords || !count)
    {
        return false;
    }

    return this->cbo.Data(coords, count * sizeof(*coords), GL_STATIC_DRAW);
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
        this->ApplyProgram();

        this->material.Apply();

        auto ec = this->ebo.Size() / sizeof(GLuint);
        if (ec)
        {
            glDrawElements(this->mode, (GLsizei)ec, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(this->mode, 0, (GLsizei)vc);
        }

        this->material.Revoke();
    
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

    this->nbo.Release();
    this->cbo.Release();
    this->vbo.Release();
    this->ebo.Release();
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

size_t GLShape::ApplyVertices()
{
    if (this->vbo)
    {
        this->vbo.Bind();
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glEnableClientState(GL_VERTEX_ARRAY);
        return this->vbo.Size() / sizeof(Vertex);
    }

    return 0;
}

size_t GLShape::ApplyNormals()
{
    if (this->nbo)
    {
        this->nbo.Bind();
        glNormalPointer(GL_FLOAT, 0, 0);
        glEnableClientState(GL_NORMAL_ARRAY);
        return this->nbo.Size() / sizeof(Normal);
    }

    return 0;
}

size_t GLShape::ApplyTexCoords()
{
    if (this->cbo)
    {
        this->cbo.Bind();
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        return this->cbo.Size() / sizeof(Coordinate);
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

void GLShape::ApplyProgram()
{
}

void GLShape::RevokeVertices()
{
    if (this->vbo)
    {
        this->vbo.Bind();
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void GLShape::RevokeNormals()
{
    if (this->nbo)
    {
        this->nbo.Bind();
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void GLShape::RevokeTexCoords()
{
    if (this->cbo)
    {
        this->cbo.Bind();
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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