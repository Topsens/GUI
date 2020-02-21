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
    if (!this->scene.Create(this))
    {
        return false;
    }
    this->scene.Show();

    return MainWindow::OnCreated();
}

void Application::OnSize()
{
    this->scene.Resize(this->ClientWidth(), this->ClientHeight());
    this->scene.Move(0, 0);
}