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
    enum class ImageBorderCrop {
        OuterEdge,
        InnerEdge,
    };

    /**
     * @brief Aligns a frame within an animation.
     *
     * This function is responsible for aligning a frame within an animation.
     *
     * @param lManager A pointer to the layout manager.
     * @param image A pointer to the image.
     * @param animationName (Optional) The name of the animation. Defaults to `"default"`.
     * @param reselectFrame1Point (Optional) Creates an extra dialog to change the focal point of the first image; good if the old point is not visible in the new image. Defaults to `false`.
     */
    void FrameAlignment(LayoutManager* lManager, Image* image, wxString animationName = "default", bool reselectFrame1Point = false);
    
    /**
     * @brief Exports the animation using the specified layout manager.
     *
     * @param lManager Pointer to the main LayoutManager object.
     * @param animationName (Optional) Name of the animation. Defaults to `"default"`.
     * @param borderCrop (Optional) Crop the image border. Defaults to `ImageBorderCrop::InnerEdge`.
     */
    const int ExportAnimation(LayoutManager* lManager, wxString animationName = "default", ImageBorderCrop borderCrop = ImageBorderCrop::InnerEdge);
}