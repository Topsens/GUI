#pragma once

#include "MainWindow.h"
#include "Viewport.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE);

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    Viewport view;
};