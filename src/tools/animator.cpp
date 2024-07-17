#include "animator.h"
#include "unordered_set" // std::unordered_set
#include <wx/wfstream.h>
#include <wx/anidecod.h> // wxImageArray
#include <wx/quantize.h> // wxQuantize
#include <limits.h>
#include <iostream>

// #include <wx/imaggiff.h>

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

    const int ExportAnimation(LayoutManager* lManager, wxString frameSequenceName, ImageBorderCrop borderCrop)
    {
        // Get the frame sequence
        FrameSequence seq = lManager->getFrameSequence(frameSequenceName);

        // WX file save dialog
        wxFileDialog saveFileDialog(nullptr, wxString("Save Animation"), wxString(""), wxString(""), wxString("GIF files (*.gif)|*.gif"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (saveFileDialog.ShowModal() == wxID_CANCEL)
        {
            return 2;
        }
        wxFileOutputStream output(saveFileDialog.GetPath());

        // Crop the images to border specifications
        std::unordered_set<wxString> checkedFrames;
        std::map<wxString, wxImage> croppedImages;
        int x;
        int y;
        int right;
        int bottom;
        switch (borderCrop)
        {
        case ImageBorderCrop::OuterEdge:
            break;

        case ImageBorderCrop::InnerEdge:
            x = 0;
            y = 0;
            right = INT_MAX;
            bottom = INT_MAX;

            // Find the max offset from top and left, and shortest bottom and right edges
            for (int i = 0; i < seq.frameCount; i++)
            {
                wxString frameName = seq.frames[i];
                if (checkedFrames.find(frameName) == checkedFrames.end())
                {
                    int tempX = 0;
                    int tempY = 0;
                    int tempInt = 0;
                    Image* image = &subImages[frameName];
                    if ((tempX = image->getPositionX()) > x)
                    {
                        x = tempX;
                    }
                    if ((tempY = image->getPositionY()) > y)
                    {
                        y = tempY;
                    }
                    if ((tempInt = tempX + image->getFullWidth()) < right)
                    {
                        right = tempInt;
                    }
                    if ((tempInt = tempY + image->getFullHeight()) < bottom)
                    {
                        bottom = tempInt;
                    }
                    checkedFrames.insert(frameName);
                }
            };

            // Crop the images to the found minimums
            for (auto&& frameName : checkedFrames)
            {
                Image* image = &subImages[frameName];
                
                wxRect region(x, y, right - x, bottom - y);
                region.Offset(-(image->getPositionX()), -(image->getPositionY()));

                croppedImages[frameName] = image->GetSubImage(region);
            }
            
            break;
        
        default:
            wxFAIL_MSG("Invalid border crop type");
            break;
        }
        
        // Create array of images for gif, using quantized imgs
        // new frame sequence
        wxImageArray imgArray;
        imgArray.Clear();
        imgArray.Alloc(seq.frameCount);
        // Check if frames in sequence have been aligned
        // Currently this is wastful as each frame must be "quanted" and saved, including if it repeats. It would be better to store pointers maybe and then if the frame is already saved, just add the pointer to the array again
        for (int i = 0; i < seq.frameCount; i++)
        {
            // Get the frame name
            wxString frameName = seq.frames[i];

            // Check if the frame is already aligned, if not throw an error
            if (croppedImages.find(frameName) == croppedImages.end())
            {
                // Display an error message
                wxMessageBox(wxString("Frames in sequence have not been aligned"), wxString("Error"), wxICON_ERROR);
                return 0;
            }

            // Get the sub image
            wxImage subImage = croppedImages[frameName];
            wxImage quantSubImage;
            // quantize the subImage
            wxQuantize::Quantize(subImage, quantSubImage);
            // Add the image to the array
            imgArray.Add(quantSubImage);
        }

        wxGIFHandler gifHandler;
        if (gifHandler.SaveAnimation(imgArray, &output)) {
            return 1;
        } else {
            return 0;
        }
    }
}