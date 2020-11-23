#pragma once

#include "MainWindow.h"
#include "DDCRenderer.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE);

protected:
    bool OnCreated() override;
    void OnPaint() override;
    void OnSize() override;

private:
    DDCRenderer renderer;
    D2DFormat   format;
    D2DBrush    red;
    D2DBrush    blue;
    D2DBrush    green;
    D2DBrush    bkgrd;
};