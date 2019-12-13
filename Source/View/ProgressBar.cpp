#include "ProgressBar.h"
#include <CommCtrl.h>

bool ProgressBar::SetRange(int min, int max)
{
    if (max <= min)
    {
        return false;
    }

    this->Send(PBM_SETRANGE32, (WPARAM)min, (LPARAM)max);
    return true;
}

void ProgressBar::GetRange(int& min, int& max)
{
    PBRANGE range;
    this->Send(PBM_GETRANGE, 0, (LPARAM)&range);

    min = range.iLow;
    max = range.iHigh;
}

void ProgressBar::Marquee(bool start, int interval)
{
    this->Send(PBM_SETMARQUEE, start ? TRUE : FALSE, (LPARAM)interval);
}

int ProgressBar::Position()
{
    return (int)this->Send(PBM_GETPOS);
}

void ProgressBar::Position(int pos)
{
    this->Send(PBM_SETPOS, (WPARAM)pos);
}