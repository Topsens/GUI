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
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    if (!this->view.Create(this))
    {
        return false;
    }
    this->view.Show();

    return true;
}

void Application::OnSize()
{
    this->view.MoveTo(0, 0);
    this->view.Resize(this->ClientWidth(), this->ClientHeight());
}