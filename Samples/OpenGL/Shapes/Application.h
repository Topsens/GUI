#pragma once

#include "MainWindow.h"
#include "Scene.h"

class Application : public MainWindow
{
public:
    Application();

protected:
    bool OnCreated() override;
    void OnDestroy() override;
    void OnTimer() override;
    void OnSize() override;

private:
    Scene scenes[2];

    float spin;
};