#pragma once
#include <vector>
#include <Windows.h>
#include <gdiplus.h>

class Texture
{
public:
    Texture(const wchar_t* file);
   ~Texture();

   const unsigned char* Pixels() const;
   int Width() const;
   int Height() const;

private:
    int width, height;
    std::vector<unsigned char> pixels;
};