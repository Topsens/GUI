#include "Application.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance)
{
}

bool Application::OnCreated()
{
    if (!this->vport.Create(this))
    {
        return false;
    }
    this->vport.Show();

    return MainWindow::OnCreated();
}

void Application::OnSize()
{
    this->vport.Resize(this->ClientWidth(), this->ClientHeight());
    this->vport.MoveTo(0, 0);
}