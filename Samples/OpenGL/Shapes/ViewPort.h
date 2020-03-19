#pragma once

#include "GLWindow.h"
#include "GLScene.h"
#include "GLShape.h"

class ViewPort : public GLWindow
{
public:
    ViewPort();
    
    void CreateShape(int shape);

protected:
    LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM) override;
    bool OnCreated() override;
    void OnDestroy() override;
    void OnPaint() override;

private:
    void Render();

private:
    POINT    cursor;
    GLScene  scene;
    GLShape* shape;
};