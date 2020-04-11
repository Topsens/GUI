#include "Texture.h"

using namespace std;
using namespace Gdiplus;

Texture::Texture(const wchar_t* file)
  : width(0), height(0)
{
    auto bmp = Bitmap::FromFile(file);

    if (bmp)
    {
        this->width  = bmp->GetWidth();
        this->height = bmp->GetHeight();

        auto data = new BitmapData();
        bmp->LockBits(nullptr, ImageLockModeRead, PixelFormat32bppARGB, data);

        this->pixels.resize(this->width * this->height * 4);

        for (auto i = 0; i < this->height; i++)
        {
            memcpy(&this->pixels[i * this->width * 4], (unsigned char*)data->Scan0 + data->Stride * i, this->width * 4);
        }

        bmp->UnlockBits(data);
        delete data;
        delete bmp;
    }
}

Texture::~Texture()
{
}

const unsigned char* Texture::Pixels() const
{
    if (this->pixels.size())
    {
        return this->pixels.data();
    }

    return nullptr;
}

int Texture::Width() const
{
    return this->width;
}

int Texture::Height() const
{
    return this->height;
}