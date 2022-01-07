#pragma once

#include "GLCommon.h"
#include <gl/glew.h>

class GLBuffer
{
public:
    GLBuffer(GLenum target);

    bool Create();
    void Release();

    void Bind() const;
    bool Data(const void* data, GLsizeiptr size, GLenum usage);
    GLint Size() const;

    GLenum Target() const
    {
        return this->target;
    }

    operator bool() const
    {
        return !!this->buffer;
    }

    operator GLuint() const
    {
        return this->buffer;
    }

private:
    GLuint buffer;
    GLenum target;
};