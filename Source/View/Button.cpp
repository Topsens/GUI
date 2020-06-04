#include "Button.h"

Button Button::Create(HWND parent, UINT id, const wchar_t* text, HINSTANCE instance)
{
    DialogItem di;

    if (parent)
    {
        auto hwnd = CreateWindowExW(0, L"BUTTON", text, WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, parent, (HMENU)id, instance, nullptr);
        if (hwnd)
        {
            di = DialogItem(parent, hwnd, id);
        }
    }

    return (Button&)di;
}