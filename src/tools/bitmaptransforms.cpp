#include "BitmapTransforms.h"
#include "../LoadedImage.h"

BitmapTransforms::BitmapTransforms() {};

BitmapTransforms::BitmapTransforms(wxStaticBitmap *bitmap, Image *image)
    : staticBitmap_(bitmap), image_(image) {};

BitmapTransforms::BitmapTransforms(wxWindow *parent, wxStaticBitmap *bitmap, Image *image)
    : staticBitmap_(bitmap), image_(image) {};


bool BitmapTransforms::loadAllControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image)
{
    return loadZoomControls(parent, bitmapDisplay, image) && loadMoveControls(parent, bitmapDisplay) && loadRotateControls(parent, bitmapDisplay);
}

bool BitmapTransforms::loadZoomControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image)
{
    staticBitmap_ = bitmapDisplay;
    image_ = image;

    button_zoom = new wxButton(parent, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));
    button_zoom_out = new wxButton(parent, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    button_zoom->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { zoomIn(staticBitmap_, this->image_); });
    button_zoom_out->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { zoomOut(staticBitmap_, this->image_); });
    return loadedZoomControls_ = true;
}

bool BitmapTransforms::loadMoveControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay)
{
    button_left = new wxButton(parent, wxID_ANY, "<", wxDefaultPosition, wxSize(40, 40));
    button_right = new wxButton(parent, wxID_ANY, ">", wxDefaultPosition, wxSize(40, 40));
    button_up = new wxButton(parent, wxID_ANY, "^", wxDefaultPosition, wxSize(40, 40));
    button_down = new wxButton(parent, wxID_ANY, "v", wxDefaultPosition, wxSize(40, 40));
    button_left->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { moveLeft(staticBitmap_); });
    button_right->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { moveRight(staticBitmap_); });
    button_up->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { moveUp(staticBitmap_); });
    button_down->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) { moveDown(staticBitmap_); });
    return loadedMoveControls_ = true;
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