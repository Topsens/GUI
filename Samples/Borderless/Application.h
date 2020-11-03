#include "MainWindow.h"
#include "D2DRenderer.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnPaint()   override;

private:
    POINT pos;

    D2DRenderer renderer;
};