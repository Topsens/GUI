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

    this->Caption(L"D2D");
    this->ResizeClient(400, 400);

    return true;
}

void Application::OnPaint()
{
    if (renderer.BeginPaint())
    {
        int w = this->ClientWidth();
        int h = this->ClientHeight();
        
        auto x = w * .5f;
        auto y = h * .5f;

        renderer.Draw(this->bitmap, 0, 0, (float)w, (float)h);
        
        renderer.SolidBrush(RGB(255, 0, 0), .5f);
        renderer.Translate(x + 100.f, y);
        renderer.Fill(D2DEllipse(25.f, 25.f));
        renderer.Identity();

        renderer.Translate(x - 100.f, y);
        renderer.Fill(D2DEllipse(25.f, 25.f));
        renderer.Identity();

        renderer.SolidBrush(RGB(0, 255, 0), .5f);
        renderer.Translate(x, y);
        renderer.Rotate(45.f);
        renderer.Fill(D2DRectangle(50.f, 50.f));

        renderer.SolidBrush(RGB(0, 0, 255), .5f);
        renderer.Stroke(20.f);
        renderer.Draw(D2DRectangle(50.f + 20.f, 50.f + 20.f));
        renderer.Identity();

        renderer.Font(L"Segoe UI", 40, DWRITE_FONT_WEIGHT_BOLD) && renderer.SolidBrush(RGB(0, 255, 0), .5f);
        renderer.Text(L"D2D", 0.f, 0.f, (float)w, 40.f);

        renderer.EndPaint();
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
        vector<int> pixels(w * h);

        for (auto i = 0, idx = 0; i < h; i++)
        {
            auto odd = (i >> 4) & 1;

            for (auto j = 0; j < w; j++)
            {
                pixels[idx++] = (odd == ((j >> 4) & 1)) ? 0xC0FFFFFF : 0x80FFFFFF;
            }
        }

        this->bitmap = this->renderer.CreateBitmap(w, h, pixels.data(), true);
    }
}