#pragma once

#include "Dialog.h"
#include "StatusBar.h"

class Application : public Dialog
{
public:
    Application(HINSTANCE);
   ~Application();

protected:
    bool OnCreated() override;
    void OnClose() override;
    void OnSize() override;

private:
    bool CreateStatus();

private:
    HFONT font;
    StatusBar status;
};