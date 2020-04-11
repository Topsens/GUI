#include "Application.h"
#include "D2DRenderer.h"

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

    if (!this->status.Create(this, this->instance))
    {
        return false;
    }
    this->status.Text(L"Hello world!");
    this->status.Show();

    return true;
}

void Application::OnPaint()
{
    D2DRenderer renderer;
    if (renderer.BeginPaint(this->hwnd))
    {
        renderer.Clear(RGB(0, 255, 0));

        // A status bar occupies client area
        D2DRectangle rect(0, 0, (float)this->ClientWidth(), (float)this->ClientHeight() - this->status.Height());
        renderer.SolidBrush(RGB(255, 0, 0));
        renderer.Fill(rect);

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
    MainWindow::OnSize();
}