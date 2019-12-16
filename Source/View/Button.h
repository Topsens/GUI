#pragma once

#include "DialogItem.h"

class Button : public DialogItem
{
public:
    Button(const DialogItem& di);
    
    void SetBitmap(HBITMAP bitmap);
    bool SetBitmap(const wchar_t* file);
    void SetIcon(HICON icon);

private:
    HANDLE img;
};