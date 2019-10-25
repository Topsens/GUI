#include "Application.h"
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    ULONG_PTR token;
    GdiplusStartupInput input;
    GdiplusStartupOutput output;
    GdiplusStartup(&token, &input, &output);

    auto ret = Application().Run();

    GdiplusShutdown(token);

    return ret;
}

Application::Application()
  : spin(0.f)
{
    this->scenes[0].Perspective(true);
    this->scenes[1].Perspective(false);
}

bool Application::OnCreated()
{
    for (auto i = 0; i < COUNTOF(this->scenes); i++)
    {
        if (!this->scenes[i].Create(this))
        {
            return false;
        }
        this->scenes[i].Show();
    }

    SetTimer(0, 15);

    return MainWindow::OnCreated();
}

void Application::OnDestroy()
{
    KillTimer(0);
    MainWindow::OnDestroy();
}

void Application::OnTimer()
{
    this->spin += .5f;
    if (this->spin > 360.f)
    {
        this->spin -= 360.f;
    }

    Quaternion<float> q;
    q = Quaternion<float>::FromAxisAngle(Vector3<float>::YAxis, this->spin);

    for (auto i = 0; i < COUNTOF(this->scenes); i++)
    {
        this->scenes[i].SetCamera(q.Rotate({ 0.f, 2.f, 2.f }), { 0.f, 0.f, 0.f });
        this->scenes[i].Invalidate();
    }
}

void Application::OnSize()
{
    auto w = this->ClientWidth() / (long)COUNTOF(this->scenes);
    auto h = this->ClientHeight();

    for (auto i = 0; i < COUNTOF(this->scenes); i++)
    {
        this->scenes[i].MoveTo(w * i, 0);
        this->scenes[i].Resize(w, h);
    }
}