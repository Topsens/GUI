#include "GLTexture.h"

GLTexture::GLTexture()
  : tex(0), envMode(GL_MODULATE), minFilter(GL_LINEAR), magFilter(GL_LINEAR), wrapS(GL_REPEAT), wrapT(GL_REPEAT)
{
}

bool GLTexture::Create()
{
    this->Release();
    glGenTextures(1, &this->tex);
    return !!this->tex;
}

void GLTexture::Release()
{
    if (this->tex)
    {
        glDeleteTextures(1, &this->tex);
        this->tex = 0;
    }
}

void GLTexture::Mode(GLuint envMode)
{
    this->envMode = envMode;
}

void GLTexture::Filter(GLuint minFilter, GLuint magFilter)
{
    this->minFilter = minFilter;
    this->magFilter = magFilter;
}

void GLTexture::Wrap(GLuint wrapS, GLuint wrapT)
{
    this->wrapS = wrapS;
    this->wrapT = wrapT;
}

bool GLTexture::Data(const unsigned char* pixels, int width, int height, int size, GLenum format)
{
    if (!pixels || width <= 0 || height <= 0 || size <= 0)
    {
        return false;
    }

    if (!this->tex && !this->Create())
    {
        return false;
    }

    GLint internalFormat;
    switch (format)
    {
        case GL_RGB:
        case GL_BGR:
        {
            internalFormat = GL_RGB;
            break;
        }

        case GL_RGBA:
        case GL_BGRA:
        {
            internalFormat = GL_RGBA;
            break;
        }

        default:
            return false;
    }

    glBindTexture(GL_TEXTURE_2D, this->tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    return true;
}

void GLTexture::Apply()
{
    if (this->tex)
    {
        glBindTexture(GL_TEXTURE_2D, this->tex);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->envMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
        glEnable(GL_TEXTURE_2D);
    }
}

void GLTexture::Revoke()
{
    if (this->tex)
    {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}