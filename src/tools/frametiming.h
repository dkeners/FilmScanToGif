#pragma once

#include <map>

#include <wx/wx.h>

struct FrameTiming {
    wxArrayInt timings;
};

using FrameTimingMap = std::map<wxString, FrameTiming>;