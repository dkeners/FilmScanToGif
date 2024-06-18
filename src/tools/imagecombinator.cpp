#include "imagecombinator.h"

namespace ImageCombinator
{
    void combineImages(Image *image1, Image *image2)
    {
        // Create a comparer window
        // ComparerWindow *comparer = new ComparerWindow(nullptr, wxT("Image Comparer"), firstImage, secondImage);
        // comparer->Show(true);
    }

    wxRect GetOuterBounds(Image *image1, Image *image2)
    {
        // generate offsets based on marked points
        wxPoint offsetI2 = wxPoint(
            image1->getPositionX() + image1->getMarkedPointX() - image2->getMarkedPointX(),
            image1->getPositionY() + image1->getMarkedPointY() - image2->getMarkedPointY()
        );

        int leftBound = std::min(image1->getPositionX(), offsetI2.x);
        int rightBound = std::max(image1->getPositionX() + image1->GetWidth(), offsetI2.x + image2->GetWidth());
        int topBound = std::min(image1->getPositionY(), offsetI2.y);
        int bottomBound = std::max(image1->getPositionY() + image1->GetHeight(), offsetI2.y + image2->GetHeight());

        return wxRect(leftBound, topBound, rightBound - leftBound, bottomBound - topBound);
    }
}