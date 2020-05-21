#pragma once

#include "MainWindow.h"
#include "ViewPort.h"
#include "Panel.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnCommand() override;
    void OnSize() override;

private:
    ViewPort vport;
    Panel    panel;
};