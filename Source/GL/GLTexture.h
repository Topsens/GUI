#pragma once

#include <gl/glew.h>
#include <vector>

class GLTexture
{
public:
    GLTexture();
   ~GLTexture();

    void Params(int textureMode, int filterModeMag, int filterModeMin, int wrapModeS, int wrapModeT);
    void Set(const unsigned int* pixels, int width, int height);
    void Clear();
    void Apply();
    void Revoke();

    int  Width() const;
    int  Height() const;

private:
    int textureMode;
    int filterModeMag;
    int filterModeMin;
    int wrapModeS;
    int wrapModeT;
    int width, height;

    GLuint tex;

    std::vector<unsigned char> pixels;
};