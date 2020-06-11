#include "Application.h"
#include "D2DRenderer.h"
#include <vector>

using namespace std;

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

    this->renderer = D2DRenderer::Create(this->Handle());
    if (!this->renderer)
    {
        return false;
    }

    this->format = D2DFormat::Create(L"Segoe UI", 40, DWRITE_FONT_WEIGHT_BOLD);

    this->red   = this->renderer.CreateSolidBrush(255, 0, 0, .5f);
    this->blue  = this->renderer.CreateSolidBrush(0, 0, 255, .5f);
    this->green = this->renderer.CreateSolidBrush(0, 255, 0, .5f);

    this->Caption(L"D2D");
    this->ResizeClient(400, 400);

    return true;
}

void Application::OnPaint()
{
    if (this->renderer.BeginPaint())
    {
        int w = this->ClientWidth();
        int h = this->ClientHeight();
        
        auto x = w * .5f;
        auto y = h * .5f;

        this->renderer.Draw(this->bitmap, 0, 0, (float)w, (float)h);
        
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
        this->renderer.Text(L"D2D", 0.f, 0.f, (float)w, 40.f);

        this->renderer.EndPaint();
    }
    
    MainWindow::OnPaint();
}

void Application::OnSize()
{
    auto w = this->ClientWidth();
    auto h = this->ClientHeight();

    this->renderer.ResizeTarget(this->ClientWidth(), this->ClientHeight());

    if (this->bitmap.Width() != w || this->bitmap.Height() != h)
    {
        vector<uint32_t> pixels(w * h);

        for (auto i = 0, idx = 0; i < h; i++)
        {
            auto odd = (i >> 4) & 1;

            for (auto j = 0; j < w; j++)
            {
                pixels[idx++] = (odd == ((j >> 4) & 1)) ? 0xC0FFFFFF : 0x80FFFFFF;
            }
        }

        this->bitmap = this->renderer.CreateBitmap(w, h);
        this->bitmap.Pixels(pixels.data(), true);
    }
}