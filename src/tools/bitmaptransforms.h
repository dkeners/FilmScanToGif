#pragma once

#include <wx/wx.h>
#include "../LoadedImage.h"

class BitmapTransforms
{
public:
    void ZoomIn(wxStaticBitmap *bitmapDisplay, Image *image);
    void ZoomOut(wxStaticBitmap *bitmapDisplay, Image *image);
    void MoveLeft(wxStaticBitmap *bitmapDisplay);
    void MoveRight(wxStaticBitmap *bitmapDisplay);
    void MoveUp(wxStaticBitmap *bitmapDisplay);
    void MoveDown(wxStaticBitmap *bitmapDisplay);
};
