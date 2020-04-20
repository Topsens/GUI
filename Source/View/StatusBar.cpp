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

void StatusBar::Text(const wchar_t* text)
{
    SetWindowTextW(this->hwnd, text);
}

void StatusBar::Text(const wstring& text)
{
    this->Text(text.c_str());
}

wstring StatusBar::Text()
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