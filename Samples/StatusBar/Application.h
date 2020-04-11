#pragma once

#include "MainWindow.h"
#include "StatusBar.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnPaint() override;
    void OnSize() override;

private:
    StatusBar status;
};