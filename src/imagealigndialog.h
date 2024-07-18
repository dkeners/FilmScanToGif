#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>

#include "tools/imagecombinator.h"
#include "tools/bitmaptransforms.h"

class ImageAlignDialog : public wxDialog
{
public:
    ImageAlignDialog(wxWindow* parent, const wxString& title, Image* subImage1, Image* subImage2)
        : wxDialog(parent, wxID_ANY, title), m_image1(subImage1), m_image2(subImage2)
    {
        CombineImages();

        this->Bind(wxEVT_CHAR_HOOK, &ImageAlignDialog::HandleKeyEvent, this);
        this->Bind(wxEVT_LEFT_DOWN, &ImageAlignDialog::OnClick, this);

        // Use a sizer to layout the controls
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(m_sb, 1, wxEXPAND);
        SetSizerAndFit(sizer);
    }

private:
    Image m_combinedImage;
    wxStaticBitmap* m_sb = new wxStaticBitmap(this, wxID_ANY, m_combinedImage);
    Image* m_image1;
    Image* m_image2;

    void CombineImages()
    {
        m_combinedImage = ImageCombinator::combineImages(m_image1, m_image2);
        m_combinedImage.setPanelSize(600, 600);
        m_combinedImage.FitImage(m_sb);
        // Have to set this because of how the bitmapDisplay is in sizer.
        m_sb->SetPosition(wxPoint(0,0));
    }

    void HandleKeyEvent(wxKeyEvent& event)
    {
        int multiplier = 1;
        int qk = event.GetKeyCode();

        switch (event.GetModifiers())
        {
        case (wxMOD_CONTROL | wxMOD_SHIFT):
            multiplier = 15;
            break;
        
        case (wxMOD_CONTROL):
            multiplier = 5;
            break;

        default:
            break;
        }

        if (qk == wxKeyCode::WXK_LEFT || (qk == 97 || qk == 65))    // Left or (a or A)
        {
            m_image2->setPositionX(m_image2->getPositionX() - multiplier);
            CombineImages();
        }
        if (qk == wxKeyCode::WXK_RIGHT || (qk == 100 || qk == 68))  // Right or (d or D)
        {
            m_image2->setPositionX(m_image2->getPositionX() + multiplier);
            CombineImages();
        }
        if (qk == wxKeyCode::WXK_UP || (qk == 119 || qk == 87))     // Up or (w or W)
        {
            m_image2->setPositionY(m_image2->getPositionY() - multiplier);
            CombineImages();
        }
        if (qk == wxKeyCode::WXK_DOWN || (qk == 115 || qk == 83))   // Down or (s or S)
        {
            m_image2->setPositionY(m_image2->getPositionY() + multiplier);
            CombineImages();
        }
       
        if (qk == wxKeyCode::WXK_RETURN)
        {
            EndModal(wxID_OK);
        }
    }

    void OnClick(wxMouseEvent& event)
    {
        EndModal(wxID_OK);
    }
};
