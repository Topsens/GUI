#pragma once

#include "View.h"
#include "DialogItem.h"
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
    void ClipChildren(bool clip = true) const;

    DialogItem Item(UINT id) const;

    int  Width() const;
    int  Height() const;

    HWND  Handle() const;
    DWORD Style() const;
    DWORD StyleEx() const;
    void  Style(DWORD style) const;
    void  StyleEx(DWORD style) const;

    bool SetParts(UINT parts, int* rightEdgePositions) const;
    UINT GetParts() const;
    UINT GetParts(std::vector<int>& positions) const;

    bool Text(const wchar_t* text, UINT part = 0) const;
    bool Text(const std::wstring& text, UINT part = 0) const;
    std::wstring Text(UINT part = 0) const;

private:
    UINT id;
    HWND hwnd;
    RECT rect;
};