#include "Application.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
    this->Style(WS_OVERLAPPED | WS_POPUP);
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

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
            this->Close();
            return (LRESULT)0;
        }

        return this->DefaultProc(this->Handle(), this->umsg, this->wparam, this->lparam);
    });

    this->renderer = D2DRenderer::Create(this->Handle());

    auto format = D2DFormat::Create(L"Segoe UI", 50.f);
    format.TextAlign(DWRITE_TEXT_ALIGNMENT_CENTER);
    format.ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    this->renderer.Format(format);
    this->renderer.Brush(this->renderer.CreateSolidBrush(RGB(0, 0, 0)));
    this->renderer.ResizeTarget(400, 400);
    this->ResizeClient(400, 400);

    return true;
}

void Application::OnPaint()
{
    if (this->renderer.BeginPaint())
    {
        this->renderer.Clear(RGB(0x87, 0xCE, 0xFA));
        this->renderer.Text(L"A borderless window", 0, 0, (float)this->ClientWidth(), (float)this->ClientHeight());
        this->renderer.EndPaint();
    }
    
    MainWindow::OnPaint();
}