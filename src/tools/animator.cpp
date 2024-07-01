#include "animator.h"
#include "unordered_set" // std::unordered_set

namespace Animator {
    // Sub images
    std::map<wxString, Image> subImages;

    void FrameAlignment(LayoutManager* lManager, Image* image, wxString frameSequenceName)
    {
        // Get the frame sequence
        FrameSequence seq = lManager->getFrameSequence(frameSequenceName);
        // Add list of aligned frames
        std::unordered_set<wxString> alignedFrames;
        // Compare the frames; 1 to 2, 2 to 3, 3 to 4, etc.
        for (int i = 0; i < seq.frameCount - 1; i++)
        {
            // Get the first frame name
            wxString frameName1 = seq.frames[i];

            if (i == 0)
            {
                // Get the sub images
                Image subImage1 = image->CustomSubImage(lManager->getFrame(frameName1));
                // Set the focal point for the first frame
                FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point"), &subImage1);
                focalPointDialog.ShowModal();
                alignedFrames.insert(frameName1);
                subImage1.setPosition(0, 0);
                subImages[frameName1] = subImage1;
            }

            // Get the frame name
            wxString frameName2 = seq.frames[i + 1];

            // Check if the frame is already aligned
            if (alignedFrames.find(frameName2) == alignedFrames.end())
            {
                // Get the sub images
                Image subImage = image->CustomSubImage(lManager->getFrame(frameName2));
                
                // Set focal points
                FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point"), &subImage);
                focalPointDialog.ShowModal();
                alignedFrames.insert(frameName2);
                subImage.setPosition(0, 0);
                subImages[frameName2] = subImage;

                // Combine and align the images
                Image combinedImages = ImageCombinator::combineImages(&subImages[frameName1], &subImages[frameName2]);
                FocalPointDialog focalPointDialog2(nullptr, wxString("Display combined images"), &combinedImages);
                focalPointDialog2.ShowModal();
            }
        }
    }
}