#include "Application.h"
#include "GDIRenderer.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance), status(1000)
{
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

    this->status.Text(L"Hello", 0);
    this->status.Text(L"World", 1);
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

    int positions[] = { this->status.Width() / 2, -1 };
    this->status.SetParts(2, positions);

    MainWindow::OnSize();
}