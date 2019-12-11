#pragma once

#include "DialogItem.h"

class CheckBox : public DialogItem
{
public:
    void Check() const;
    bool IsChecked() const;
    void Uncheck() const;
    bool IsUnchecked() const;
    void Indeterminate() const;
    bool IsIndeterminate() const;
};