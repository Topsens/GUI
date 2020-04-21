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

void StatusBar::Show()
{
    this->AutoResize();
    ShowWindow(this->hwnd, SW_SHOW);
}

void StatusBar::Hide()
{
    ShowWindow(this->hwnd, SW_HIDE);
    this->rect = { 0, 0, 0, 0 };
}

void StatusBar::AutoResize()
{
    SendMessageW(this->hwnd, WM_SIZE, 0, 0);
    GetWindowRect(this->hwnd, &this->rect);
}

int StatusBar::Width()
{
    return this->rect.right - this->rect.left;
}

int StatusBar::Height()
{
    return this->rect.bottom - this->rect.top;
}

bool StatusBar::SetParts(UINT parts, int* positions)
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

UINT StatusBar::GetParts()
{
    return SendMessageW(this->hwnd, SB_GETPARTS, 0, 0);
}

UINT StatusBar::GetParts(std::vector<int>& positions)
{
    positions.resize(this->GetParts());
    return SendMessageW(this->hwnd, SB_GETPARTS, (WPARAM)positions.size(), (LPARAM)&positions[0]);
}

bool StatusBar::Text(const wchar_t* text, UINT part)
{
    if (part > 256)
    {
        return false;
    }

    return SendMessageW(this->hwnd, SB_SETTEXTW, part, (LPARAM)text) ? true : false;
}

bool StatusBar::Text(const wstring& text, UINT part)
{
    return this->Text(text.c_str(), part);
}

wstring StatusBar::Text(UINT part)
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