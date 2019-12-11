#pragma once

#include "DialogItem.h"

class ComboBox : public DialogItem
{
public:
    int  Add(const wchar_t* item);
    int  Add(const std::wstring& item);
    int  Insert(const wchar_t* item, int index);
    int  Insert(const std::wstring& item, int index);
    int  Remove(int index);
    int  Count();
    void Clear();

    int  FindExact(const wchar_t* item, int first = 0);
    int  FindExact(const std::wstring& item, int first = 0);
    int  FindBeginWith(const wchar_t* item, int first = 0);
    int  FindBeginWith(const std::wstring& item, int first = 0);

    int  TextLength(int index);
    bool GetText(int index, std::wstring& text);

    int  Selection();
    bool Select(int index);
    void ClearSelection();
};