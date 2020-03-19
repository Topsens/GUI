#include "GLTexture.h"

GLTexture::GLTexture()
  : tex(0), envMode(GL_MODULATE), minFilter(GL_LINEAR), magFilter(GL_LINEAR), wrapS(GL_REPEAT), wrapT(GL_REPEAT)
{
}

GLTexture::~GLTexture()
{
    this->Release();
}

GLTexture::operator bool() const
{
    return this->tex ? true : false;
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


bool GLTexture::Set(const unsigned char* pixels, int width, int height, int size, GLenum format)
{
    if (!pixels || width <= 0 || height <= 0 || size <= 0)
    {
        return false;
    }

    if (!this->tex)
    {
        glGenTextures(1, &this->tex);

        if (!this->tex)
        {
            return false;
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->envMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->magFilter);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void GLTexture::Clear()
{
    if (this->tex)
    {
        glDeleteTextures(1, &this->tex);
        this->tex = 0;
    }
}

void GLTexture::Apply()
{
    if (this->tex)
    {
        glBindTexture(GL_TEXTURE_2D, this->tex);
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

void GLTexture::Release()
{
    this->Clear();
}