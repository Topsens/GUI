#pragma once

#include <gl/glew.h>
#include <vector>

class GLTexture
{
public:
    GLTexture();

    bool Create();

    void Mode(GLuint envMode);
    void Filter(GLuint minFilter, GLuint magFilter);
    void Wrap(GLuint wrapS, GLuint wrapT);
    bool Data(const unsigned char* pixels, int width, int height, int size, GLenum format);
    void Apply();
    void Revoke();
    void Release();

    operator bool() const
    {
        return !!this->tex;
    }
    operator GLuint() const
    {
        return this->tex;
    }

private:
    GLuint envMode;
    GLuint minFilter;
    GLuint magFilter;
    GLuint wrapS;
    GLuint wrapT;

    GLuint tex;
};