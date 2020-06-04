#include "StatusBar.h"
#include <vector>

using namespace std;

StatusBar::StatusBar(UINT id) : id(id), hwnd(nullptr), rect({ 0, 0, 0, 0}), parent(nullptr)
{
}

StatusBar::operator bool() const
{
    return this->hwnd ? true : false;
}

bool StatusBar::Create(View* parent, HINSTANCE instance, DWORD style)
{
    if (!parent)
    {
        return false;
    }

    this->parent = parent;

    this->hwnd = CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, style, 0, 0, 0, 0, parent->Handle(), (HMENU)this->id, instance, nullptr);
    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, (LONG_PTR)this);
        this->defaultProc = (WNDPROC)SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)MessageRouter);
    }

    return this->hwnd ? true : false;
}

bool StatusBar::Create(View* parent, bool sizeGrip)
{
    if (sizeGrip)
    {
        return this->Create(parent, nullptr, SBARS_SIZEGRIP | CCS_BOTTOM | WS_CHILD);
    }
    else
    {
        return this->Create(parent);
    }
}

void StatusBar::Show()
{
    ShowWindow(this->hwnd, SW_SHOW);
}

void StatusBar::Hide()
{
    ShowWindow(this->hwnd, SW_HIDE);
}

void StatusBar::AutoResize()
{
    SendMessageW(this->hwnd, WM_SIZE, 0, 0);
    GetWindowRect(this->hwnd, &this->rect);
}

void StatusBar::ClipChildren(bool clip) const
{
    if (clip)
    {
        this->Style(this->Style() | WS_CLIPCHILDREN);
    }
    else
    {
        this->Style(this->Style() &~ WS_CLIPCHILDREN);
    }
}

DialogItem StatusBar::Item(UINT id) const
{
    return DialogItem(this->Handle(), id);
}

int StatusBar::Width() const
{
    return this->rect.right - this->rect.left;
}

int StatusBar::Height() const
{
    return this->rect.bottom - this->rect.top;
}

HWND StatusBar::Handle() const
{
    return this->hwnd;
}

DWORD StatusBar::Style() const
{
    if (this->hwnd)
    {
        return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_STYLE);
    }

    return 0;
}

DWORD StatusBar::StyleEx() const
{
    if (this->hwnd)
    {
        return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_EXSTYLE);
    }

    return 0;
}

void StatusBar::Style(DWORD style) const
{
    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWL_STYLE, style);
    }
}

void StatusBar::StyleEx(DWORD style) const
{
    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWL_EXSTYLE, style);
    }
}

bool StatusBar::SetParts(UINT parts, int* positions) const
{
    if (parts > 256)
    {
        return false;
    }

    if (!positions)
    {
        return false;
    }

    return SendMessageW(this->hwnd, SB_SETPARTS, parts, (LPARAM)positions) ? true : false;
}

UINT StatusBar::GetParts() const
{
    return (UINT)SendMessageW(this->hwnd, SB_GETPARTS, 0, 0);
}

UINT StatusBar::GetParts(std::vector<int>& positions) const
{
    positions.resize(this->GetParts());
    return (UINT)SendMessageW(this->hwnd, SB_GETPARTS, (WPARAM)positions.size(), (LPARAM)&positions[0]);
}

bool StatusBar::Text(const wchar_t* text, UINT part) const
{
    if (part > 256)
    {
        return false;
    }

    return SendMessageW(this->hwnd, SB_SETTEXTW, part, (LPARAM)text) ? true : false;
}

bool StatusBar::Text(const wstring& text, UINT part) const
{
    return this->Text(text.c_str(), part);
}

wstring StatusBar::Text(UINT part) const
{
    wstring text;

    if (part > 256)
    {
        return text;
    }

    auto l = SendMessageW(this->hwnd, SB_GETTEXTLENGTH, part, 0) & 0xFF;
    if (l)
    {
        text.resize(l);
        SendMessageW(this->hwnd, SB_GETTEXTW, part, (LPARAM)&text[0]);
    }

    return text;
}

void StatusBar::RegisterCommand(UINT command, const function<void()>& handler)
{
    this->commands[command] = handler;
}

void StatusBar::RemoveCommand(UINT command)
{
    auto it = this->commands.find(command);
    if (this->commands.end() != it)
    {
        this->commands.erase(it);
    }
}

LRESULT StatusBar::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            auto it = this->commands.find(this->command);
            if (this->commands.end() != it)
            {
                it->second();
                return 0;
            }
            else
            {
                return this->parent->Send(WM_COMMAND, this->wparam, this->lparam);
            }
        }

        default:
            return this->defaultProc(hWnd, uMsg, wParam, lParam);
    }
}

LRESULT StatusBar::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    StatusBar* pThis = reinterpret_cast<StatusBar*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (pThis)
    {
        pThis->hwnd = hWnd;
        pThis->wparam = wParam;
        pThis->lparam = lParam;
        return pThis->WindowProc(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}