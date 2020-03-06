#include "Scene.h"
#include "Texture.h"

Scene::Scene() : GLWindow()
{
    auto& l0 = this->scene.EnableLight(0);
    l0.Ambient(1.f, 1.f, 1.f, 1.f);
    l0.Diffuse(1.f, 1.f, 1.f, 1.f);
    l0.Specular(1.f, 1.f, 1.f, 1.f);
    l0.Position(0.f, 0.f, 5.f, 1.f);

    auto& l1 = this->scene.EnableLight(1);
    l1.Diffuse({ 1.f, 1.f, 1.f, 1.f });
    l1.Specular(1.f, 1.f, 1.f, 1.f);
    l1.Position(0.f, 5.f, 0.f, 1.f);

    this->cube.Position[0] = sqrtf(2.f) * 0.5f;
    this->cube.Position[2] = 0.5f;

    this->sphere.Position[0] = -sqrt(2.f) * 0.5f;
    this->sphere.Position[2] = 0.5f;

    this->cylinder.Position[2] = -1.f;
    this->cylinder.Rotation[0] = 90.f;

    this->scene.Camera().Clip(0.01f, 100.f);
}

void Scene::Perspective(bool perspective)
{
    this->scene.Camera().IsPerspective(perspective);

    if (perspective)
    {
        this->scene.Camera().HorizontalFov(90.f);
    }
    else
    {
        this->scene.Camera().OrthogonalWidth(4.f * sqrtf(2.f));
    }
}

void Scene::SetCamera(const Vector<float, 3>& position, const Vector<float, 3>& lookAt)
{
    this->scene.Camera().Position(position);
    this->scene.Camera().LookAt(lookAt);
}

void Scene::OnPaint()
{
    this->scene.Begin(this->ClientWidth(), this->ClientHeight());
    this->cube.Render();
    this->sphere.Render();
    this->cylinder.Render();
    this->scene.End();

    GLWindow::OnPaint();
}

bool Scene::OnCreated()
{
    if (GLWindow::OnCreated())
    {
        LoadTexture(this->cube.Texture(), L"Portrait.png");
        LoadTexture(this->sphere.Texture(), L"Portrait.png");
        LoadTexture(this->cylinder.Texture(), L"Portrait.png");

        return true;
    }

    return false;
}

void Scene::OnDestroy()
{
    this->cube.Release();
    this->sphere.Release();
    this->cylinder.Release();
    GLWindow::OnDestroy();
}

void Scene::LoadTexture(GLTexture& texture, const wchar_t* file)
{
    Texture tex(file);
    texture.Set(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);
}