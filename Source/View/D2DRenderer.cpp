#include "D2DRenderer.h"
#include "Cleanup.h"
#include <vector>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace std;

std::mutex D2DRenderer::mutex;
ID2D1Factory*   D2DRenderer::d2d1Factory   = nullptr;
IDWriteFactory* D2DRenderer::dwriteFactory = nullptr;

D2DRenderer D2DRenderer::Create(HWND hWnd)
{
    if (!hWnd)
    {
        return D2DRenderer();
    }

    if (!AddRefFactories())
    {
        return D2DRenderer();
    }
    ONCLEANUP(factories, []{ ReleaseFactories(); });

    RECT client;
    if (!GetClientRect(hWnd, &client))
    {
        return D2DRenderer();
    }

    auto propH = D2D1::RenderTargetProperties();
    propH.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
    propH.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
    propH.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    propH.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    auto propD = propH;
    propD.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

    ID2D1HwndRenderTarget* target;
    if (FAILED(D2DRenderer::d2d1Factory->CreateHwndRenderTarget(propH, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)) &&
        FAILED(D2DRenderer::d2d1Factory->CreateHwndRenderTarget(propD, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)))
    {
        return D2DRenderer();
    }
    ONCLEANUP(target, [target]{ target->Release(); });

    if (FAILED(target->Resize(D2D1::SizeU(client.right - client.left, client.bottom - client.top))))
    {
        return D2DRenderer();
    }

    return D2DRenderer(target);
}

D2DRenderer::D2DRenderer() : target(nullptr), format(nullptr), brush(nullptr), style(nullptr), width(1.f), transform(D2D1::Matrix3x2F::Identity())
{
    AddRefFactories();
}

D2DRenderer::D2DRenderer(D2DRenderer&& other) : D2DRenderer()
{
    *this = move(other);
}

D2DRenderer::D2DRenderer(ID2D1RenderTarget* target) : D2DRenderer()
{
    if (target)
    {
        this->target = target;
        this->target->AddRef();
    }
}

D2DRenderer::~D2DRenderer()
{
    this->Release();
    ReleaseFactories();
}

D2DRenderer::operator bool() const
{
    return this->target ? true : false;
}

D2DRenderer& D2DRenderer::operator=(D2DRenderer&& other)
{
    this->Release();

    this->x      = other.x;
    this->y      = other.y;
    this->width  = other.width;
    this->brush  = other.brush;
    this->style  = other.style;
    this->format = other.format;
    this->target = other.target;
    this->transform = other.transform;

    other.brush  = nullptr;
    other.style  = nullptr;
    other.format = nullptr;
    other.target = nullptr;

    return *this;
}

bool D2DRenderer::ResizeTarget(int width, int height)
{
    if (!this->target)
    {
        return false;
    }

    width  = max(0, width);
    height = max(0, height);

    return SUCCEEDED(((ID2D1HwndRenderTarget*)this->target)->Resize(D2D1::SizeU(width, height))) ? true : false;
}

bool D2DRenderer::BeginPaint()
{
    if (!this->target)
    {
        return false;
    }

    this->target->BeginDraw();
    return true;
}

void D2DRenderer::EndPaint()
{
    if (this->target)
    {
        this->target->EndDraw();
    }
}

void D2DRenderer::Release()
{
    if (this->style)
    {
        this->style->Release();
        this->style = nullptr;
    }

    if (this->brush)
    {
        this->brush->Release();
        this->brush = nullptr;
    }

    if (this->format)
    {
        this->format->Release();
        this->format = nullptr;
    }

    if (this->target)
    {
        this->target->Release();
        this->target = nullptr;
    }
}

bool D2DRenderer::Skew(float angleX, float angleY, float centerX, float centerY)
{
    if (!this->target)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Skew(angleX, angleY, D2D1::Point2F(centerX, centerY)) * this->transform;
    this->target->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Scale(float scaleX, float scaleY, float centerX, float centerY)
{
    if (!this->target)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleX, scaleY), D2D1::Point2F(centerX, centerY)) * this->transform;
    this->target->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Rotate(float angle, float centerX, float centerY)
{
    if (!this->target)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(centerX, centerY)) * this->transform;
    this->target->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Translate(float translateX, float translateY)
{
    if (!this->target)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Translation(D2D1::SizeF(translateX, translateY)) * this->transform;
    this->target->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Identity()
{
    if (!this->target)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Identity();
    this->target->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Brush(const D2DBrush& brush)
{
    if (this->brush)
    {
        this->brush->Release();
    }

    this->brush = brush;
    if (this->brush)
    {
        this->brush->AddRef();
    }

    return this->brush ? true : false;
}

void D2DRenderer::From(float x, float y)
{
    this->x = x;
    this->y = y;
}

bool D2DRenderer::LineTo(float x, float y)
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->DrawLine(D2D1::Point2F(this->x, this->y), D2D1::Point2F(x, y), this->brush, this->width, this->style);

    this->x = x;
    this->y = y;

    return true;
}

void D2DRenderer::Stroke(const D2DStroke& stroke)
{
    this->width = stroke.Width();

    if (this->style)
    {
        this->style->Release();
    }

    this->style = stroke;

    if (this->style)
    {
        this->style->AddRef();
    }
}

bool D2DRenderer::Draw(const D2DRectangle& rectangle) const
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    if (((D2D1_ROUNDED_RECT&)rectangle).radiusX || ((D2D1_ROUNDED_RECT&)rectangle).radiusY)
    {
        this->target->DrawRoundedRectangle(rectangle, this->brush, this->width, this->style);
    }
    else
    {
        this->target->DrawRectangle(rectangle, this->brush, this->width, this->style);
    }

    return true;
}

bool D2DRenderer::Draw(const D2D1_ELLIPSE& ellipse) const
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->DrawEllipse(ellipse, this->brush, this->width, this->style);

    return true;
}

bool D2DRenderer::Draw(const D2DBitmap& bitmap, const D2D1_RECT_F& rectangle, float opacity) const
{
    if (!this->target || !bitmap)
    {
        return false;
    }

    this->target->DrawBitmap(bitmap, rectangle, opacity);

    return true;
}

bool D2DRenderer::Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity) const
{
    return this->Draw(bitmap, D2D1::RectF(x, y, x + cx, y + cy), opacity);
}

bool D2DRenderer::Fill(const D2DRectangle& rectangle) const
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    if (((D2D1_ROUNDED_RECT&)rectangle).radiusX || ((D2D1_ROUNDED_RECT&)rectangle).radiusY)
    {
        this->target->FillRoundedRectangle(rectangle, this->brush);
    }
    else
    {
        this->target->FillRectangle(rectangle, this->brush);
    }

    return true;
}

bool D2DRenderer::Fill(const D2D1_ELLIPSE& ellipse) const
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->FillEllipse(ellipse, this->brush);

    return true;
}

bool D2DRenderer::Format(const D2DFormat& format)
{
    if (this->format)
    {
        this->format->Release();
    }

    this->format = format;
    if (this->format)
    {
        this->format->AddRef();
    }

    return this->format ? true : false;
}

bool D2DRenderer::Text(const wstring& text, const D2D1_RECT_F& rectangle) const
{
    return this->Text(text.c_str(), rectangle);
}

bool D2DRenderer::Text(const wstring& text, float x, float y, float w, float h) const
{
    return this->Text(text.c_str(), x, y, w, h);
}

bool D2DRenderer::Text(const wchar_t* text, const D2D1_RECT_F& rectangle) const
{
    if (!this->target || !this->brush || !this->format)
    {
        return false;
    }

    this->target->DrawText(text, (UINT32)wcslen(text), this->format, rectangle, this->brush);
    return true;
}

bool D2DRenderer::Text(const wchar_t* text, float x, float y, float w, float h) const
{
    return this->Text(text, D2D1::RectF(x, y, x + w, y + h));
}

bool D2DRenderer::Clear(COLORREF rgb, float opacity)
{
    return this->Clear(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);
}

bool D2DRenderer::Clear(UCHAR r, UCHAR g, UCHAR b, float opacity)
{
    if (!this->target)
    {
        return false;
    }

    this->target->Clear(D2D1::ColorF(RGB(b, g, r), opacity));

    return true;
}

D2DBitmap D2DRenderer::CreateBitmap(int width, int height)
{
    if (!this->target)
    {
        return D2DBitmap();
    }

    width  = max(0, width);
    height = max(0, height);

    ID2D1Bitmap* bitmap;
    if (FAILED(this->target->CreateBitmap(D2D1::SizeU(width, height), D2D1::BitmapProperties(this->target->GetPixelFormat()), &bitmap)))
    {
        return D2DBitmap();
    }
    ONCLEANUP(bitmap, [bitmap]{ bitmap->Release(); });

    return D2DBitmap(bitmap);
}

D2DBrush D2DRenderer::CreateSolidBrush(COLORREF rgb, float opacity)
{
    return this->CreateSolidBrush(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);
}

D2DBrush D2DRenderer::CreateSolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity)
{
    if (!this->target)
    {
        return D2DBrush();
    }

    ID2D1SolidColorBrush* brush;
    if (FAILED(this->target->CreateSolidColorBrush(D2D1::ColorF(RGB(b, g, r), opacity), &brush)))
    {
        return D2DBrush();
    }
    ONCLEANUP(brush, [brush]{ brush->Release(); });

    return D2DBrush(brush);
}

bool D2DRenderer::AddRefFactories()
{
    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (D2DRenderer::d2d1Factory)
    {
        D2DRenderer::d2d1Factory->AddRef();
        D2DRenderer::dwriteFactory->AddRef();
    }
    else
    {
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &D2DRenderer::d2d1Factory)))
        {
            return false;
        }
        if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory))))
        {
            D2DRenderer::d2d1Factory->Release();
            D2DRenderer::d2d1Factory = nullptr;
            return false;
        }
    }

    return true;
}

void D2DRenderer::ReleaseFactories()
{
    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (D2DRenderer::d2d1Factory)
    {
        if (!D2DRenderer::d2d1Factory->Release())
        {
            D2DRenderer::d2d1Factory = nullptr;
        }

        if (!D2DRenderer::dwriteFactory->Release())
        {
            D2DRenderer::dwriteFactory = nullptr;
        }
    }
}

D2DBitmap::D2DBitmap() : bitmap(nullptr)
{
}

D2DBitmap::D2DBitmap(D2DBitmap&& other)
{
    *this = move(other);
}

D2DBitmap::D2DBitmap(ID2D1Bitmap* bitmap) : bitmap(bitmap)
{
    if (this->bitmap)
    {
        this->bitmap->AddRef();
    }
}

D2DBitmap::~D2DBitmap()
{
    if (this->bitmap)
    {
        this->bitmap->Release();
    }
}

D2DBitmap::operator bool() const
{
    return this->bitmap ? true : false;
}

D2DBitmap::operator ID2D1Bitmap*() const
{
    return this->bitmap;
}

D2DBitmap& D2DBitmap::operator=(D2DBitmap&& other)
{
    if (this->bitmap)
    {
        this->bitmap->Release();
    }

    this->bitmap = other.bitmap;
    other.bitmap = nullptr;

    return *this;
}

int D2DBitmap::Width() const
{
    if (this->bitmap)
    {
        return (int)this->bitmap->GetPixelSize().width;
    }

    return 0;
}

int D2DBitmap::Height() const
{
    if (this->bitmap)
    {
        return (int)this->bitmap->GetPixelSize().height;
    }

    return 0;
}

bool D2DBitmap::Pixels(const uint32_t* pixels, bool premultiply)
{
    if (!pixels || !this->bitmap || !this->Width() || !this->Height())
    {
        return false;
    }

    if (premultiply)
    {
        vector<int> pix(this->Width() * this->Height());

        for (size_t i = 0; i < pix.size(); i++)
        {
            auto bgra = (UCHAR*)&pixels[i];
            if (bgra[3] < 255)
            {
                auto a =  bgra[3] + 1;
                auto b = (bgra[0] * a) >> 8; // approximate to (color = color * alpha / 255) but probably faster
                auto g = (bgra[1] * a) >> 8 << 8;
                auto r = (bgra[2] * a) >> 8 << 16;

                pix[i] = b | g | r | (bgra[3] << 24);
            }
            else
            {
                pix[i] = pixels[i];
            }
        }

        if (FAILED(this->bitmap->CopyFromMemory(nullptr, pix.data(), this->Width() * sizeof(pix[0]))))
        {
            return false;
        }
    }
    else if (FAILED(bitmap->CopyFromMemory(nullptr, pixels, this->Width() * sizeof(pixels[0]))))
    {
        return false;
    }

    return true;
}

D2DBrush::D2DBrush() : brush(nullptr)
{
}

D2DBrush::D2DBrush(D2DBrush&& other)
{
    *this = move(other);
}

D2DBrush::D2DBrush(ID2D1Brush* brush) : brush(brush)
{
    if (this->brush)
    {
        this->brush->AddRef();
    }
}

D2DBrush::~D2DBrush()
{
    if (this->brush)
    {
        this->brush->Release();
    }
}

D2DBrush::operator bool() const
{
    return this->brush ? true : false;
}

D2DBrush::operator ID2D1Brush*() const
{
    return this->brush;
}

D2DBrush& D2DBrush::operator=(D2DBrush&& other)
{
    if (this->brush)
    {
        this->brush->Release();
    }

    this->brush = other.brush;
    other.brush = nullptr;

    return *this;
}

D2DStroke::D2DStroke(float width) : width(width), style(nullptr)
{
}

D2DStroke::D2DStroke(D2DStroke&& other)
{
    *this = move(other);
}

D2DStroke::D2DStroke(float width, ID2D1StrokeStyle* style) : width(width), style(style)
{
    if (this->style)
    {
        this->style->AddRef();
    }
}

D2DStroke::~D2DStroke()
{
    if (this->style)
    {
        this->style->Release();
    }
}

D2DStroke::operator ID2D1StrokeStyle*() const
{
    return this->style;
}

D2DStroke& D2DStroke::operator=(D2DStroke&& other)
{
    if (this->style)
    {
        this->style->Release();
    }

    this->width = other.width;
    this->style = other.style;
    other.style = nullptr;

    return *this;
}

float D2DStroke::Width() const
{
    return this->width;
}

D2DFormat D2DFormat::Create(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
    if (!D2DRenderer::AddRefFactories())
    {
        return D2DFormat();
    }
    ONCLEANUP(factories, []{ D2DRenderer::ReleaseFactories(); });

    IDWriteTextFormat* format;
    if (FAILED(D2DRenderer::dwriteFactory->CreateTextFormat(family, nullptr, weight, style, stretch, size, L"", &format)))
    {
        return D2DFormat();
    }
    ONCLEANUP(format, [format]{ format->Release(); });

    return D2DFormat(format);
}

D2DFormat::D2DFormat() : format(nullptr)
{
}

D2DFormat::D2DFormat(D2DFormat&& other)
{
    *this = move(other);
}

D2DFormat::D2DFormat(IDWriteTextFormat* format) : format(format)
{
    if (this->format)
    {
        this->format->AddRef();
    }
}

D2DFormat::~D2DFormat()
{
    if (this->format)
    {
        this->format->Release();
    }
}

D2DFormat::operator bool() const
{
    return this->format ? true : false;
}

D2DFormat::operator IDWriteTextFormat*() const
{
    return this->format;
}

D2DFormat& D2DFormat::operator=(D2DFormat&& other)
{
    if (this->format)
    {
        this->format->Release();
    }

    this->format = other.format;
    other.format = nullptr;

    return *this;
}

void D2DFormat::TextAlign(DWRITE_TEXT_ALIGNMENT alignment)
{
    if (this->format)
    {
        this->format->SetTextAlignment(alignment);
    }
}

void D2DFormat::ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
    if (this->format)
    {
        this->format->SetParagraphAlignment(alignment);
    }
}