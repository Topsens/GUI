#include "GLCloud.h"

GLCloud::GLCloud() : vbo(0), cbo(0), pointSize(1.f), cloudDirty(false), coordinateDirty(false)
{
    this->Position = { 0.f, 0.f, 0.f };
    this->Rotation = { 0.f, 0.f, 0.f };
    this->Scaling  = { 1.f, 1.f, 1.f };
}

GLCloud::~GLCloud()
{
}

void GLCloud::Cloud(const Vertex* vertices, int count)
{
    this->cloud.clear();

    if (vertices && count > 0)
    {
        this->cloud.resize(count);
        memcpy(&this->cloud[0], vertices, this->cloud.size() * sizeof(this->cloud[0]));
    }

    this->cloudDirty = true;
}

void GLCloud::TexCoords(const Coordinate* coordinates, int count)
{
    this->coordinates.clear();

    if (coordinates && count > 0)
    {
        this->coordinates.resize(count);
        memcpy(&this->coordinates[0], coordinates, this->coordinates.size() * sizeof(this->coordinates[0]));
    }

    this->coordinateDirty = true;
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
    auto count = this->ApplyCloud();
    this->ApplyCoordinates();

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    this->texture.Apply();

    glPushMatrix();

    glTranslatef(this->Position[0], this->Position[1], this->Position[2]);
    glRotatef(this->Rotation[0], 1.f, 0.f, 0.f);
    glRotatef(this->Rotation[1], 0.f, 1.f, 0.f);
    glRotatef(this->Rotation[2], 0.f, 0.f, 1.f);
    glScalef(this->Scaling[0], this->Scaling[1], this->Scaling[2]);

    glPointSize(this->pointSize);
    glDrawArrays(GL_POINTS, 0, count);

    glPopMatrix();

    this->texture.Revoke();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

GLint GLCloud::ApplyCloud()
{
    if (!this->vbo)
    {
        glGenBuffers(1, &this->vbo);
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    if (this->cloudDirty)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->cloud[0]) * this->cloud.size(), this->cloud.size() ? this->cloud.data() : 0, GL_STATIC_DRAW);
        this->cloud.clear();
        this->cloudDirty = false;
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

GLint GLCloud::ApplyCoordinates()
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