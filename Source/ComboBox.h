#pragma once

#include "DialogItem.h"

class ComboBox : public DialogItem
{
public:
    ComboBox(HWND hWnd);

    int  Add(const wchar_t* item);
    int  Add(const std::wstring& item);
    int  Add(const wchar_t* item, void* data);
    int  Add(const std::wstring& item, void* data);
    int  Insert(int index, const wchar_t* item);
    int  Insert(int index, const std::wstring& item);
    int  Insert(int index, const wchar_t* item, void* data);
    int  Insert(int index, const std::wstring& item, void* data);
    int  Remove(int index);
    int  Count() const;
    void Clear();

    void  ItemData(int index, void* data);
    void* ItemData(int index) const;

    int  FindExact(const wchar_t* item, int first = 0) const;
    int  FindExact(const std::wstring& item, int first = 0) const;
    int  FindBeginWith(const wchar_t* item, int first = 0) const;
    int  FindBeginWith(const std::wstring& item, int first = 0) const;

    int  TextLength(int index) const;
    bool GetText(int index, std::wstring& text) const;

    int  Selection() const;
    bool Select(int index);
    void ClearSelection();

    std::wstring Text() const;
    void*        Data() const;

    static ComboBox Create(HWND parent, UINT id, DWORD type = ComboBox::DropdownList, HINSTANCE instance = nullptr);

public:
    static const DWORD Simple = CBS_SIMPLE;
    static const DWORD Dropdown = CBS_DROPDOWN;
    static const DWORD DropdownList = CBS_DROPDOWNLIST;
};