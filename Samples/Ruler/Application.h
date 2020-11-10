#pragma once

#include "MainWindow.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE);

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    void OnKeyDown();
    void OnMouseMove();

private:
    POINT pos;

    int  length;
    int  orient;
    bool sizing;
};