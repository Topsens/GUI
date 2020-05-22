#pragma once

#include "MainWindow.h"
#include "SubWindow.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE);

protected:
    bool OnCreated() override;
    void OnPaint() override;
    void OnTimer() override;
    void OnSize() override;

private:
    SubWindow sub;
};