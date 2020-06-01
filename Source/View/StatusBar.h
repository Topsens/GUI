#pragma once

#include "View.h"
#include <vector>
#include <CommCtrl.h>

class StatusBar
{
public:
    StatusBar(UINT controlID);

    operator bool() const;

    bool Create(View* parent, HINSTANCE instance = nullptr, DWORD style = CCS_BOTTOM | WS_CHILD);
    bool Create(View* parent, bool sizeGrip);
    void Show();
    void Hide();
    void AutoResize();

    int  Width();
    int  Height();

    bool SetParts(UINT parts, int* rightEdgePositions);
    UINT GetParts();
    UINT GetParts(std::vector<int>& positions);

    bool Text(const wchar_t* text, UINT part = 0);
    bool Text(const std::wstring& text, UINT part = 0);
    std::wstring Text(UINT part = 0);

private:
    UINT id;
    HWND hwnd;
    RECT rect;
};