#include "BitmapTransforms.h"
#include "../LoadedImage.h"

void BitmapTransforms::ZoomIn(wxStaticBitmap *bitmapDisplay, Image *image)
{
    image->m_scale *= 1.1;
    image->ScaleImage(bitmapDisplay);
}

void BitmapTransforms::ZoomOut(wxStaticBitmap *bitmapDisplay, Image *image)
{
    image->m_scale *= 0.9;
    image->ScaleImage(bitmapDisplay);
}

void BitmapTransforms::MoveLeft(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x -= 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::MoveRight(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x += 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::MoveUp(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y -= 10;
    bitmapDisplay->SetPosition(position);
}

void BitmapTransforms::MoveDown(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y += 10;
    bitmapDisplay->SetPosition(position);
}