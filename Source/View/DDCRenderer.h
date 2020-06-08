#include "D2DRenderer.h"

class D2DDc
{
public:
    D2DDc(int width, int height)
    {
        BITMAPINFO bmi = {0};
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = width * height * 4;
        bmi.bmiHeader.biXPelsPerMeter = 3780;
        bmi.bmiHeader.biYPelsPerMeter = 3780;
        bmi.bmiHeader.biClrUsed = 0;
        bmi.bmiHeader.biClrImportant = 0;

        this->bmp = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &this->bits, 0, 0);
        this->hdc = this->bmp ? CreateCompatibleDC(nullptr) : nullptr;
        this->old = this->hdc ? SelectObject(this->hdc, this->bmp) : nullptr;
    }

   ~D2DDc()
    {
        if (this->hdc)
        {
            SelectObject(this->hdc, this->old);
            DeleteDC(this->hdc);
        }

        if (this->bmp)
        {
            DeleteObject(this->bmp);
        }
    }

    operator bool()
    {
        return this->hdc ? true : false;
    }

    operator HDC()
    {
        return this->hdc;
    }

private:
    HDC     hdc;
    HBITMAP bmp;
    HGDIOBJ old;
    void*   bits;
};

class DDCRenderer : public D2DRenderer
{
public:
    static DDCRenderer Create(HDC hdc);

    DDCRenderer();
    DDCRenderer(DDCRenderer&& other);
    DDCRenderer(ID2D1RenderTarget* target);
    DDCRenderer(const DDCRenderer&) = delete;

    DDCRenderer& operator=(DDCRenderer&& other);
    DDCRenderer& operator=(const DDCRenderer&) = delete;

    bool BeginPaint();

private:
    bool BeginPaint(HWND);
};