#include "MainWindow.h"

class Application : public MainWindow
{
public:
    Application(HINSTANCE instance);

protected:
    bool OnCreated() override;
    void OnSize()    override;

private:
    void Update();

private:
    POINT pos;
};