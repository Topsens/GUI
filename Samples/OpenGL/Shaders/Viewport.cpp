#include "Viewport.h"
#include <string>

using namespace std;

Viewport::Viewport() : triangle0(this->program0), triangle1(this->program1)
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
    this->triangle0.Release();
    this->triangle1.Release();
    this->program0.Release();
    this->program1.Release();
    GLWindow::OnDestroy();
}

void Viewport::OnPaint()
{
    if (this->AttachContext())
    {
        this->scene.Begin(this->ClientWidth(), this->ClientHeight());
        this->triangle0.Position = Vertex{ -1, 0, 0 };
        this->triangle1.Position = Vertex{  1, 0, 0 };
        this->triangle0.Render();
        this->triangle1.Render();
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
    if (!this->program0.Create(L"VertexShader.txt", L"FragmentShader0.txt") || !this->program0.Link(log) ||
        !this->program1.Create(L"VertexShader.txt", L"FragmentShader1.txt") || !this->program1.Link(log))
    {
        return false;
    }

    this->triangle0.Create();
    this->triangle1.Create();

    return true;
}