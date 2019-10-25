#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"

class Scene : public GLWindow
{
public:
    Scene();

    void Perspective(bool perspective);
    void SetCamera(const Vector<float, 3>& position, const Vector<float, 3>& lookAt);

protected:
    virtual void OnPaint() override;
    virtual bool OnCreated() override;
    virtual void OnDestroy() override;

private:
    static void LoadTexture(GLTexture& texture, const wchar_t* file);

private:
    Cube     cube;
    Sphere   sphere;
    Cylinder cylinder;
};