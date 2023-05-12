#include "DialogItem.h"
#include <sstream>
#include <vector>

using namespace std;

DialogItem::DialogItem() : hwnd(0)
{
}

DialogItem::DialogItem(HWND hWnd) : hwnd(hWnd)
{
}

DialogItem::~DialogItem()
{
    if (this->hwnd)
    {
        this->RemoveProp(L"~");
    }
}

void DialogItem::Destroy()
{
    if (this->hwnd)
    {
        DestroyWindow(this->hwnd);
        this->hwnd = 0;
    }
}

DWORD DialogItem::Style() const
{
    if (this->hwnd)
    {
        return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_STYLE);
    }

    return 0;
}

DWORD DialogItem::StyleEx() const
{
    if (this->hwnd)
    {
        return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_EXSTYLE);
    }

    return 0;
}

void DialogItem::Style(DWORD style)
{
    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWL_STYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
}

void DialogItem::StyleEx(DWORD style)
{
    if (this->hwnd)
    {
        SetWindowLongPtrW(this->hwnd, GWL_EXSTYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
}

void DialogItem::Show() const
{
    ShowWindow(this->hwnd, SW_SHOW);
}

void DialogItem::Hide() const
{
    ShowWindow(this->hwnd, SW_HIDE);
}

bool DialogItem::MoveTo(int x, int y, bool repaint) const
{
    if (this->hwnd)
    {
        return MoveWindow(this->hwnd, x, y, this->Width(), this->Height(), repaint ? TRUE : FALSE) ? true : false;
    }

    return false;
}

bool DialogItem::Resize(int width, int height, bool repaint) const
{
    if (this->hwnd)
    {
        return MoveWindow(this->hwnd, this->X(), this->Y(), width, height, repaint ? TRUE : FALSE) ? true : false;
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
    PostMessageW(this->Parent(), WM_NEXTDLGCTL, 0, 0);
}

LRESULT DialogItem::Send(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return SendMessageW(this->hwnd, uMsg, wParam, lParam);
}

bool DialogItem::Post(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return FALSE != PostMessageW(this->hwnd, uMsg, wParam, lParam);
}

bool DialogItem::SetProp(const wchar_t* name, HANDLE data)
{
    return ::SetPropW(this->hwnd, name, data) ? true : false;
}

HANDLE DialogItem::GetProp(const wchar_t* name)
{
    return ::GetPropW(this->hwnd, name);
}

HANDLE DialogItem::RemoveProp(const wchar_t* name)
{
    return ::RemovePropW(this->hwnd, name);
}

int DialogItem::X() const
{
    POINT c2scr = {0};
    RECT  rect;

    if (ClientToScreen(this->Parent(), &c2scr) &&
        GetWindowRect(this->hwnd, &rect))
    {
        return rect.left - c2scr.x;
    }

    return 0;
}

int DialogItem::Y() const
{
    POINT c2scr = {0};
    RECT  rect;

    if (ClientToScreen(this->Parent(), &c2scr) &&
        GetWindowRect(this->hwnd, &rect))
    {
        return rect.top - c2scr.y;
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

void DialogItem::Font(HFONT font) const
{
    if (this->hwnd)
    {
        SendMessageW(this->hwnd, WM_SETFONT, (WPARAM)font, TRUE);
    }
}

void DialogItem::Text(const wchar_t* text) const
{
    SetWindowTextW(this->hwnd, text);
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

int DialogItem::ID() const
{
    return GetDlgCtrlID(this->hwnd);
}

HWND DialogItem::Handle() const
{
    return this->hwnd;
}

HWND DialogItem::Parent() const
{
    return GetParent(this->hwnd);
}

void DialogItem::Subclass()
{
    if (!this->hwnd)
    {
        return;
    }

    auto WndProc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        auto _this = ::GetPropW(hWnd, L"~");
        if (_this)
        {
            return ((DialogItem*)_this)->WndProc(hWnd, uMsg, wParam, lParam);
        }

        return DialogItem::DefWndProc(hWnd, uMsg, wParam, lParam);
    };

    auto defProc = SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

    if (!GetWindowLongPtrW(this->hwnd, GWLP_USERDATA))
    {
        SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, defProc);
    }

    this->SetProp(L"~", (HANDLE)this);
}

LRESULT DialogItem::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_DESTROY == uMsg)
    {
        this->hwnd = 0;
    }

    return DialogItem::DefWndProc(hWnd, uMsg, wParam, lParam);
}

DialogItem::operator bool() const
{
    return nullptr != this->hwnd;
}

DialogItem::operator HWND() const
{
    return this->hwnd;
}

HFONT DialogItem::CreateFont(const wchar_t* family, int size, int weight, bool italic, bool underline, bool strikeOut, DWORD charSet, DWORD outPrecision, DWORD clipPrecision, DWORD quality, DWORD pitchAndFamity, int escapement, int orientation)
{
    return ::CreateFontW(size, 0, escapement, orientation, weight, italic ? TRUE : FALSE, underline ? TRUE : FALSE, strikeOut ? TRUE : FALSE, charSet, outPrecision, clipPrecision, quality, pitchAndFamity, family);
}

void DialogItem::DestroyFont(HFONT font)
{
    if (font)
    {
        DeleteObject(font);
    }
}

WNDPROC DialogItem::Subclass(DialogItem& item, WNDPROC WndProc)
{
    if (!item.Handle() || !WndProc)
    {
        return nullptr;
    }

    auto defProc = SetWindowLongPtrW(item.Handle(), GWLP_WNDPROC, (LONG_PTR)WndProc);

    if (!GetWindowLongPtrW(item.Handle(), GWLP_USERDATA))
    {
        SetWindowLongPtrW(item.Handle(), GWLP_USERDATA, defProc);
    }

    return (WNDPROC)defProc;
}

LRESULT DialogItem::DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA))(hWnd, uMsg, wParam, lParam);
}