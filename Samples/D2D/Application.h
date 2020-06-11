#include "MainWindow.h"
#include "D2DRenderer.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnPaint()   override;
    void OnSize()    override;

private:
    D2DRenderer renderer;
    D2DBitmap   bitmap;
    D2DFormat   format;
    D2DBrush    red;
    D2DBrush    blue;
    D2DBrush    green;
};