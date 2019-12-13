#pragma once

#include "DialogItem.h"

class ProgressBar : public DialogItem
{
public:
    bool SetRange(int min, int max);
    void GetRange(int& min, int& max);

    void Marquee(bool start, int updateInterval = 0);

    int  Position();
    void Position(int pos);
};