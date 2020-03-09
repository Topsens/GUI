#include "MainWindow.h"
#include "ViewPort.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    ViewPort vport;
};