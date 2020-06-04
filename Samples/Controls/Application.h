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
    void OnSize() override;

private:
    HFONT font;
    StatusBar status;
};