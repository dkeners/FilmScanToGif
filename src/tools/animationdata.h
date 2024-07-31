#pragma once

#include <wx/wx.h>

struct AnimationData
{
    int frameCount;
    wxArrayString frameSequence;
    std::vector<int> frameTiming;
};