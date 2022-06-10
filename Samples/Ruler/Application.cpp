#include "Application.h"

#define EDGE (10)

const COLORREF Colors[] = { RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 255), RGB(0, 0, 0) };

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(nCmdShow);
}

Application::Application(HINSTANCE instance) : MainWindow(instance), color(0)
{
}

bool Application::OnCreated()
{
    if (!MainWindow::OnCreated())
    {
        return false;
    }

    this->renderer = DDCRenderer::Create();
    if (!this->renderer)
    {
        return false;
    }

    this->RegisterMessage(WM_KEYUP, [this]
    {
        if (VK_ESCAPE == this->wparam)
        {
            this->Destroy();
        }

        return (LRESULT)0;
    });

    this->RegisterMessage(WM_KEYDOWN, [this]
    {
        this->OnKeyDown();
        return (LRESULT)0;
    });

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        this->OnMouseMove();
        return (LRESULT)0;
    });

    this->RegisterMessage(WM_LBUTTONDBLCLK, [this]
    {
        this->vertical = !this->vertical;
        this->Resize(this->Height(), this->Width());
        return 0;
    });

    this->Style(WS_OVERLAPPED | WS_POPUP);
    this->StyleEx(WS_EX_LAYERED);

    this->vertical = false;
    this->Resize(300, 50);

    return true;
}

void Application::OnSize()
{
    this->Update();
}

void Application::OnKeyDown()
{
    auto shift = GetKeyState(VK_SHIFT) < 0 ? true : false;

    switch (this->wparam)
    {
        case VK_LEFT:
        {
            if (shift)
            {
                if (!this->vertical && this->length > 0)
                {
                    this->Resize(this->Width() - 1, this->Height());
                }
            }
            else if (this->X() + EDGE > 0)
            {
                this->MoveTo(this->X() - 1, this->Y());
            }

            break;
        }

        case VK_RIGHT:
        {
            if (shift)
            {
                if (!this->vertical)
                {
                    this->Resize(this->Width() + 1, this->Height());
                }
            }
            else
            {
                RECT rect;
                GetWindowRect(GetDesktopWindow(), &rect);

                if (this->X() + this->Width() < rect.right + EDGE)
                {
                    this->MoveTo(this->X() + 1, this->Y());
                }
            }

            break;
        }

        case VK_UP:
        {
            if (shift)
            {
                if (this->vertical && this->length > 0)
                {
                    this->Resize(this->Width(), this->Height() - 1);
                }
            }
            else if (this->Y() + EDGE > 0)
            {
                this->MoveTo(this->X(), this->Y() - 1);
            }
            break;
        }

        case VK_DOWN:
        {
            if (shift)
            {
                if (this->vertical)
                {
                    this->Resize(this->Width(), this->Height() + 1);
                }
            }
            else
            {
                RECT rect;
                GetWindowRect(GetDesktopWindow(), &rect);

                if (this->Y() + this->Height() < rect.bottom + EDGE)
                {
                    this->MoveTo(this->X(), this->Y() + 1);
                }
            }

            break;
        }

        case VK_RETURN:
        {
            this->vertical = !this->vertical;
            this->Resize(this->Height(), this->Width());
            break;
        }

        case VK_SPACE:
        {
            this->color = (this->color + 1) % (sizeof(Colors) / sizeof(Colors[0]));
            this->Update();
            break;
        }

        default: break;
    }
}

void Application::OnMouseMove()
{
    POINT pos;
    GetCursorPos(&pos);

    if (MK_LBUTTON & this->wparam)
    {
        SetCapture(this->Handle());
        auto x = pos.x - this->pos.x;
        auto y = pos.y - this->pos.y;

        if (this->sizing)
        {
            if (this->vertical)
            {
                this->Resize(this->Width(), max(2 * EDGE, this->Height() + y));
            }
            else
            {
                this->Resize(max(2 * EDGE, this->Width() + x), this->Height());
            }
        }
        else
        {
            this->Move(x, y);
        }
    }
    else
    {
        if (this->Handle() == GetCapture())
        {
            ReleaseCapture();
        }

        if (this->vertical && pos.y - this->Y() >= this->Height() - EDGE)
        {
            this->sizing = true;
            this->Cursor(IDC_SIZENS);
        }
        else if (!this->vertical && pos.x - this->X() >= this->Width() - EDGE)
        {
            this->sizing = true;
            this->Cursor(IDC_SIZEWE);
        }
        else
        {
            this->sizing = false;
            this->Cursor(IDC_ARROW);
        }
    }

    this->pos = pos;
}

void Application::Update()
{
    int w = this->ClientWidth();
    int h = this->ClientHeight();
    this->renderer.ResizeTarget(w, h);

    if (this->renderer.BeginPaint())
    {
        this->renderer.Clear(RGB(0, 0, 0), 0.1f);

        this->renderer.Brush(this->renderer.CreateSolidBrush(Colors[this->color]));

        if (this->vertical)
        {
            this->length = h - 2 * EDGE;

            this->renderer.From(EDGE, EDGE - .5f);
            this->renderer.LineTo((float)w - EDGE, EDGE - .5f);

            this->renderer.From(EDGE, EDGE + this->length + .5f);
            this->renderer.LineTo((float)w - EDGE, EDGE + this->length + .5f);
        }
        else
        {
            this->length = w - 2 * EDGE;

            this->renderer.From(EDGE - .5f, EDGE);
            this->renderer.LineTo(EDGE - .5f, (float)h - EDGE);

            this->renderer.From(EDGE + this->length + .5f, EDGE);
            this->renderer.LineTo(EDGE + this->length + .5f, (float)h - EDGE);
        }

        auto format = D2DFormat::Create(L"Segoe UI", 10);
        if (format)
        {
            format.TextAlign(DWRITE_TEXT_ALIGNMENT_CENTER);
            format.ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
            this->renderer.Format(format);
            this->renderer.Text(std::to_wstring(this->length), 0, 0, (float)w, (float)h);
        }

        this->renderer.EndPaint();

        auto sp = POINT{ 0, 0 };
        auto dp = POINT{ this->X(), this->Y() };
        auto size = SIZE{ w, h };

        BLENDFUNCTION blend = {};
        blend.AlphaFormat = AC_SRC_ALPHA;
        blend.SourceConstantAlpha = 255;

        UPDATELAYEREDWINDOWINFO ulwi = {};
        ulwi.cbSize  = sizeof(ulwi);
        ulwi.psize   = &size;
        ulwi.hdcSrc  = this->renderer.GetDC();
        ulwi.pptSrc  = &sp;
        ulwi.pptDst  = &dp;
        ulwi.pblend  = &blend;
        ulwi.dwFlags = ULW_ALPHA;

        UpdateLayeredWindowIndirect(this->Handle(), &ulwi);
    }
}