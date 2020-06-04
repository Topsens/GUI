#include "StatusBar.h"
#include <vector>

using namespace std;

StatusBar::StatusBar(UINT id) : id(id), hwnd(nullptr), rect({ 0, 0, 0, 0})
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

    this->hwnd = CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, style, 0, 0, 0, 0, parent->Handle(), (HMENU)this->id, instance, nullptr);

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
    this->AutoResize();
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