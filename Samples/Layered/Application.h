#include "MainWindow.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM) override;
    bool OnCreated() override;
    void OnSize()    override;

private:
    void Update();

private:
    POINT pos;
};