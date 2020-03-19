#pragma once

#include <gl/glew.h>
#include <vector>

class GLTexture
{
public:
    GLTexture();
   ~GLTexture();

    operator bool() const;

    void Mode(GLuint envMode);
    void Filter(GLuint minFilter, GLuint magFilter);
    void Wrap(GLuint wrapS, GLuint wrapT);
    bool Set(const unsigned char* pixels, int width, int height, int size, GLenum format);
    void Clear();
    void Apply();
    void Revoke();
    void Release();

private:
    GLuint envMode;
    GLuint minFilter;
    GLuint magFilter;
    GLuint wrapS;
    GLuint wrapT;

    GLuint tex;
};