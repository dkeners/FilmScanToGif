#pragma once

#include <wx/wx.h>

#include "subbmprect.h"
#include "layoutmanager.h"
#include "imagecombinator.h"
#include "../loadedimage.h"
#include "../focalpointdialog.h"

/**
 * @namespace Animator
 * @brief The Animator namespace contains classes and functions for animating images.
 */
namespace Animator {
    // Methods to call a comparer window with the first and second images
    // void compareImages(wxString firstImage, wxString secondImage);
    void FrameAlignment(LayoutManager* lManager, Image* image, wxString animationName = "default");
}