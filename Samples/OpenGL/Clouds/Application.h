#include "MainWindow.h"
#include "Scene.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    Scene scene;
};