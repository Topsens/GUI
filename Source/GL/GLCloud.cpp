#include "GLCloud.h"

GLCloud::GLCloud() : vbo(GL_ARRAY_BUFFER), nbo(GL_ARRAY_BUFFER), cbo(GL_ARRAY_BUFFER), pointSize(1.f)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

bool GLCloud::Vertices(const Vertex* vertices, size_t count)
{
    if (!vertices && count)
    {
        return false;
    }

    this->vbo.Data(vertices, count * sizeof(*vertices), GL_STATIC_DRAW);

    return true;
}

bool GLCloud::Normals(const Normal* normals, size_t count)
{
    if (!normals && count)
    {
        return false;
    }

    this->nbo.Data(normals, count * sizeof(*normals), GL_STATIC_DRAW);

    return true;
}

bool GLCloud::TexCoords(const Coordinate* coordinates, size_t count)
{
    if (!coordinates && count)
    {
        return false;
    }

    this->cbo.Data(coordinates, count * sizeof(*coordinates), GL_STATIC_DRAW);

    return true;
}

void GLCloud::Release()
{
    this->vbo.Release();
    this->nbo.Release();
    this->cbo.Release();
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
        this->ApplyNormals();
        this->ApplyTexCoords();
        this->texture.Apply();

        glPushMatrix();

        glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
        glRotatef(this->Rotation[3], this->Rotation[0], this->Rotation[1], this->Rotation[2]);
        glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

        glPointSize(this->pointSize);
        glDrawArrays(GL_POINTS, 0, (GLsizei)count);

        glPopMatrix();

        this->texture.Revoke();
        this->RevokeTexCoords();
        this->RevokeNormals();
        this->RevokeVertices();
    }
}

size_t GLCloud::ApplyVertices()
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

size_t GLCloud::ApplyNormals()
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

size_t GLCloud::ApplyTexCoords()
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

void GLCloud::RevokeVertices()
{
    if (this->vbo)
    {
        this->vbo.Bind();
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void GLCloud::RevokeNormals()
{
    if (this->nbo)
    {
        this->nbo.Bind();
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void GLCloud::RevokeTexCoords()
{
    if (this->cbo)
    {
        this->cbo.Bind();
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}