#include "ViewPort.h"
#include "Texture.h"
#include "Triangle.h"
#include "Square.h"
#include "Circle.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Donut.h"
#include "resource.h"
#include <iostream>

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

        case IDC_DONUT:
        {
            s = new Donut();
            break;
        }

        default:
            break;
    }

    if (s)
    {
        Texture tex(L"Portrait.png");
        s->Texture().Data(tex.Pixels(), tex.Width(), tex.Height(), tex.Width() * tex.Height() * 4, GL_BGRA);

        if (this->shape)
        {
            this->shape->Release();
            delete this->shape;
        }
        this->shape = s;
        this->scene.Camera().Position(0.f, 0.f, 5.f);
        this->scene.Camera().LookAt(this->shape->Position);

        this->Render();
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
                        auto qx = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
                        auto qy = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
                        auto qr = Quaternion<float>::FromRotation(this->shape->Rotation);

                        this->shape->Rotation = (qx * qy * qr).ToRotation();
                        this->Render();
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

        case WM_LBUTTONDBLCLK:
        {
            if (this->shape)
            {
                this->shape->Rotation = { 0.f, 0.f, 0.f, 0.f };
            }
        }

        case WM_MOUSEWHEEL:
        {
            auto delta = 1.f + (float)((short)HIWORD(wParam) / WHEEL_DELTA) * 0.05f;

            auto position = this->scene.Camera().Position();
            auto lookAt   = this->scene.Camera().LookAt();
            this->scene.Camera().Position((position - lookAt) * delta + lookAt);
            this->Render();

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
    this->scene.Camera().Focal(616.f);

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
    this->Render();
    GLWindow::OnPaint();
}

void ViewPort::Render()
{
    this->AttachContext();
    this->scene.Begin(this->ClientWidth(), this->ClientHeight());
    if (this->shape)
    {
        this->shape->Render();
    }
    this->scene.End();
    this->DetachContext();
}