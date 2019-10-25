#include "Application.h"
#include "D2DRenderer.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    return Application(hInstance).Run();
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
    this->Style(WS_OVERLAPPED | WS_POPUP);
}

LRESULT Application::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOUSEMOVE:
    {
        POINT pos;
        GetCursorPos(&pos);

        if (MK_LBUTTON & wParam)
        {
            auto x = pos.x - this->pos.x;
            auto y = pos.y - this->pos.y;
            this->Move(this->X() + x, this->Y() + y);
        }

        this->pos = pos;

        break;
    }

    case WM_KEYUP:
    {
        if (VK_ESCAPE == wParam)
        {
            this->Destroy();
            return 0;
        }

        break;
    }

    default:
        break;
    }
    
    return MainWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}

bool Application::OnCreated()
{
    this->ResizeClient(400, 400);
    return MainWindow::OnCreated();
}

void Application::OnPaint()
{
    D2DRenderer renderer;
    if (renderer.BeginPaint(this->Handle()))
    {
        renderer.Clear(RGB(0x87, 0xCE, 0xFA));

        renderer.SolidBrush(RGB(0, 0, 0));
        renderer.Font(L"Segoe UI", 50.f);
        renderer.Text(L"A borderless window", 0, 0, (float)this->ClientWidth(), (float)this->ClientHeight(), DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

        renderer.EndPaint();
    }
    else
    {
        MainWindow::OnPaint();
    }
}