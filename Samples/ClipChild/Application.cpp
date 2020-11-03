#include "Application.h"
#include "GDIRenderer.h"

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

    // Use WS_CLIPCHILDREN style to avoid child windows flickering when parent window is re-painted
    this->ClipChildren(true);

    if (!this->sub.Create(this))
    {
        return false;
    }

    this->sub.Show();

    this->SetTimer(0, 30);

    return true;
}

void Application::OnPaint()
{
    GDIRenderer renderer;
    if (renderer.BeginPaint(this->Handle()))
    {
        renderer.Clear(RGB(128, 128, 128));
        renderer.EndPaint();
    }
    else
    {
        MainWindow::OnPaint();
    }
}

void Application::OnTimer()
{
    this->Invalidate();
}

void Application::OnSize()
{
    this->sub.Resize(this->ClientWidth() / 2, this->ClientHeight() / 2);
    this->sub.MoveTo(this->ClientWidth() / 4, this->ClientHeight() / 4);
}