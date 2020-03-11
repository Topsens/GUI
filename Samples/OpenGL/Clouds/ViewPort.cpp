#include "ViewPort.h"
#include <fstream>
#include <vector>

using namespace std;

ViewPort::ViewPort()
{
}

LRESULT ViewPort::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOUSEMOVE:
        if (MK_LBUTTON & wParam)
        {
            POINT cursor;
            if (GetCursorPos(&cursor))
            {
                auto x = cursor.x - this->cursor.x;
                auto y = cursor.y - this->cursor.y;
                this->cursor = cursor;

                auto qx = Quaternion<float>::FromAxisAngle(Vertex::XAxis, ToRadian(y * .5f));
                auto qy = Quaternion<float>::FromAxisAngle(Vertex::YAxis, ToRadian(x * .5f));
                auto qr = Quaternion<float>::FromRotation(this->cloud.Rotation);

                this->cloud.Rotation = (qx * qy * qr).ToRotation();
                this->Invalidate();
            }
        }
        break;

    case WM_LBUTTONDOWN:
        SetCapture(this->Handle());
        GetCursorPos(&this->cursor);
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        break;

    case WM_LBUTTONDBLCLK:
        this->cloud.Rotation = { 0.f, 0.f, 0.f, 0.f };
        this->Invalidate();
        break;

    case WM_MOUSEWHEEL:
        {
            auto delta = (short)HIWORD(wParam) / WHEEL_DELTA;
            this->scene.Camera().Position(this->scene.Camera().Position() * (1.f + delta * 0.05f));
            this->Invalidate();
        }

        break;

    default:
        break;
    }

    return GLWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}

bool ViewPort::OnCreated()
{
    if (!GLWindow::OnCreated())
    {
        return false;
    }
    
    this->scene.Camera().Clip(0.01f, 100.f);
    this->scene.Camera().LookAt(0.f, 0.f, 0.f);
    this->scene.Camera().Position(0.f, 0.f, -2.f);

    this->LoadCloud();
    return true;
}

void ViewPort::OnDestroy()
{
    this->AttachContext();
    this->cloud.Release();
    this->DetachContext();
    GLWindow::OnDestroy();
}

void ViewPort::OnPaint()
{
    this->AttachContext();
    this->scene.Begin(this->ClientWidth(), this->ClientHeight());
    this->cloud.Render();
    this->scene.End();
    this->DetachContext();

    GLWindow::OnPaint();
}

void ViewPort::LoadCloud()
{
    ifstream ifs("cloud.txt");

    if (ifs.is_open())
    {
        vector<float> points;
        float value;

        while (!ifs.eof())
        {
            ifs >> value;
            points.push_back(value);
        }

        points.resize((points.size() / 3) * 3);
        if (points.size())
        {
            vector<Vertex> vertices;
            vertices.resize(points.size() / 3);
            memcpy(&vertices[0], (Vertex*)points.data(), vertices.size() * sizeof(vertices[0]));

            while (vertices.size() > 1)
            {
                for (size_t i = 0; i < vertices.size() / 2; i++)
                {
                    auto k = i * 2;
                    vertices[i] = vertices[k] + vertices[k + 1];
                }

                if (vertices.size() & 1)
                {
                    vertices[0] += vertices[vertices.size() - 1];
                }

                vertices.resize(vertices.size() / 2);
            }

            Vertex center = vertices[0] / (float)(points.size() / 3);

            auto& v = (vector<Vertex>&)points;
            for (size_t i = 0; i < v.size(); i++)
            {
                v[i] = v[i] - center;
            }

            this->AttachContext();
            this->cloud.Vertices(v.data(), (int)v.size());
            this->DetachContext();
        }
    }
}