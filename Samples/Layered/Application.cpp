#include "Application.h"
#include "DDCRenderer.h"
#include <vector>

using namespace std;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
    this->Style(WS_OVERLAPPED | WS_POPUP);
    this->StyleEx(WS_EX_LAYERED);
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    this->Resize(400, 400);

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        POINT pos;
        GetCursorPos(&pos);

        if (MK_LBUTTON & this->wparam)
        {
            auto x = pos.x - this->pos.x;
            auto y = pos.y - this->pos.y;
            this->Move(x, y);
        }

        this->pos = pos;
        return 0;
    });

    this->RegisterMessage(WM_KEYUP, [this]
    {
        if (VK_ESCAPE == this->wparam)
        {
            this->Destroy();
            return (LRESULT)0;
        }

        return MainWindow::WindowProc(this->Handle(), WM_KEYUP, this->wparam, this->lparam);
    });

    return true;
}

void Application::OnSize()
{
    this->Update();
}

void Application::Update()
{
    auto w = this->ClientWidth();
    auto h = this->ClientHeight();

    if (w <= 0 || h <= 0)
    {
        return;
    }

    D2DDc src(w, h);
    auto renderer = DDCRenderer::Create(src);
    if (src && renderer.BeginPaint())
    {
        renderer.SolidBrush(RGB(0x87, 0xCE, 0xFA), .5f);
        renderer.Fill(D2DEllipse(0.f, 0.f, (float)w, (float)h));
        renderer.SolidBrush(RGB(0, 0, 0));
        renderer.Font(L"Segoe UI", 50.f);
        renderer.Text(L"A layered transparent window", 0, 0, (float)w, (float)h, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        renderer.EndPaint();
    }

    auto sp = POINT{ 0, 0 };
    auto dp = POINT{ 0, 0 };
    auto size = SIZE{ w, h };

    BLENDFUNCTION blend = {};
    blend.AlphaFormat = AC_SRC_ALPHA;
    blend.SourceConstantAlpha = 255;

    UPDATELAYEREDWINDOWINFO ulwi = {};
    ulwi.cbSize  = sizeof(ulwi);
    ulwi.psize   = &size;
    ulwi.hdcSrc  = src;
    ulwi.pptSrc  = &sp;
    ulwi.pptDst  = &dp;
    ulwi.pblend  = &blend;
    ulwi.dwFlags = ULW_ALPHA;

    UpdateLayeredWindowIndirect(this->Handle(), &ulwi);
}