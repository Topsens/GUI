#include "GLBuffer.h"

GLBuffer::GLBuffer(GLenum target) : buffer(0), target(target)
{
}

bool GLBuffer::Create()
{
    this->Release();

    glGenBuffers(1, &this->buffer);
    return !!this->buffer;
}

void GLBuffer::Release()
{
    if (this->buffer)
    {
        glDeleteBuffers(1, &this->buffer);
        this->buffer = 0;
    }
}

void GLBuffer::Bind() const
{
    glBindBuffer(this->target, this->buffer);
}

bool GLBuffer::Data(const void* data, GLsizeiptr size, GLenum usage)
{
    if (!this->buffer && !this->Create())
    {
        return false;
    }

    this->Bind();

    glBufferData(this->target, size, data, usage);
    
    return true;
}

GLint GLBuffer::Size() const
{
    this->Bind();

    GLint size;
    glGetBufferParameteriv(this->target, GL_BUFFER_SIZE, &size);
    return size;
}