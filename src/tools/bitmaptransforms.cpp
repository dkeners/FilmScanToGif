#include "BitmapTransforms.h"
#include "../LoadedImage.h"

BitmapTransforms::BitmapTransforms(wxStaticBitmap *bitmap, Image *image, wxWindow *parent)
    : staticBitmap_(bitmap), image_(image), parent_(parent) {};


bool BitmapTransforms::loadAllControls()
{
    return loadZoomControls() && loadMoveControls() && loadRotateControls();
}

bool BitmapTransforms::loadAllControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image)
{
    return loadZoomControls(parent, bitmapDisplay, image) && loadMoveControls(parent, bitmapDisplay, image) && loadRotateControls(parent, bitmapDisplay);
}

bool BitmapTransforms::loadZoomControls()
{
    if (parent_ == nullptr || staticBitmap_ == nullptr || image_ == nullptr)
    {
        wxLogError("Parent, static bitmap, and image must be assigned before loading zoom controls.");
        return false;
    }
    return loadZoomControls(parent_, staticBitmap_, image_);
}

bool BitmapTransforms::loadZoomControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image)
{
    button_zoom = new wxButton(parent, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));
    button_zoom_out = new wxButton(parent, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    button_zoom->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->zoomIn(bitmapDisplay); });
    button_zoom_out->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->zoomOut(bitmapDisplay); });
    return loadedZoomControls_ = true;
}

bool BitmapTransforms::loadMoveControls()
{
    if (parent_ == nullptr || staticBitmap_ == nullptr)
    {
        throw std::invalid_argument("Parent and static bitmap must be assigned before loading move controls.");
        return false;
    }
    
    return loadMoveControls(parent_, staticBitmap_, image_);
}

bool BitmapTransforms::loadMoveControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image)
{
    button_left = new wxButton(parent, wxID_ANY, "<", wxDefaultPosition, wxSize(40, 40));
    button_right = new wxButton(parent, wxID_ANY, ">", wxDefaultPosition, wxSize(40, 40));
    button_up = new wxButton(parent, wxID_ANY, "^", wxDefaultPosition, wxSize(40, 40));
    button_down = new wxButton(parent, wxID_ANY, "v", wxDefaultPosition, wxSize(40, 40));
    button_left->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->moveLeft(bitmapDisplay); });
    button_right->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->moveRight(bitmapDisplay); });
    button_up->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->moveUp(bitmapDisplay); });
    button_down->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, bitmapDisplay, image](wxCommandEvent& event) { image->moveDown(bitmapDisplay); });
    return loadedMoveControls_ = true;
}

bool BitmapTransforms::loadRotateControls()
{
    if (parent_ == nullptr || staticBitmap_ == nullptr)
    {
        throw std::invalid_argument("Parent and static bitmap must be assigned before loading rotate controls.");
        return false;
    }
    return loadRotateControls(parent_, staticBitmap_);
}

bool BitmapTransforms::loadRotateControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay)
{
    button_rotate_left = new wxButton(parent, wxID_ANY, "↺", wxDefaultPosition, wxSize(40, 40));
    button_rotate_right = new wxButton(parent, wxID_ANY, "↻", wxDefaultPosition, wxSize(40, 40));
    return loadedRotateControls_ = true;
}

wxBoxSizer *BitmapTransforms::createControlSizer(int padding)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    if (loadedZoomControls_)
    {
        sizer->Add(button_zoom, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
        sizer->Add(button_zoom_out, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
    }
    if (loadedMoveControls_)
    {
        sizer->Add(button_left, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
        sizer->Add(button_right, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
        sizer->Add(button_up, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
        sizer->Add(button_down, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
    }
    if (loadedRotateControls_)
    {
        sizer->Add(button_rotate_left, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
        sizer->Add(button_rotate_right, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, padding);
    }
    sizer->AddStretchSpacer();
    return sizer;
}