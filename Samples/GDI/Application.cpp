#include "Application.h"
#include "GDIRenderer.h"
#include <iostream>
#include <vector>

using namespace std;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
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
            this->Move(x, y);
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
    this->Caption(L"D2D");
    this->ResizeClient(400, 400);
    return MainWindow::OnCreated();
}

void Application::OnPaint()
{
    cout << "OnPaint()" << endl;
    
    GDIRenderer renderer;
    if (renderer.BeginPaint(this->Handle()))
    {
        int w = this->ClientWidth();
        int h = this->ClientHeight();
        
        vector<int> pixels(w * h);

        int idx = 0;
        for (auto i = 0; i < h; i++)
        {
            auto odd = (i >> 4) & 1;

            for (auto j = 0; j < w; j++, idx++)
            {
                pixels[idx] = (odd == ((j >> 4) & 1)) ? 0xC0FFFFFF : 0x80FFFFFF;
            }
        }
        renderer.Clear(RGB(0, 255, 0));
        renderer.PixelsARGB((BYTE*)pixels.data(), w, h, 0, 0, w, h, true);

        renderer.SolidBrush(RGB(255, 0, 0));
        renderer.Ellipse(w / 4, h / 4, w / 4 * 3, h / 4 * 3, 128);

        renderer.Font(L"Segoe", 40, FW_BOLD);
        renderer.Text(L"GDI", 0,  0, 100, 40, DT_TOP | DT_LEFT, RGB(0, 0, 255));
        renderer.Text(L"GDI", 0, 40, 100, 40, DT_TOP | DT_LEFT, RGB(0, 0, 255), 128);

        renderer.EndPaint();
    }
    else
    {
        MainWindow::OnPaint();
    }
}