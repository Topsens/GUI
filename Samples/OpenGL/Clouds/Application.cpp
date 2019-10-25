#include "Application.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    return Application(hInstance).Run();
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