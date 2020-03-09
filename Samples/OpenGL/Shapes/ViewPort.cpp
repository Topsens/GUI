#include "ViewPort.h"
#include "Texture.h"
#include "Triangle.h"
#include "Square.h"
#include "Circle.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "resource.h"

ViewPort::ViewPort() : shape(nullptr)
{
}

void ViewPort::CreateShape(int shape)
{
    this->AttachContext();

    GLShape* s = nullptr;

    switch (shape)
    {
        case IDC_TRIANGLE:
        {
            s = new Triangle();
            break;
        }

        case IDC_SQUARE:
        {
            s = new Square();
            break;
        }

        case IDC_CIRCLE:
        {
            s = new Circle();
            break;
        }

        case IDC_CUBE:
        {
            s = new Cube();
            break;
        }

        case IDC_CYLINDER:
        {
            s = new Cylinder();
            break;
        }

        case IDC_SPHERE:
        {
            s = new Sphere();
            break;
        }

        default:
            break;
    }

    if (s)
    {
        Texture tex(L"Portrait.png");
        s->Texture().Set(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);

        if (this->shape)
        {
            this->shape->Release();
            delete this->shape;
        }
        this->shape = s;
        this->scene.Camera().Position(0.f, 0.f, 5.f);
        this->scene.Camera().LookAt(this->shape->Position);

        this->Invalidate();
    }

    this->DetachContext();
}

LRESULT ViewPort::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_MOUSEMOVE:
        {
            if (MK_LBUTTON & wParam)
            {
                POINT pos;
                if (GetCursorPos(&pos))
                {
                    auto x = pos.x - this->cursor.x;
                    auto y = pos.y - this->cursor.y;
                    this->cursor = pos;

                    if (this->shape)
                    {
                        auto& r0 = this->shape->Rotation[0];
                        auto& r1 = this->shape->Rotation[1];

                        r0 +=  y * 0.1f;
                        r1 +=  x * 0.1f;

                        r0 += r0 > 180.f ? -360.f : (r0 < -180.f ? 360.f : 0.f);
                        r1 += r1 > 180.f ? -360.f : (r1 < -180.f ? 360.f : 0.f);

                        this->Invalidate();
                    }
                }
            }
            break;
        }

        case WM_LBUTTONDOWN:
        {
            SetCapture(this->Handle());
            GetCursorPos(&this->cursor);
            break;
        }

        case WM_LBUTTONUP:
        {
            ReleaseCapture();
            break;
        }

        case WM_MOUSEWHEEL:
        {
            auto delta = 1.f + (float)((short)HIWORD(wParam) / WHEEL_DELTA) * 0.05f;

            auto position = this->scene.Camera().Position();
            auto lookAt   = this->scene.Camera().LookAt();
            this->scene.Camera().Position((position - lookAt) * delta + lookAt);
            this->Invalidate();

            break;
        }
    }

    return GLWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}

bool ViewPort::OnCreated()
{
    if (!GLWindow::OnCreated())
    {
        return false;
    }

    auto& l0 = this->scene.EnableLight(0);
    l0.Ambient(1.f, 1.f, 1.f, 1.f);
    l0.Diffuse(1.f, 1.f, 1.f, 1.f);
    l0.Specular(1.f, 1.f, 1.f, 1.f);
    l0.Position(0.f, 0.f, 5.f, 1.f);

    auto& l1 = this->scene.EnableLight(1);
    l1.Diffuse({ 1.f, 1.f, 1.f, 1.f });
    l1.Specular(1.f, 1.f, 1.f, 1.f);
    l1.Position(0.f, 0.f, 5.f, 1.f);

    this->scene.Camera().Clip(0.01f, 100.f);

    return true;
}

void ViewPort::OnDestroy()
{
    this->AttachContext();
    if (this->shape)
    {
        this->shape->Release();
        delete this->shape;
    }
    this->DetachContext();

    GLWindow::OnDestroy();
}

void ViewPort::OnPaint()
{
    this->AttachContext();
    this->scene.Begin(this->ClientWidth(), this->ClientHeight());
    if (this->shape)
    {
        this->shape->Render();
    }
    this->scene.End();
    this->DetachContext();
    GLWindow::OnPaint();
}