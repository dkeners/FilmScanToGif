#pragma once

#include <wx/wx.h>

#include "../loadedimage.h"

enum class BlendMode
{
    NORMAL_,
    AVERAGE_,
    DARKEN_,
    LIGHTEN_,
    MULTIPLY_,
    SCREEN_,
    OVERLAY_,
    HARD_LIGHT_,
    SOFT_LIGHT_,
    DIFFERENCE_
};

namespace ImageCombinator
{
    // Methods to call a comparer window with the first and second images
    Image combineImages(Image *image1, Image *image2);
    wxPoint GetRelativeOffset(Image *image1, Image *image2);
    wxRect GetOuterBounds(Image *image1, Image *image2);
    wxRect GetInnerBounds(Image *image1, Image *image2);
    void pixelCombine(wxImage *combinedImage, Image *image1, Image *image2);
    wxColour blendPixel(unsigned char* r1, unsigned char* g1, unsigned char* b1,
                        unsigned char* r2, unsigned char* g2, unsigned char* b2,
                        BlendMode mode);
    unsigned char blendNormal(unsigned char* a, unsigned char* b);
    unsigned char blendAverage(unsigned char* a, unsigned char* b);
    unsigned char blendDarken(unsigned char* a, unsigned char* b);
    unsigned char blendLighten(unsigned char* a, unsigned char* b);
    unsigned char blendMultiply(unsigned char* a, unsigned char* b);
    unsigned char blendScreen(unsigned char* a, unsigned char* b);
    unsigned char blendOverlay(unsigned char* a, unsigned char* b);
    unsigned char blendHardLight(unsigned char* a, unsigned char* b);
    unsigned char blendSoftLight(unsigned char* a, unsigned char* b);
    unsigned char blendDifference(unsigned char* a, unsigned char* b);
} // namespace Image
