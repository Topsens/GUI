#include "Viewport.h"
#include <string>

using namespace std;

Viewport::Viewport() : triangle(this->program)
{
}

bool Viewport::OnCreated()
{
    if (!GLWindow::OnCreated())
    {
        return false;
    }
    
    auto& l = this->scene.EnableLight(0);
    l.Ambient(1.f, 1.f, 1.f, 1.f);
    l.Diffuse(.1f, .1f, .1f, 1.f);
    l.Specular(0.f, 0.f, 0.f, 0.f);
    l.Position(10.f, 15.f, 10.f, 0.f);

    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().Position(0.f, 0.f, 4.f);

    return true;
}

void Viewport::OnDestroy()
{
    this->triangle.Release();
    this->program.Release();
    GLWindow::OnDestroy();
}

void Viewport::OnPaint()
{
    if (this->AttachContext())
    {
        this->scene.Begin(this->ClientWidth(), this->ClientHeight());
        this->triangle.Render();
        this->scene.End();
        this->DetachContext();
    }

    GLWindow::OnPaint();
}

bool Viewport::OnContextCreated()
{
    if (!GLWindow::OnContextCreated())
    {
        return false;
    }

    string log;
    if (!this->program.Create() ||
        !this->program.Link(log))
    {
        return false;
    }

    this->triangle.Create();

    return true;
}