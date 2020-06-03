#include "Application.h"
#include "GDIRenderer.h"

#define IDC_STATUS  (1000)

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance), status(IDC_STATUS)
{
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    int positions[] = { 100, 200, -1 };
    if (!this->status.Create(this, this->instance, SBARS_SIZEGRIP | CCS_BOTTOM | WS_CHILD) || !this->status.SetParts(3, positions))
    {
        return false;
    }

    this->progress = ProgressBar::Create(this->status.Handle());
    if (!this->progress)
    {
        return false;
    }

    this->status.ClipChildren();
    this->status.Text(L"Hello World!", 0);
    this->status.Text(L"50%", 2);
    this->status.Show();

    this->progress.SetRange(0, 100);
    this->progress.Position(50);
    this->progress.Show();

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        POINTS p = MAKEPOINTS(this->lparam);
        auto text = L"x,y = {" + std::to_wstring(p.x) + L',' + std::to_wstring(p.y) + L'}';
        this->status.Text(text, 1);
        return 0;
    });

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
    this->progress.MoveTo(200 + 30, 5);
    this->progress.Resize(this->status.Width() - this->progress.X() - 15, 18);
    MainWindow::OnSize();
}