#include "D2DRenderer.h"
#include <vector>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace std;

std::mutex D2DRenderer::mutex;
ID2D1Factory*   D2DRenderer::d2d1Factory   = nullptr;
IDWriteFactory* D2DRenderer::dwriteFactory = nullptr;

D2DRenderer::D2DRenderer() : target(nullptr), style(nullptr), brush(nullptr), format(nullptr), width(1.f), transform(D2D1::Matrix3x2F::Identity())
{
    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (!d2d1Factory)
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2d1Factory);
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory));
    }
    else
    {
        d2d1Factory->AddRef();
        dwriteFactory->AddRef();
    }
}

D2DRenderer::~D2DRenderer()
{
    this->EndPaint();

    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (!d2d1Factory->Release())
    {
        d2d1Factory = nullptr;
    }

    if (!dwriteFactory->Release())
    {
        dwriteFactory = nullptr;
    }
}

bool D2DRenderer::BeginPaint(HWND hWnd)
{
    if (!this->CreateRenderTarget(hWnd))
    {
        return false;
    }

    this->target->BeginDraw();

    return true;
}

void D2DRenderer::EndPaint()
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
        this->target->EndDraw();
        this->target = nullptr;
    }
}

void D2DRenderer::Brush(ID2D1Brush* brush)
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
}

bool D2DRenderer::SolidBrush(COLORREF rgb, float opacity)
{
    return this->SolidBrush(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);
}

bool D2DRenderer::SolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity)
{
    if (!this->target)
    {
        return false;
    }

    ID2D1SolidColorBrush* brush;
    if (SUCCEEDED(this->target->CreateSolidColorBrush(D2D1::ColorF(RGB(b, g, r), opacity), &brush)))
    {
        if (this->brush)
        {
            this->brush->Release();
        }

        this->brush = brush;
        return true;
    }

    return false;
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

void D2DRenderer::Stroke(float width, ID2D1StrokeStyle* style)
{
    this->width = width;

    if (this->style)
    {
        this->style->Release();
    }

    this->style = style;

    if (this->style)
    {
        this->style->AddRef();
    }
}

bool D2DRenderer::Draw(const D2DRectangle& rectangle)
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->DrawRectangle(rectangle.Value, this->brush, this->width, this->style);

    return true;
}

bool D2DRenderer::Draw(const D2DEllipse& ellipse)
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->DrawEllipse(ellipse.Value, this->brush, this->width, this->style);

    return true;
}

bool D2DRenderer::Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity)
{
    if (!this->target || !bitmap)
    {
        return false;
    }

    this->target->DrawBitmap(bitmap, D2D1::RectF(x, y, x + cx, y + cy), opacity);

    return true;
}

bool D2DRenderer::Fill(const D2DRectangle& rectangle)
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->FillRectangle(rectangle.Value, this->brush);

    return true;
}

bool D2DRenderer::Fill(const D2DEllipse& ellipse)
{
    if (!this->target || !this->brush)
    {
        return false;
    }

    this->target->FillEllipse(ellipse.Value, this->brush);

    return true;
}

bool D2DRenderer::Font(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight, bool italic)
{
    IDWriteTextFormat* format;
    if (SUCCEEDED(dwriteFactory->CreateTextFormat(family, nullptr, weight, italic ? DWRITE_FONT_STYLE_NORMAL : DWRITE_FONT_STYLE_ITALIC, DWRITE_FONT_STRETCH_NORMAL, size, L"", &format)))
    {
        if (this->format)
        {
            this->format->Release();
        }

        this->format = format;
        return true;
    }

    return false;
}

bool D2DRenderer::Text(const wchar_t* text, float x, float y, float w, float h, DWRITE_TEXT_ALIGNMENT hAlignment, DWRITE_PARAGRAPH_ALIGNMENT vAlignment)
{
    if (!this->target || !this->format || !this->brush)
    {
        return false;
    }

    this->format->SetTextAlignment(hAlignment);
    this->format->SetParagraphAlignment(vAlignment);
    this->target->DrawText(text, (UINT32)wcslen(text), this->format, D2D1::RectF(x, y, x + w, y + h), this->brush);

    return true;
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

D2DBitmap D2DRenderer::CreateBitmap(int width, int height, const int* pixels, bool premultiply)
{
    if (!this->target)
    {
        return D2DBitmap();
    }

    ID2D1Bitmap* bitmap;
    if (FAILED(this->target->CreateBitmap(D2D1::SizeU(width, height), D2D1::BitmapProperties(this->target->GetPixelFormat()), &bitmap)))
    {
        return D2DBitmap();
    }

    if (pixels)
    {
        if (premultiply)
        {
            vector<int> pix(width * height);

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

            if (FAILED(bitmap->CopyFromMemory(nullptr, pix.data(), width * sizeof(pix[0]))))
            {
                bitmap->Release();
                return D2DBitmap();
            }
        }
        else if (FAILED(bitmap->CopyFromMemory(nullptr, pixels, width * sizeof(pixels[0]))))
        {
            bitmap->Release();
            return D2DBitmap();
        }
    }

    D2DBitmap bmp(bitmap);
    bitmap->Release();

    return bmp;
}

bool D2DRenderer::CreateRenderTarget(HWND hWnd)
{
    if (!this->target)
    {
        auto prop = D2D1::RenderTargetProperties();
        prop.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
        prop.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
        prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
        prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

        RECT client;
        ::GetClientRect(hWnd, &client);

        ID2D1HwndRenderTarget* target = nullptr;
        if (SUCCEEDED(d2d1Factory->CreateHwndRenderTarget(prop, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right, client.bottom)), &target)))
        {
            if (SUCCEEDED(target->Resize(D2D1::SizeU(client.right, client.bottom))))
            {
                this->target = target;
                return true;
            }

            target->Release();
        }
        else
        {
            prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
            if (SUCCEEDED(d2d1Factory->CreateHwndRenderTarget(prop, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right, client.bottom)), &target)))
            {
                if (SUCCEEDED(target->Resize(D2D1::SizeU(client.right, client.bottom))))
                {
                    this->target = target;
                    return true;
                }

                target->Release();
            }
        }
    }

    return false;
}