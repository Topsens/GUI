#include "Application.h"
#include <gdiplus.h>

using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input;
    GdiplusStartupOutput output;
    GdiplusStartup(&token, &input, &output);

    auto ret = Application(hInstance).Run(nCmdShow);

    GdiplusShutdown(token);

    return ret;
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

    if (!this->vport.Create(this))
    {
        return false;
    }
    this->vport.Show();

    if (!this->panel.Create(this))
    {
        return false;
    }
    this->panel.Show();

    return true;
}

bool Application::OnCommand()
{
    this->vport.CreateShape(this->command);
    return true;
}

void Application::OnSize()
{
    this->vport.MoveTo(0, 0);
    this->vport.Resize(this->ClientWidth(), this->ClientHeight() - this->panel.Height());

    this->panel.MoveTo(0, this->vport.Height());
    this->panel.Resize(this->ClientWidth(), this->panel.Height());
}