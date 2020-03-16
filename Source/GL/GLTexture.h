#pragma once

#include <gl/glew.h>
#include <vector>

class GLTexture
{
public:
    GLTexture();
   ~GLTexture();

    operator bool() const;

    void Params(int textureMode, int filterModeMag, int filterModeMin, int wrapModeS, int wrapModeT);
    bool Set(const unsigned char* pixels, int width, int height, int size, GLenum format);
    void Clear();
    void Apply();
    void Revoke();
    void Release();

private:
    int textureMode;
    int filterModeMag;
    int filterModeMin;
    int wrapModeS;
    int wrapModeT;

    GLuint tex;
};