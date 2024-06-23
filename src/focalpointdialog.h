#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>

class FocalPointDialog : public wxDialog
{
public:
    FocalPointDialog(wxWindow* parent, const wxString& title, Image* subImage1)
        : wxDialog(parent, wxID_ANY, title), subImage1(subImage1)
    {
        // Create a wxStaticBitmap to display the image
        wxStaticBitmap* bitmap = new wxStaticBitmap(this, wxID_ANY, *subImage1);

        // Bind the mouse click event to the OnClick method
        bitmap->Bind(wxEVT_LEFT_DOWN, &FocalPointDialog::OnClick, this);

        // Use a sizer to layout the controls
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(bitmap, 1, wxEXPAND);
        SetSizerAndFit(sizer);
    }

private:
    Image* subImage1;

    void OnClick(wxMouseEvent& event)
    {
        // Get the position of the mouse click
        wxPoint position = event.GetPosition();

        // Do something with the position...
        subImage1->setMarkedPoint(position);
        wxLogMessage("Clicked at %d, %d", position.x, position.y);

        // Close the dialog
        EndModal(wxID_OK);
    }
};