#include "imagecombinator.h"
// For performance testing
// #include <iostream>
// #include <chrono>

namespace ImageCombinator
{
    Image combineImages(Image *image1, Image *image2)
    {
        wxRect outerBounds = GetOuterBounds(image1, image2);

        // Create a new image with the outer bounds
        Image combinedImage = Image();
        combinedImage.Create(outerBounds.width, outerBounds.height);

        // Combine the images
        pixelCombine(&combinedImage, image1, image2);

        // Modify image data based on new positioning
        return combinedImage;
    }

    wxPoint GetRelativeOffset(Image *image1, Image *image2)
    {
        return wxPoint(
            image1->getPositionX() + image1->getMarkedPointX() - image2->getMarkedPointX(),
            image1->getPositionY() + image1->getMarkedPointY() - image2->getMarkedPointY()
        );
    }

    wxRect GetOuterBounds(Image *image1, Image *image2)
    {
        // generate offsets based on marked points
        wxPoint offsetImage2 = GetRelativeOffset(image1, image2);

        int leftBound = std::min(image1->getPositionX(), offsetImage2.x);
        int rightBound = std::max(image1->getPositionX() + image1->GetWidth(), offsetImage2.x + image2->GetWidth());
        int topBound = std::min(image1->getPositionY(), offsetImage2.y);
        int bottomBound = std::max(image1->getPositionY() + image1->GetHeight(), offsetImage2.y + image2->GetHeight());

        return wxRect(leftBound, topBound, rightBound - leftBound, bottomBound - topBound);
    }

    wxRect GetInnerBounds(Image *image1, Image *image2)
    {
        // generate offsets based on marked points
        wxPoint offsetImage2 = GetRelativeOffset(image1, image2);

        int leftBound = std::max(image1->getPositionX(), offsetImage2.x);
        int rightBound = std::min(image1->getPositionX() + image1->GetWidth(), offsetImage2.x + image2->GetWidth());
        int topBound = std::max(image1->getPositionY(), offsetImage2.y);
        int bottomBound = std::min(image1->getPositionY() + image1->GetHeight(), offsetImage2.y + image2->GetHeight());

        return wxRect(leftBound, topBound, rightBound - leftBound, bottomBound - topBound);
    }

    void pixelCombine(wxImage *combinedImage, Image *image1, Image *image2)
    {
        // Set new image2 location based on offset from image1
        wxPoint offsetImage2 = GetRelativeOffset(image1, image2);
        image2->setPositionX(image2->getPositionX() + offsetImage2.x);
        image2->setPositionY(image2->getPositionY() + offsetImage2.y);

        // Find top left of images to offset for data manipulation
        wxRect outerBounds = GetOuterBounds(image1, image2);
        wxRect innerBounds = GetInnerBounds(image1, image2);

        wxPoint boundsLocation = outerBounds.GetPosition();
        outerBounds.SetPosition(wxPoint(0, 0));
        innerBounds.SetPosition(innerBounds.GetPosition() - boundsLocation);

        // Create variables for relative offsets
        int xImage1 = image1->getPositionX() - boundsLocation.x;
        int yImage1 = image1->getPositionY() - boundsLocation.y;

        int xImage2 = image2->getPositionX() - boundsLocation.x;
        int yImage2 = image2->getPositionY() - boundsLocation.y;
        
        // Gather image data sources
        unsigned int pixelCount = combinedImage->GetWidth() * combinedImage->GetHeight();
        unsigned char *combinedData = combinedImage->GetData();
        unsigned char *image1Data = image1->GetData();
        unsigned char *image2Data = image2->GetData();

        // Commented timer for performance testing
        // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (unsigned int i = 0; i < pixelCount * 3; i += 3)
        {
            // Get relative coordinates in data, (x, y) = (i % width, i / width)
            unsigned int globalX = i % (combinedImage->GetWidth() * 3);
            unsigned int globalY = i / (combinedImage->GetWidth() * 3);

            // // Return early if outside of any image (check if pixel is left and above inner bounds, right and above inner bounds, left and below inner bounds, right and below inner bounds)
            // if ((globalX < innerBounds.GetX() * 3 && globalY < innerBounds.GetY()) ||
            //     (globalX >= (innerBounds.GetX() + innerBounds.GetWidth()) * 3 && globalY < innerBounds.GetY()) ||
            //     (globalX < innerBounds.GetX() * 3 && globalY >= innerBounds.GetY() + innerBounds.GetHeight()) ||
            //     (globalX >= (innerBounds.GetX() + innerBounds.GetWidth()) * 3 && globalY >= innerBounds.GetY() + innerBounds.GetHeight()))
            // {
            //     // Set the combined pixel to black
            //     combinedData[i] = 0;
            //     combinedData[i + 1] = 0;
            //     combinedData[i + 2] = 0;
            //     continue;
            // }

            // // If global coordinates are within the bounds of either image, get the pixel data
            // if (globalX >= image1->getPositionX() * 3 && globalX < (image1->getPositionX() + image1->GetWidth()) * 3 &&
            //     globalY >= image1->getPositionY() && globalY < image1->getPositionY() + image1->GetHeight()
            // {
            //     /* code */
            // }
            
            unsigned char* image1Red = nullptr;
            unsigned char* image1Green = nullptr;
            unsigned char* image1Blue = nullptr;
            unsigned char* image2Red = nullptr;
            unsigned char* image2Green = nullptr;
            unsigned char* image2Blue = nullptr;

            // Check if the pixel is within the bounds of image 1
            if (globalX >= (xImage1 * 3) && globalX < (xImage1 + image1->GetWidth()) * 3 && globalY >= yImage1 && globalY < (yImage1 + image1->GetHeight())) {
                image1Red = &image1Data[(globalY - yImage1) * image1->GetWidth() * 3 + (globalX - (xImage1 * 3))];
                image1Green = &image1Data[(globalY - yImage1) * image1->GetWidth() * 3 + (globalX - (xImage1 * 3)) + 1];
                image1Blue = &image1Data[(globalY - yImage1) * image1->GetWidth() * 3 + (globalX - (xImage1 * 3)) + 2];
            }
            // Check if the pixel is within the bounds of image 2
            if (globalX >= (xImage2 * 3) && globalX < (xImage2 + image2->GetWidth()) * 3 && globalY >= yImage2 && globalY < (yImage2 + image2->GetHeight())){
                image2Red = &image2Data[(globalY - yImage2) * image2->GetWidth() * 3 + (globalX - (xImage2 * 3))];
                image2Green = &image2Data[(globalY - yImage2) * image2->GetWidth() * 3 + (globalX - (xImage2 * 3)) + 1];
                image2Blue = &image2Data[(globalY - yImage2) * image2->GetWidth() * 3 + (globalX - (xImage2 * 3)) + 2];
            }

            // Combine the pixels
            wxColour combinedPixel = blendPixel(
                image1Red, image1Green, image1Blue,
                image2Red, image2Green, image2Blue,
                BlendMode::DIFFERENCE_);

            // Set the combined pixel
            combinedData[i] = combinedPixel.Red();
            combinedData[i + 1] = combinedPixel.Green();
            combinedData[i + 2] = combinedPixel.Blue();
        }
        // Commented timer for performance testing
        // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    }

    wxColour blendPixel(unsigned char* r1, unsigned char* g1, unsigned char* b1,
                        unsigned char* r2, unsigned char* g2, unsigned char* b2,
                        BlendMode mode) {
        auto blend = [&](unsigned char* a, unsigned char* b) -> unsigned char {
            if (a == nullptr && b == nullptr) return 0;
            if (a == nullptr) return *b;
            if (b == nullptr) return *a;
            switch (mode) {
                case BlendMode::NORMAL_: return blendNormal(a, b);
                case BlendMode::AVERAGE_: return blendAverage(a, b);
                case BlendMode::DIFFERENCE_: return blendDifference(a, b);
                default: return 0;
            }
        };

        return wxColour(
            blend(r1, r2),
            blend(g1, g2),
            blend(b1, b2)
        );
    }

    unsigned char blendNormal(unsigned char* a, unsigned char* b)
    {
        return *b;
    }

    unsigned char blendAverage(unsigned char* a, unsigned char* b)
    {
        return (*a + *b) / 2;
    }
    
    unsigned char blendDifference(unsigned char* a, unsigned char* b)
    {
        return abs(*a - *b);
    }
}