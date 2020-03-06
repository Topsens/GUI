#include "GLWindow.h"
#include "GLScene.h"
#include "GLCloud.h"

class Scene : public GLWindow
{
public:
    Scene();

protected:
    LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM) override;
    bool OnCreated() override;
    void OnDestroy() override;
    void OnPaint() override;

private:
    void LoadCloud();

private:
    bool dragging;
    POINT cursor;
    GLCloud cloud;
};