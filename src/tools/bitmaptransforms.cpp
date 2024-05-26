#include "BitmapTransforms.h"
#include "../LoadedImage.h"

void BitmapTransforms::zoomIn(wxStaticBitmap *bitmapDisplay, Image *image)
{
    image->m_scale *= 1.1;
    image->ScaleImage(bitmapDisplay);
}

void BitmapTransforms::zoomOut(wxStaticBitmap *bitmapDisplay, Image *image)
{
    image->m_scale *= 0.9;
    image->ScaleImage(bitmapDisplay);
}

void BitmapTransforms::moveLeft(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x -= 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::moveRight(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x += 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::moveUp(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y -= 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::moveDown(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y += 10;
    bitmapDisplay->SetPosition(position);
}