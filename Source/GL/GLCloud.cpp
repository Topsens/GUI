#include "GLCloud.h"

GLCloud::GLCloud() : vbo(GL_ARRAY_BUFFER), cbo(GL_ARRAY_BUFFER), pointSize(1.f)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

bool GLCloud::Vertices(const Vertex* vertices, int count)
{
    if (!vertices || count <= 0)
    {
        return false;
    }

    this->vbo.Data(vertices, count * sizeof(*vertices), GL_STATIC_DRAW);

    return true;
}

bool GLCloud::TexCoords(const Coordinate* coordinates, int count)
{
    if (!coordinates || count <= 0)
    {
        return false;
    }

    this->cbo.Data(coordinates, count * sizeof(*coordinates), GL_STATIC_DRAW);

    return true;
}

void GLCloud::ClearVertices()
{
    this->vbo.Release();
}

void GLCloud::ClearTexCoords()
{
    this->cbo.Release();
}

void GLCloud::Release()
{
    this->ClearVertices();
    this->ClearTexCoords();
    this->texture.Release();
}

GLTexture& GLCloud::Texture()
{
    return this->texture;
}

float GLCloud::PointSize() const
{
    return this->pointSize;
}

void GLCloud::PointSize(float size)
{
    this->pointSize = size;
}

void GLCloud::Render()
{
    auto count = this->ApplyVertices();
    if (count)
    {
        this->ApplyTexCoords();
        this->texture.Apply();

        glPushMatrix();

        glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
        glRotatef(this->Rotation[3], this->Rotation[0], this->Rotation[1], this->Rotation[2]);
        glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

        glPointSize(this->pointSize);
        glDrawArrays(GL_POINTS, 0, count);

        glPopMatrix();

        this->texture.Revoke();
        this->RevokeTexCoords();
        this->RevokeVertices();
    }
}

GLint GLCloud::ApplyVertices()
{
    if (this->vbo)
    {
        this->vbo.Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
        glEnableVertexAttribArray(0);
        return this->vbo.Size() / sizeof(Vertex);
    }
    
    return 0;
}

void GLCloud::RevokeVertices()
{
    if (this->vbo)
    {
        this->vbo.Bind();
        glDisableVertexAttribArray(0);
    }
}

GLint GLCloud::ApplyTexCoords()
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

void GLCloud::RevokeTexCoords()
{
    if (this->cbo)
    {
        this->cbo.Bind();
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}