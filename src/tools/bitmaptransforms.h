#pragma once

#include <wx/wx.h>
#include "../LoadedImage.h"

namespace BitmapTransforms
{
    void zoomIn(wxStaticBitmap *bitmapDisplay, Image *image);
    void zoomOut(wxStaticBitmap *bitmapDisplay, Image *image);
    void moveLeft(wxStaticBitmap *bitmapDisplay);
    void moveRight(wxStaticBitmap *bitmapDisplay);
    void moveUp(wxStaticBitmap *bitmapDisplay);
    void moveDown(wxStaticBitmap *bitmapDisplay);
};
