#pragma once

#include "View.h"
#include <CommCtrl.h>

class StatusBar
{
public:
    StatusBar(UINT controlID);

    operator bool() const;

    bool Create(View* parent, HINSTANCE instance = nullptr, DWORD style = CCS_BOTTOM | WS_CHILD);
    void Show();
    void Hide();
    void AutoResize();

    int  Width();
    int  Height();

    void Text(const wchar_t* text);
    void Text(const std::wstring& text);
    std::wstring Text();

private:
    UINT id;
    HWND hwnd;
    RECT rect;
};