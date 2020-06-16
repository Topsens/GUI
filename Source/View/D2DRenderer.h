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
    D2DRectangle(float left, float top, float right, float bottom, float radius = 0.f)
    {
        this->rect.rect.left   = left;
        this->rect.rect.top    = top;
        this->rect.rect.right  = right;
        this->rect.rect.bottom = bottom;
        this->rect.radiusX     = radius;
        this->rect.radiusY     = radius;
    }

    D2DRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY)
    {
        this->rect.rect.left   = left;
        this->rect.rect.top    = top;
        this->rect.rect.right  = right;
        this->rect.rect.bottom = bottom;
        this->rect.radiusX     = radiusX;
        this->rect.radiusY     = radiusY;
    }

    D2DRectangle(float width, float height, const D2DPoint& center = D2DPoint(0.f, 0.f), float radius = 0.f)
    {
        this->rect.rect.left   = center.X - width  * .5f;
        this->rect.rect.right  = center.X + width  * .5f;
        this->rect.rect.top    = center.Y - height * .5f;
        this->rect.rect.bottom = center.Y + height * .5f;
        this->rect.radiusX     = radius;
        this->rect.radiusY     = radius;
    }

    D2DRectangle(float width, float height, const D2DPoint& center, float radiusX, float radiusY)
    {
        this->rect.rect.left   = center.X - width  * .5f;
        this->rect.rect.right  = center.X + width  * .5f;
        this->rect.rect.top    = center.Y - height * .5f;
        this->rect.rect.bottom = center.Y + height * .5f;
        this->rect.radiusX     = radiusX;
        this->rect.radiusY     = radiusY;
    }

    operator D2D1_RECT_F() const
    {
        return this->rect.rect;
    }

    operator D2D1_ROUNDED_RECT() const
    {
        return this->rect;
    }

    float L() const
    {
        return this->rect.rect.left;
    }

    float R() const
    {
        return this->rect.rect.right;
    }

    float T() const
    {
        return this->rect.rect.top;
    }

    float B() const
    {
        return this->rect.rect.bottom;
    }

    float W() const
    {
        return this->rect.rect.right - this->rect.rect.left;
    }

    float H() const
    {
        return this->rect.rect.bottom - this->rect.rect.top;
    }

    float Rx() const
    {
        return this->rect.radiusX;
    }

    float Ry() const
    {
        return this->rect.radiusY;
    }

private:
    D2D1_ROUNDED_RECT rect;
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

    operator D2D1_ELLIPSE() const
    {
        return this->Value;
    }

    D2D1_ELLIPSE Value;
};

class D2DBitmap
{
public:
    D2DBitmap();
    D2DBitmap(D2DBitmap&& other);
    D2DBitmap(ID2D1Bitmap* bitmap);
    D2DBitmap(const D2DBitmap&) = delete;
   ~D2DBitmap();

    operator bool() const;
    operator ID2D1Bitmap*() const;
    D2DBitmap& operator=(D2DBitmap&& other);
    D2DBitmap& operator=(const D2DBitmap&) = delete;

    int Width() const;
    int Height() const;

    bool Pixels(const uint32_t* pixels, bool premultiply = false);

private:
    ID2D1Bitmap* bitmap;
};

class D2DBrush
{
public:
    D2DBrush();
    D2DBrush(D2DBrush&& other);
    D2DBrush(ID2D1Brush* brush);
    D2DBrush(const D2DBrush&) = delete;
   ~D2DBrush();

    operator bool() const;
    operator ID2D1Brush*() const;

    D2DBrush& operator=(D2DBrush&& other);
    D2DBrush& operator=(const D2DBrush&) = delete;

private:
    ID2D1Brush* brush;
};

class D2DStroke
{
public:
    D2DStroke(float width = 1.f);
    D2DStroke(D2DStroke&& other);
    D2DStroke(float width, ID2D1StrokeStyle* style);
    D2DStroke(const D2DStroke& stroke) = delete;
   ~D2DStroke();

    operator ID2D1StrokeStyle*() const;

    D2DStroke& operator=(D2DStroke&& other);

    float Width() const;

private:
    float width;
    ID2D1StrokeStyle* style;
};

class D2DFormat
{
public:
    static D2DFormat Create(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

    D2DFormat();
    D2DFormat(D2DFormat&& other);
    D2DFormat(IDWriteTextFormat* format);
    D2DFormat(const D2DFormat&) = delete;
   ~D2DFormat();

    operator bool() const;
    operator IDWriteTextFormat*() const;

    D2DFormat& operator=(D2DFormat&& other);
    D2DFormat& operator=(const D2DFormat&) = delete;

    void TextAlign(DWRITE_TEXT_ALIGNMENT alignment);
    void ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment);

private:
    IDWriteTextFormat* format;
};

class D2DRenderer
{
    friend class D2DFormat;

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

    bool Skew(float angleX, float angleY, float centerX, float centerY);
    bool Scale(float scaleX, float scaleY, float centerX, float centerY);
    bool Rotate(float angle, float centerX = 0.f, float centerY = 0.f);
    bool Translate(float translateX, float translateY);
    bool Identity();

    bool Brush(const D2DBrush& brush);

    void From(float x, float y);
    bool LineTo(float x, float y);
    void Stroke(const D2DStroke& stroke);

    bool Draw(const D2DRectangle& rectangle) const;
    bool Draw(const D2D1_ELLIPSE& ellipse) const;
    bool Draw(const D2DBitmap& bitmap, const D2D1_RECT_F& rectangle, float opacity = 1.f) const;
    bool Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity = 1.f) const;
    bool Fill(const D2DRectangle& rectangle) const;
    bool Fill(const D2D1_ELLIPSE& ellipse) const;

    bool Format(const D2DFormat& format);
    bool Text(const std::wstring& text, const D2D_RECT_F& rectangle) const;
    bool Text(const std::wstring& text, float x, float y, float w, float h) const;
    bool Text(const wchar_t* text, const D2D1_RECT_F& rectangle) const;
    bool Text(const wchar_t* text, float x, float y, float w, float h) const;

    bool Clear(COLORREF rgb, float opacity = 1.f);
    bool Clear(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);

    D2DBitmap CreateBitmap(int width, int height);
    D2DBrush  CreateSolidBrush(COLORREF rgb, float opacity = 1.f);
    D2DBrush  CreateSolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);

protected:
    static bool AddRefFactories();
    static void ReleaseFactories();

protected:
    float x, y;
    float width;

    ID2D1RenderTarget* target;
    IDWriteTextFormat* format;
    ID2D1StrokeStyle*  style;
    ID2D1Brush*        brush;

    D2D1_MATRIX_3X2_F transform;

    static std::mutex mutex;

    static ID2D1Factory*   d2d1Factory;
    static IDWriteFactory* dwriteFactory;
};