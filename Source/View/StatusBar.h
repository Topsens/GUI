#pragma once

#include "View.h"
#include <functional>
#include <map>
#include <vector>
#include <CommCtrl.h>

class StatusBar
{
public:
    StatusBar(UINT controlID);
   ~StatusBar();

    operator bool() const;

    bool Create(View* parent, HINSTANCE instance = nullptr, DWORD style = CCS_BOTTOM | WS_CHILD);
    bool Create(View* parent, bool sizeGrip);
    void Destroy();
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

    void RegisterCommand(UINT command, const std::function<void()>& handler);
    void RemoveCommand(UINT command);

protected:
    virtual LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
    static LRESULT MessageRouter(HWND, UINT, WPARAM, LPARAM);

private:
    UINT id;
    HWND hwnd;
    RECT rect;

    View* parent;

    WNDPROC defaultProc;
    WORD    command;
    WPARAM  wparam;
    LPARAM  lparam;
    
    std::map<UINT, std::function<void()>> commands;
};