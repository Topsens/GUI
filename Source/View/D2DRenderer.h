#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <mutex>

class D2DPoint
{
public:
    D2DPoint(float x, float y)
    {
        this->X = x;
        this->Y = y;
    }

    float X;
    float Y;
};

class D2DRectangle
{
public:
    D2DRectangle(float left, float top, float right, float bottom)
    {
        this->Value.left   = left;
        this->Value.top    = top;
        this->Value.right  = right;
        this->Value.bottom = bottom;
    }

    D2DRectangle(float width, float height, const D2DPoint& center = D2DPoint(0.f, 0.f))
    {
        this->Value.left   = center.X - width  * .5f;
        this->Value.right  = center.X + width  * .5f;
        this->Value.top    = center.Y - height * .5f;
        this->Value.bottom = center.Y + height * .5f;
    }
    
    D2D1_RECT_F Value;
};

class D2DEllipse
{
public:
    D2DEllipse(float left, float top, float right, float bottom)
    {
        auto center = D2D1::Point2F((left + right) * .5f, (top + bottom) * .5f);
        auto radius = D2D1::Point2F((right - left) * .5f, (bottom - top) * .5f);
        this->Value = D2D1::Ellipse(center, radius.x, radius.y);
    }

    D2DEllipse(float radiusX, float radiusY, const D2DPoint& center = D2DPoint(0.f, 0.f))
    {
        this->Value = D2D1::Ellipse(D2D1::Point2F(center.X, center.Y), radiusX, radiusY);
    }

    D2D1_ELLIPSE Value;
};

class D2DBitmap
{
public:
    D2DBitmap() : bitmap(nullptr)
    {
    }

    D2DBitmap(D2DBitmap&& other)
    {
        this->bitmap = other.bitmap;
        other.bitmap = nullptr;
    }

    D2DBitmap(ID2D1Bitmap* bitmap)
    {
        this->bitmap = bitmap;
        if (this->bitmap)
        {
            this->bitmap->AddRef();
        }
    }

    D2DBitmap(const D2DBitmap&) = delete;

   ~D2DBitmap()
    {
        if (this->bitmap)
        {
            this->bitmap->Release();
        }
    }

    operator bool() const
    {
        return this->bitmap ? true : false;
    }

    operator ID2D1Bitmap*() const
    {
        return this->bitmap;
    }

    D2DBitmap& operator=(D2DBitmap&& other)
    {
        if (this->bitmap)
        {
            this->bitmap->Release();
        }

        this->bitmap = other.bitmap;
        other.bitmap = nullptr;

        return *this;
    }

    D2DBitmap& operator=(const D2DBitmap&) = delete;

    int Width() const
    {
        if (this->bitmap)
        {
            return (int)this->bitmap->GetPixelSize().width;
        }

        return 0;
    }

    int Height() const
    {
        if (this->bitmap)
        {
            return (int)this->bitmap->GetPixelSize().height;
        }

        return 0;
    }

private:
    ID2D1Bitmap* bitmap;
};

class D2DRenderer
{
public:
    static D2DRenderer Create(HWND hWnd);

    D2DRenderer();
    D2DRenderer(D2DRenderer&& other);
    D2DRenderer(const D2DRenderer&) = delete;
    D2DRenderer(ID2D1RenderTarget* target);
   ~D2DRenderer();

    operator bool() const;
    D2DRenderer& operator=(D2DRenderer&& other);
    D2DRenderer& operator=(const D2DRenderer&) = delete;

    bool ResizeTarget(int width, int height);
    bool BeginPaint();
    void EndPaint();
    void Release();

    void Brush(ID2D1Brush* brush);
    bool SolidBrush(COLORREF rgb, float opacity = 1.f);
    bool SolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);

    bool Skew(float angleX, float angleY, float centerX, float centerY);
    bool Scale(float scaleX, float scaleY, float centerX, float centerY);
    bool Rotate(float angle, float centerX = 0.f, float centerY = 0.f);
    bool Translate(float translateX, float translateY);
    bool Identity();

    void From(float x, float y);
    bool LineTo(float x, float y);
    void Stroke(float width, ID2D1StrokeStyle* style = nullptr);

    bool Draw(const D2DRectangle& rectangle);
    bool Draw(const D2DEllipse& ellipse);
    bool Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity = 1.f);
    bool Fill(const D2DRectangle& rectangle);
    bool Fill(const D2DEllipse& ellipse);

    bool Font(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, bool italic = false);
    bool Text(const wchar_t* text, float x, float y, float w, float h, DWRITE_TEXT_ALIGNMENT hAlignment = DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT vAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    bool Clear(COLORREF rgb, float opacity = 1.f);
    bool Clear(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);

    D2DBitmap CreateBitmap(int width, int height, const uint32_t* pixels, bool premultiply = false);

protected:
    static bool AddRefFactories();
    static void ReleaseFactories();

protected:
    float x, y;
    float width;

    ID2D1Brush*        brush;
    ID2D1StrokeStyle*  style;
    ID2D1RenderTarget* target;
    IDWriteTextFormat* format;

    D2D1_MATRIX_3X2_F transform;

    static std::mutex mutex;

    static ID2D1Factory*   d2d1Factory;
    static IDWriteFactory* dwriteFactory;
};