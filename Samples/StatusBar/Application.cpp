#include "Application.h"
#include "GDIRenderer.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance), status(1000)
{
}

LRESULT Application::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_MOUSEMOVE:
        {
            POINTS p = MAKEPOINTS(lParam);

            auto text = L"x,y = {" + std::to_wstring(p.x) + L',' + std::to_wstring(p.y) + L'}';
            this->status.Text(text, 1);
            return 0;
        }

        default:
            return MainWindow::WindowProc(hWnd, uMsg, wParam, lParam);
    }
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    int positions[] = { 1, 2 };
    if (!this->status.Create(this, this->instance, SBARS_SIZEGRIP | CCS_BOTTOM | WS_CHILD) || !this->status.SetParts(2, positions))
    {
        return false;
    }

    this->status.Text(L"Hello World!", 0);
    this->status.Show();

    return true;
}

void Application::OnPaint()
{
    GDIRenderer renderer;
    if (renderer.BeginPaint(this->hwnd))
    {
        // A status bar occupies client area
        renderer.Clear(RGB(255, 255, 255));
        renderer.EndPaint();
    }
    else
    {
        MainWindow::OnPaint();
    }
}

void Application::OnSize()
{
    this->status.AutoResize();

    int positions[] = { 100, -1 };
    this->status.SetParts(2, positions);

    MainWindow::OnSize();
}