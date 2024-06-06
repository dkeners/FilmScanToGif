#pragma once

#include <map>

#include <wx/wx.h>

struct FrameSequence {
    int frameCount;
    wxArrayString frames;
};

using FrameSequenceMap = std::map<wxString, FrameSequence>;