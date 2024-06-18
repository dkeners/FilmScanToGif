#include "animator.h"

namespace Animator {
    void FrameAlignment(LayoutManager* lManager, Image* image, wxString frameSequenceName)
    {
        // Get the frame sequence
        FrameSequence seq = lManager->getFrameSequence(frameSequenceName);
        // Compare the frames; 1 to 2, 2 to 3, 3 to 4, etc.
        for (int i = 0; i < seq.frameCount - 1; i++)
        {
            // Get the first frame name
            wxString frameName1 = seq.frames[i];
            // Get the second frame name
            wxString frameName2 = seq.frames[i + 1];
            // Get the sub images
            Image subImage1 = image->CustomSubImage(lManager->getFrame(frameName1));
            Image subImage2 = image->CustomSubImage(lManager->getFrame(frameName2));
            // Set focal points
            if (i == 0)
            {
                // Set the focal point for the first frame
                FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point"), &subImage1);
                focalPointDialog.ShowModal();
            }
            FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point"), &subImage2);
            focalPointDialog.ShowModal();
        }
    }
}