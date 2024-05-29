#pragma once

#include <wx/wx.h>
#include "../LoadedImage.h"

class BitmapTransforms
{
public:
    BitmapTransforms();
    BitmapTransforms(wxStaticBitmap *bitmap, Image *image);
    BitmapTransforms(wxWindow *parent, wxStaticBitmap *bitmap, Image *image);

    wxButton *button_zoom;
    wxButton *button_zoom_out;
    wxButton *button_left;
    wxButton *button_right;
    wxButton *button_up;
    wxButton *button_down;
    wxButton *button_rotate_left;
    wxButton *button_rotate_right;

    bool loadAllControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image);
    bool loadZoomControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image);
    bool loadMoveControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay);
    bool loadRotateControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay);
    wxBoxSizer *createControlSizer(int padding = 5);

    void zoomIn(wxStaticBitmap *bitmapDisplay, Image *image);
    void zoomOut(wxStaticBitmap *bitmapDisplay, Image *image);
    void moveLeft(wxStaticBitmap *bitmapDisplay);
    void moveRight(wxStaticBitmap *bitmapDisplay);
    void moveUp(wxStaticBitmap *bitmapDisplay);
    void moveDown(wxStaticBitmap *bitmapDisplay);
private:
    wxStaticBitmap *staticBitmap_;
    Image *image_;
};
