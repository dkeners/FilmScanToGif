#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>

class FocalPointDialog : public wxDialog
{
public:
    FocalPointDialog(wxWindow* parent, const wxString& title, Image* m_subImage)
        : wxDialog(parent, wxID_ANY, title), m_subImage(m_subImage)
    {
        // Create a wxStaticBitmap to display the image
        wxStaticBitmap* sb = new wxStaticBitmap(this, wxID_ANY, *m_subImage);
        m_subImage->setPanelSize(600, 600);
        m_subImage->FitImage(sb);

        // Bind the mouse click event to the OnClick method
        sb->Bind(wxEVT_LEFT_DOWN, &FocalPointDialog::OnClick, this);

        // Use a sizer to layout the controls
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(sb, 1, wxEXPAND);
        SetSizerAndFit(sizer);
    }

private:
    Image* m_subImage;

    void OnClick(wxMouseEvent& event)
    {
        // Get the position of the mouse click
        wxPoint position = event.GetPosition();
        position.x = position.x / m_subImage->m_scale;
        position.y = position.y / m_subImage->m_scale;

        // Do something with the position...
        m_subImage->setMarkedPoint(position);

        // Close the dialog
        EndModal(wxID_OK);
    }
};