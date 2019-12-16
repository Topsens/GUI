#include "Button.h"

Button::Button(const DialogItem& di) : DialogItem(di.Handle(), di.ID())
{
}

void Button::SetBitmap(HBITMAP bitmap)
{
    this->Send(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bitmap);
}

bool Button::SetBitmap(const wchar_t* file)
{
    if (!file)
    {
        return false;
    }

    return true;
}

void Button::SetIcon(HICON icon)
{
    this->Send(BM_SETIMAGE, IMAGE_ICON, (LPARAM)icon);
}