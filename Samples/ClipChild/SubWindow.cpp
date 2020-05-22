#include "SubWindow.h"
#include "GDIRenderer.h"
#include <iostream>

void SubWindow::OnPaint()
{
    GDIRenderer renderer;
    if (renderer.BeginPaint(this->Handle()))
    {
        renderer.Clear(RGB(255, 255, 255));
        renderer.EndPaint();
    }
    else
    {
        Window::OnPaint();
    }
}