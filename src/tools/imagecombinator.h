#pragma once

#include <wx/wx.h>

#include "../loadedimage.h"

namespace ImageCombinator
{
    // Methods to call a comparer window with the first and second images
    void combineImages(Image *image1, Image *image2);
    wxRect GetOuterBounds(Image *image1, Image *image2);
} // namespace Image
