#pragma once

#include "Window.h"
#include "GLScene.h"
#include <gl/glew.h>

class GLWindow : public Window
{
public:
    GLWindow(HINSTANCE instance = nullptr);
    virtual ~GLWindow();

protected:
    bool AttachContext();
    void DetachContext();

    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    bool OnCreated() override;
    void OnDestroy() override;

    virtual bool OnContextCreated();

protected:
    HDC   hdc;
    HGLRC hrc;
    GLScene scene;
};