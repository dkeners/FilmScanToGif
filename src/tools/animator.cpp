#include "animator.h"
#include "../ui/imagealigndialog.h"
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
    
    ExportModifier operator|(ExportModifier a, ExportModifier b) {
        return static_cast<ExportModifier>(static_cast<int>(a) | static_cast<int>(b));
    }

    ExportModifier operator&(ExportModifier a, ExportModifier b) {
        return static_cast<ExportModifier>(static_cast<int>(a) & static_cast<int>(b));
    }

    ExportModifier& operator|=(ExportModifier& lhs, ExportModifier rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    void FrameAlignment(LayoutManager* lManager, Image* image, wxString frameSequenceName, bool reselectFrame1Point)
    {
        // Get the frame sequence
        FrameSequence seq = lManager->getFrameSequence(frameSequenceName);
        FrameAlignment(lManager, image, seq.frames, reselectFrame1Point);
    }

    void FrameAlignment(LayoutManager* lManager, Image* image, wxArrayString frameSequence, bool reselectFrame1Point)
    {
        // Add list of aligned frames
        std::unordered_set<wxString> alignedFrames;
        // Compare the frames; 1 to 2, 2 to 3, 3 to 4, etc.
        for (int i = 0; i < frameSequence.GetCount() - 1; i++)
        {
            // Get the first frame name
            wxString frameName1 = frameSequence[i];

            if (i == 0)
            {
                // Get the sub images
                Image subImage1 = image->CustomSubImage(lManager->getFrame(frameName1));
                // Set the focal point for the first frame
                FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point: First Image"), &subImage1);
                focalPointDialog.ShowModal();
                alignedFrames.insert(frameName1);
                subImage1.setPosition(0, 0);
                subImages[frameName1] = subImage1;
            }

            // Get the frame name
            wxString frameName2 = frameSequence[i + 1];

            // Check if the frame is already aligned
            if (alignedFrames.find(frameName2) == alignedFrames.end())
            {
                // Placing this here avoids clicking the first point if the second isn't even called.
                Image* image1 = &subImages[frameName1];
                if (i != 0 && reselectFrame1Point)
                {
                    FocalPointDialog focalPointDialog3(nullptr, wxString("Set Focal Point: First Image"), image1);
                    focalPointDialog3.ShowModal();
                }

                // Get the sub images
                Image subImage = image->CustomSubImage(lManager->getFrame(frameName2));
                
                // Set focal points
                FocalPointDialog focalPointDialog(nullptr, wxString("Set Focal Point: Second Image"), &subImage);
                focalPointDialog.ShowModal();
                alignedFrames.insert(frameName2);
                subImage.setPosition(0, 0);
                subImages[frameName2] = subImage;

                // Combine and align the images
                Image* image2 = &subImages[frameName2];
                // Immediatly set the position from the offsets
                ImageCombinator::SetSecondImagePosition(image1, image2);
                // Send two images to viewer, where they are displayed and can be adjusted
                ImageAlignDialog imageAlignCheck(nullptr, wxString("Image Alignment Preview"), image1, image2);
                if (imageAlignCheck.ShowModal() != wxID_OK)
                {
                    wxMessageBox("Frame alignment canceled.", "Warning", wxICON_WARNING);
                    return;
                }
                
            }
        }
    }

    const int ExportAnimation(LayoutManager* lManager, wxString frameSequenceName, ExportModifier modifiers, wxColour bgColor)
    {
        // Get the frame sequence
        AnimationData animationData = AnimationData();
        FrameSequence seq = lManager->getFrameSequence(frameSequenceName);
        animationData.frameCount = seq.frames.GetCount();
        animationData.frameSequence = seq.frames;
        animationData.frameTiming = lManager->getFrameTiming(frameSequenceName);
        return ExportAnimation(lManager, animationData, modifiers, bgColor);
    }

    const int ExportAnimation(LayoutManager* lManager, AnimationData animationData, ExportModifier modifiers, wxColour bgColor)
    {
        // Check each frame in sequence is aligned
        for (int i = 0; i < animationData.frameCount; i++)
        {
            wxString frameName = animationData.frameSequence[i];
            if (subImages.find(frameName) == subImages.end())
            {
                // Display an error message
                wxMessageBox(wxString("Frames in sequence have not been aligned"), wxString("Error"), wxICON_ERROR);
                return 0;
            }
        }

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
        // Crop the images to the border specifications
        if ((modifiers & ExportModifier::CropInterior) == ExportModifier::CropInterior)  // Crop to inner edge
        {
            x = 0;
            y = 0;
            right = INT_MAX;
            bottom = INT_MAX;

            // Find the max offset from top and left, and shortest bottom and right edges
            for (int i = 0; i < animationData.frameCount; i++)
            {
                wxString frameName = animationData.frameSequence[i];
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

            // Check if the images all have a common overlap
            if (x >= right || y >= bottom)
            {
                // Display an error message
                wxMessageBox(wxString("Frames in sequence have no common overlap"), wxString("Error"), wxICON_ERROR);
                return 0;
            }

            // Crop the images to the found minimums
            for (auto&& frameName : checkedFrames)
            {
                Image* image = &subImages[frameName];
                
                wxRect region(x, y, right - x, bottom - y);
                region.Offset(-(image->getPositionX()), -(image->getPositionY()));

                croppedImages[frameName] = image->GetSubImage(region);
            }
        }
        else        // Crop to outer edge
        {
            x = INT_MAX;
            y = INT_MAX;
            right = 0;
            bottom = 0;

            // Find the max offset (into negative space) from top and left, and max bottom and right edges
            for (int i = 0; i < animationData.frameCount; i++)
            {
                wxString frameName = animationData.frameSequence[i];
                if (checkedFrames.find(frameName) == checkedFrames.end())
                {
                    int tempX = 0;
                    int tempY = 0;
                    int tempInt = 0;
                    Image* image = &subImages[frameName];
                    if ((tempX = image->getPositionX()) < x)
                    {
                        x = tempX;
                    }
                    if ((tempY = image->getPositionY()) < y)
                    {
                        y = tempY;
                    }
                    if ((tempInt = tempX + image->getFullWidth()) > right)
                    {
                        right = tempInt;
                    }
                    if ((tempInt = tempY + image->getFullHeight()) > bottom)
                    {
                        bottom = tempInt;
                    }
                    checkedFrames.insert(frameName);
                }
            };

            wxString previousImageName = "";

            // Create new image the size of the maximums, fill with background color, and paste the images in the correct positions
            for (auto&& frameName : checkedFrames)
            {
                Image* image = &subImages[frameName];

                wxImage newImage(right - x, bottom - y);

                if ((modifiers & ExportModifier::StackFrames) == ExportModifier::StackFrames)  // Stack frames
                {
                    if (previousImageName != "")
                    {
                        newImage = croppedImages[previousImageName].Copy();
                    }
                    else
                    {
                        newImage.SetRGB(wxRect(0, 0, right - x, bottom - y), bgColor.Red(), bgColor.Green(), bgColor.Blue());
                    }
                    previousImageName = frameName;
                }
                else  // single frame, solid background
                {
                    newImage.SetRGB(wxRect(0, 0, right - x, bottom - y), bgColor.Red(), bgColor.Green(), bgColor.Blue());
                }

                if ((modifiers & ExportModifier::Transparent) == ExportModifier::Transparent)  // Transparent background
                {
                    int imageLeftBound = image->getPositionX();
                    int imageTopBound = image->getPositionY();
                    int imageRightBound = imageLeftBound + image->getFullWidth();
                    int imageBottomBound = imageTopBound + image->getFullHeight();

                    unsigned char* alphaData = (unsigned char*)malloc(newImage.GetWidth() * newImage.GetHeight());

                    for (int i = 0; i < newImage.GetWidth() * newImage.GetHeight(); i++)
                    {
                        int x = i % newImage.GetWidth();
                        int y = i / newImage.GetWidth();
                        if (x >= imageLeftBound && x < imageRightBound && y >= imageTopBound && y < imageBottomBound)
                        {
                            alphaData[i] = 255;
                        }
                        else
                        {
                            alphaData[i] = 0;
                        }
                    }

                    newImage.SetAlpha(alphaData);
                }

                newImage.Paste(*image, (image->getPositionX() - x), (image->getPositionY() - y));
                
                croppedImages[frameName] = newImage;
            }
        }
        
        // Create array of images for gif, using quantized imgs
        // new frame sequence
        wxImageArray imgArray;
        imgArray.Clear();
        imgArray.Alloc(animationData.frameCount);
        // Check if frames in sequence have been aligned
        // Currently this is wastful as each frame must be "quanted" and saved, including if it repeats. It would be better to store pointers maybe and then if the frame is already saved, just add the pointer to the array again
        for (int i = 0; i < animationData.frameCount; i++)
        {
            // Get the frame name
            wxString frameName = animationData.frameSequence[i];
            // Get the sub image
            // wxImage subImage = croppedImages[frameName];
            // wxImage quantSubImage;
            // quantize the subImage
            wxImage quantSubImage = croppedImages[frameName].Copy();
            wxQuantize::Quantize(quantSubImage, quantSubImage);

            if (quantSubImage.HasAlpha())
            {
                quantSubImage.ConvertAlphaToMask();
            }
            // // If transparent, move transparency to the quantized image
            // if ((modifiers & ExportModifier::Transparent) == ExportModifier::Transparent)
            // {
            //     unsigned char* originalAlpha = subImage.GetAlpha();
            //     unsigned char* newAlpha = (unsigned char*)malloc(subImage.GetWidth() * subImage.GetHeight());

            //     memcpy(newAlpha, originalAlpha, subImage.GetWidth() * subImage.GetHeight());

            //     quantSubImage.SetAlpha(newAlpha);
            //     quantSubImage.ConvertAlphaToMask();
            //     free(newAlpha);
            // }
            // Save image to file
            int palattecount = quantSubImage.GetPalette().GetColoursCount();
            quantSubImage.SaveFile("D:\\Downloads - HDD\\contadopierde_danforth-portra-160-3877_2024-02-25_1435\\DANFORTH PORTRA 160 3877\\Transparent Test tRUE" + frameName + ".png", wxBITMAP_TYPE_PNG);
            // Add the image to the array
            imgArray.Add(quantSubImage);
        }

        wxGIFHandler gifHandler;
        if (gifHandler.SaveAnimation(imgArray, &output, true, animationData.frameTiming)) {
            return 1;
        } else {
            return 0;
        }
    }
}