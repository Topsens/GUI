#include "GLTexture.h"

GLTexture::GLTexture()
  : tex(0), textureMode(GL_MODULATE), filterModeMag(GL_LINEAR), filterModeMin(GL_LINEAR), wrapModeS(GL_CLAMP), wrapModeT(GL_CLAMP)
{
}

GLTexture::~GLTexture()
{
    if (this->tex)
    {
        glDeleteTextures(1, &this->tex);
    }
}

void GLTexture::Params(int textureMode, int filterModeMag, int filterModeMin, int wrapModeS, int wrapModeT)
{
    this->textureMode = textureMode;
    this->filterModeMag = filterModeMag;
    this->filterModeMin = filterModeMin;
    this->wrapModeS = wrapModeS;
    this->wrapModeT = wrapModeT;
}

void GLTexture::Set(const unsigned int* pixels, int width, int height)
{
    if (pixels && width > 0 && height > 0)
    {
        this->pixels.clear();
        this->pixels.resize(width * height * sizeof(pixels[0]));
        memcpy(&this->pixels[0], pixels, width * height * sizeof(pixels[0]));

        this->width  = width;
        this->height = height;
    }
}

void GLTexture::Clear()
{
    this->pixels.clear();
    this->width  = 0;
    this->height = 0;
}

void GLTexture::Apply()
{
    if (this->pixels.size())
    {
        if (!this->tex)
        {
            glGenTextures(1, &this->tex);
        }

        glBindTexture(GL_TEXTURE_2D, this->tex);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, this->textureMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterModeMag);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterModeMin);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->pixels.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        this->pixels.clear();
    }

    if (this->tex)
    {
        glBindTexture(GL_TEXTURE_2D, this->tex);
        glEnable(GL_TEXTURE_2D);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}

void GLTexture::Revoke()
{
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int GLTexture::Width() const
{
    return this->width;
}

int GLTexture::Height() const
{
    return this->height;
}