#include "View.h"
#include <vector>

using namespace std;

View::View(HINSTANCE instance)
  : hwnd(nullptr), owner(nullptr), parent(nullptr), instance(instance), cursor(nullptr)
{
    this->wrect = { 0, 0, 0, 0 };
    this->crect = { 0, 0, 0, 0 };
}

View::~View()
{
}

WPARAM View::MessageLoop(HWND hwnd, HACCEL haccel)
{
    MSG  msg = {0};
    BOOL ret;

    while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
    {
        if (-1 == ret)
        {
            break;
        }

        if (hwnd)
        {
            if (haccel && TranslateAcceleratorW(hwnd, haccel, &msg))
            {
                continue;
            }

            if (IsDialogMessageW(hwnd, &msg))
            {
                continue;
            }
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return msg.wParam;
}

LRESULT View::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    View* pThis = nullptr;

    if (WM_NCCREATE == uMsg)
    {
        pThis = reinterpret_cast<View*>(((LPCREATESTRUCTW)lParam)->lpCreateParams);
        pThis->hwnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<View*>(lParam);
        pThis->hwnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, lParam);
    }
    else
    {
        pThis = reinterpret_cast<View*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        pThis->umsg   = uMsg;
        pThis->wparam = wParam;
        pThis->lparam = lParam;

        return (pThis->MessageHandler(hWnd, uMsg, wParam, lParam));
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT View::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_MOVE:
        this->c2scr = { 0, 0 };
        ClientToScreen(this->hwnd, &this->c2scr);
        GetWindowRect(this->hwnd, &this->wrect);
        this->OnMove();
        break;

    case WM_SIZE:
        this->c2scr = { 0, 0 };
        ClientToScreen(this->hwnd, &this->c2scr);
        GetWindowRect(this->hwnd, &this->wrect);
        GetClientRect(this->hwnd, &this->crect);
        this->OnSize();
        break;

    case WM_DESTROY:
        this->OnDestroy();
        this->c2scr  = { 0, 0 };
        this->wrect  = { 0, 0, 0, 0 };
        this->crect  = { 0, 0, 0, 0 };
        this->hwnd   = nullptr;
        this->owner  = nullptr;
        this->parent = nullptr;
        break;

    default:
        return this->WindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

LRESULT View::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        this->OnPaint();
        break;

    case WM_CLOSE:
        this->OnClose();
        break;

    case WM_TIMER:
        this->OnTimer();
        break;

    case WM_SETCURSOR:
        if (this->OnSetCursor())
        {
            return TRUE;
        }
        break;

    case WM_COMMAND:
        this->command = LOWORD(wParam);
        if (this->OnCommand())
        {
            return FALSE;
        }
        break;

    default:
        break;
    }

    return this->DefaultProc(hWnd, uMsg, wParam, lParam);
}

void View::Destroy()
{
    if (this->hwnd)
    {
        DestroyWindow(this->hwnd);
    }
}

void View::Show() const
{
    if (this->hwnd)
    {
        ShowWindow(this->hwnd, SW_SHOW);
        UpdateWindow(this->hwnd);
    }
}

void View::Hide() const
{
    if (this->hwnd)
    {
        ShowWindow(this->hwnd, SW_HIDE);
    }
}

bool View::Update() const
{
    return FALSE != UpdateWindow(this->hwnd);
}

bool View::Invalidate(bool erase) const
{
    return FALSE != InvalidateRect(this->hwnd, nullptr, erase ? TRUE : FALSE);
}

bool View::Invalidate(const RECT& rect, bool erase) const
{
    return FALSE != InvalidateRect(this->hwnd, &rect,  erase ? TRUE : FALSE);
}

bool View::Move(int x, int y, bool repaint) const
{
    return this->MoveTo(this->X() + x, this->Y() + y, repaint);
}

bool View::MoveTo(int x, int y, bool repaint) const
{
    if (!this->hwnd)
    {
        return false;
    }

    return MoveWindow(this->hwnd, x, y, this->Width(), this->Height(), repaint ? TRUE : FALSE) ? true : false;
}

bool View::Resize(int width, int height, bool repaint) const
{
    if (!this->hwnd || width < 0 || height < 0)
    {
        return false;
    }

    return MoveWindow(this->hwnd, this->X(), this->Y(), width, height, repaint ? TRUE : FALSE) ? true : false;
}

bool View::ResizeClient(int width, int height, bool repaint) const
{
    return this->Resize(width + this->Width() - this->ClientWidth(), height + this->Height() - this->ClientHeight(), repaint);
}

void View::Enable() const
{
    if (this->hwnd)
    {
        EnableWindow(this->hwnd, TRUE);
    }
}

void View::Disable() const
{
    if (this->hwnd)
    {
        EnableWindow(this->hwnd, FALSE);
    }
}

bool View::IsEnabled() const
{
    if (this->hwnd)
    {
        return FALSE != IsWindowEnabled(this->hwnd);
    }

    return false;
}

bool View::IsVisible() const
{
    if (this->hwnd)
    {
        return FALSE != IsWindowVisible(this->hwnd);
    }

    return false;
}

bool View::HasFocus() const
{
    if (this->hwnd)
    {
        return GetFocus() == this->hwnd;
    }

    return false;
}

HWND View::SetFocus() const
{
    if (this->hwnd)
    {
        return ::SetFocus(this->hwnd);
    }

    return nullptr;
}

LRESULT View::Send(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return SendMessageW(this->hwnd, uMsg, wParam, lParam);
}

bool View::Post(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return FALSE != PostMessageW(this->hwnd, uMsg, wParam, lParam);
}

int View::X() const
{
    if (this->parent)
    {
        return this->wrect.left - this->parent->c2scr.x;
    }

    return this->wrect.left;
}

int View::Y() const
{
    if (this->parent)
    {
        return this->wrect.top - this->parent->c2scr.y;
    }

    return this->wrect.top;
}

int View::Width() const
{
    return this->wrect.right - this->wrect.left;
}

int View::Height() const
{
    return this->wrect.bottom - this->wrect.top;
}

RECT View::Rect() const
{
    return this->wrect;
}

int View::ClientWidth() const
{
    return crect.right - crect.left;
}

int View::ClientHeight() const
{
    return this->crect.bottom - this->crect.top;
}

RECT View::ClientRect() const
{
    return this->crect;
}

HWND View::Handle() const
{
    return this->hwnd;
}

bool View::Owner(View* parent)
{
    if (this->hwnd)
    {
        if (!parent)
        {
            SetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT, 0);
            SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
            this->owner = nullptr;
            return true;
        }
        else if (parent->hwnd)
        {
            SetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT, (LONG_PTR)parent->hwnd);
            SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
            this->owner = parent;
            return true;
        }
    }

    return false;
}

View* View::Owner() const
{
    return this->owner;
}

bool View::Parent(View* parent)
{
    if (this->hwnd)
    {
        if (!parent)
        {
            if (SetParent(this->hwnd, nullptr))
            {
                this->parent = nullptr;
                return true;
            }
        }
        else if (parent->hwnd)
        {
            if (SetParent(this->hwnd, parent->hwnd))
            {
                this->parent = parent;
                return true;
            }
        }
    }

    return false;
}

View* View::Parent() const
{
    return this->parent;
}

void View::Cursor(LPTSTR name)
{
    if (name)
    {
        this->cursor = LoadCursor(nullptr, name);
    }
    else
    {
        this->cursor = nullptr;
    }
}

void View::Caption(const wchar_t* caption) const
{
    if (this->hwnd)
    {
        SetWindowTextW(this->hwnd, caption);
    }
}

void View::Caption(const wstring& caption) const
{
    this->Caption(caption.c_str());
}

wstring View::Caption() const
{
    vector<wchar_t> buf;

    auto l = GetWindowTextLengthW(this->hwnd);
    if (l)
    {
        buf.resize(l + 1);

        if (GetWindowTextW(this->hwnd, &buf[0], (int)buf.size()))
        {
            return wstring(buf.data());
        }
    }

    return wstring();
}

void View::SetTimer(UINT_PTR id, UINT elapse)
{
    ::SetTimer(this->Handle(), id, elapse, nullptr);
}

void View::KillTimer(UINT_PTR id)
{
    ::KillTimer(this->Handle(), id);
}

bool View::OnSetCursor()
{
    if (this->cursor)
    {
        SetCursor(this->cursor);
        return true;
    }

    return false;
}

bool View::OnCommand()
{
    if (this->parent)
    {
        this->parent->wparam  = this->wparam;
        this->parent->lparam  = this->lparam;
        this->parent->command = this->command;

        return this->parent->OnCommand();
    }

    return false;
}

bool View::OnCreated()
{
    return true;
}

void View::OnDestroy()
{
}

void View::OnPaint()
{
    PAINTSTRUCT ps;

    auto hdc = BeginPaint(this->hwnd, &ps);
    EndPaint(this->hwnd, &ps);
}

void View::OnClose()
{
    this->Destroy();
}

void View::OnTimer()
{
}

void View::OnMove()
{
}

void View::OnSize()
{
}