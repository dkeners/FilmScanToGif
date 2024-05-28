#pragma once

#include <wx/wx.h>

class SubBmpRect : public wxRect
{
    public:
        SubBmpRect();
        SubBmpRect(int x, int y, int width, int height);
        bool scaleFrame(double scale);
        bool isScaled() { return scaled_; };
    private:
        bool scaled_ = false;
};