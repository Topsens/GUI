#include "DialogItem.h"
#include <sstream>
#include <vector>

using namespace std;

DialogItem::DialogItem(HWND parent, UINT id)
  : parent(parent), id(id)
{
    this->hwnd = GetDlgItem(parent, id);
}

void DialogItem::Show() const
{
    ShowWindow(this->hwnd, SW_SHOW);
}

void DialogItem::Hide() const
{
    ShowWindow(this->hwnd, SW_HIDE);
}

bool DialogItem::Move(int x, int y, bool repaint) const
{
    return this->Move(x, y, this->Width(), this->Height(), repaint);
}

bool DialogItem::Move(int x, int y, int width, int height, bool repaint) const
{
    if (!this->hwnd || width < 0 || height < 0)
    {
        return false;
    }

    return FALSE != MoveWindow(this->hwnd, x, y, width, height, repaint ? TRUE : FALSE);
}

bool DialogItem::Resize(int width, int height, bool repaint) const
{
    if (this->hwnd)
    {
        auto rect = this->Rect();
        return this->Move(rect.left, rect.top, width, height, repaint);
    }

    return false;
}

void DialogItem::Enable() const
{
    EnableWindow(this->hwnd, TRUE);
}

void DialogItem::Disable() const
{
    EnableWindow(this->hwnd, FALSE);
}

bool DialogItem::IsEnabled() const 
{
    return FALSE != IsWindowEnabled(this->hwnd);
}

bool DialogItem::IsVisible() const
{
    return FALSE != IsWindowVisible(this->hwnd);
}

bool DialogItem::HasFocus() const
{
    return GetFocus() == this->hwnd;
}

HWND DialogItem::SetFocus() const
{
    return ::SetFocus(this->hwnd);
}

void DialogItem::NextTabStop() const
{
    PostMessageW(this->parent, WM_NEXTDLGCTL, 0, 0);
}

LRESULT DialogItem::Send(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return SendMessageW(this->hwnd, uMsg, wParam, lParam);
}

bool DialogItem::Post(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return FALSE != PostMessageW(this->hwnd, uMsg, wParam, lParam);
}

int DialogItem::X() const
{
    RECT rect;
    if (GetWindowRect(this->hwnd, &rect))
    {
        return rect.left;
    }

    return 0;
}

int DialogItem::Y() const
{
    RECT rect;
    if (GetWindowRect(this->hwnd, &rect))
    {
        return rect.top;
    }

    return 0;
}

int DialogItem::Width() const
{
    RECT rect;
    if (GetWindowRect(this->hwnd, &rect))
    {
        return rect.right - rect.left;
    }

    return 0;
}

int DialogItem::Height() const
{
    RECT rect;
    if (GetWindowRect(this->hwnd, &rect))
    {
        return rect.bottom - rect.top;
    }

    return 0;
}

RECT DialogItem::Rect() const
{
    RECT rect;
    GetWindowRect(this->hwnd, &rect);
    return rect;
}

bool DialogItem::Font(const wchar_t* family, int size, int weight, bool italic, bool underline, bool strikeOut, DWORD charSet, DWORD outPrecision, DWORD clipPrecision, DWORD quality, DWORD pitchAndFamity, int escapement, int orientation) const
{
    if (this->hwnd)
    {
        auto font = CreateFontW(size, 0, escapement, orientation, weight, italic ? TRUE : FALSE, underline ? TRUE : FALSE, strikeOut ? TRUE : FALSE, charSet, outPrecision, clipPrecision, quality, pitchAndFamity, family);

        if (font)
        {
            SendMessageW(this->hwnd, WM_SETFONT, (WPARAM)font, TRUE);
            return true;
        }
    }

    return false;
}

void DialogItem::Text(const wchar_t* text) const
{
    SetDlgItemTextW(this->parent, id, text);
}

void DialogItem::Text(const wstring& text) const
{
    this->Text(text.c_str());
}

wstring DialogItem::Text() const
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

void DialogItem::Check() const
{
    SendMessageW(this->hwnd, BM_SETCHECK, BST_CHECKED, 0);
}

bool DialogItem::IsChecked() const
{
    return BST_CHECKED == SendMessageW(this->hwnd, BM_GETCHECK, 0, 0);
}

void DialogItem::Uncheck() const
{
    SendMessageW(this->hwnd, BM_SETCHECK, BST_UNCHECKED, 0);
}

bool DialogItem::IsUnchecked() const
{
    return BST_UNCHECKED == SendMessageW(this->hwnd, BM_GETCHECK, 0, 0);
}

void DialogItem::Indeterminate() const
{
    SendMessageW(this->hwnd, BM_SETCHECK, BST_INDETERMINATE, 0);
}

bool DialogItem::IsIndeterminate() const
{
    return BST_INDETERMINATE == SendMessageW(this->hwnd, BM_SETCHECK, BST_INDETERMINATE, 0);
}

UINT DialogItem::ID() const
{
    return this->id;
}

HWND DialogItem::Handle() const
{
    return this->hwnd;
}

DialogItem::operator bool()
{
    return nullptr != this->hwnd;
}