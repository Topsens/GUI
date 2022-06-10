#include "Application.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    this->renderer = DDCRenderer::Create();
    if (!this->renderer)
    {
        return false;
    }

    this->format = D2DFormat::Create(L"Segoe UI", 40, DWRITE_FONT_WEIGHT_BOLD);

    this->red   = this->renderer.CreateSolidBrush(255, 0, 0, .5f);
    this->blue  = this->renderer.CreateSolidBrush(0, 0, 255, .5f);
    this->green = this->renderer.CreateSolidBrush(0, 255, 0, .5f);

    uint32_t pixels[] = { 0xC0FFFFFF, 0x80FFFFFF, 0x80FFFFFF, 0xC0FFFFFF };
    auto bitmap = this->renderer.CreateBitmap(2, 2);
    bitmap.Pixels(pixels, true);

    this->bkgrd = this->renderer.CreateBitmapBrush(bitmap, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
    this->bkgrd.Transform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(16, 16)));

    this->Caption(L"DDC");
    this->ResizeClient(400, 400);

    return true;
}

void Application::OnPaint()
{
    PAINTSTRUCT ps = {0};
    auto hdc = BeginPaint(this->Handle(), &ps);

    if (this->renderer.BeginPaint())
    {
        int w = this->ClientWidth();
        int h = this->ClientHeight();

        auto x = w * .5f;
        auto y = h * .5f;

        this->renderer.Brush(this->bkgrd);
        this->renderer.Fill(D2DRectangle(0, 0, (float)w, (float)h));

        this->renderer.Brush(this->red);
        this->renderer.Translate(x + 100.f, y);
        this->renderer.Fill(D2DEllipse(25.f, 25.f));
        this->renderer.Identity();

        this->renderer.Translate(x - 100.f, y);
        this->renderer.Fill(D2DEllipse(25.f, 25.f));
        this->renderer.Identity();

        this->renderer.Brush(this->green);
        this->renderer.Translate(x, y);
        this->renderer.Rotate(45.f);
        this->renderer.Fill(D2DRectangle(50.f, 50.f));

        this->renderer.Brush(this->blue);
        this->renderer.Stroke(D2DStroke(20.f));
        this->renderer.Draw(D2DRectangle(50.f + 20.f, 50.f + 20.f));
        this->renderer.Identity();

        this->renderer.Brush(this->green);
        this->renderer.Format(this->format);
        this->renderer.Text(L"DDC", 0.f, 0.f, (float)w, 40.f);

        this->renderer.EndPaint();

        BitBlt(hdc, 0, 0, this->ClientWidth(), this->ClientHeight(), this->renderer.GetDC(), 0, 0, SRCCOPY);
    }

    EndPaint(this->Handle(), &ps);
}

void Application::OnSize()
{
    this->renderer.ResizeTarget(this->ClientWidth(), this->ClientHeight());
    MainWindow::OnSize();
}