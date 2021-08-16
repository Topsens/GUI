#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "Program.h"
#include "Triangle.h"

class Viewport : public GLWindow
{
protected:
    bool OnCreated() override;
    void OnDestroy() override;
    void OnPaint() override;

    bool OnContextCreated() override;

private:
    GLScene scene;
    Program program;
    Triangle triangle;
};