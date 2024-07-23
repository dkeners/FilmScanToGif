#include "exportdialog.h"

ExportDialog::ExportDialog(wxWindow* parent, const wxString& title, LayoutManager& layout_manager)
    : wxDialog(parent, wxID_ANY, title), m_layout_manager(layout_manager)
{
    // Create a wxStaticText to display the export options
    wxStaticText* stTitle = new wxStaticText(this, wxID_ANY, "Export options:");
            
    // Create a wxCheckBox for the crop interior option
    m_cbCropInterior = new wxCheckBox(this, wxID_ANY, "Crop Interior");
    m_cbCropInterior->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
    {
        m_cbStackFrames->Enable(!m_cbCropInterior->IsChecked());
        m_cbTransparent->Enable(!m_cbCropInterior->IsChecked());
        ToggleRGB(!m_cbTransparent->IsChecked());
    });

    // Create a wxCheckBox for the stack frames option
    m_cbStackFrames = new wxCheckBox(this, wxID_ANY, "Stack Frames");
    m_cbStackFrames->SetValue(true);

    // Create a wxCheckBox for the transparent option
    m_cbTransparent = new wxCheckBox(this, wxID_ANY, "Transparent");
    m_cbTransparent->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent& event)
    {
        ToggleRGB(!m_cbTransparent->IsChecked());
    });

    // Create 3 integers inputs for the RGB values
    wxSize inputSize(50, -1);
    wxIntegerValidator<int> RGBValidator;
    RGBValidator.SetRange(0, 255);
    m_tcRed = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, inputSize, wxTE_CENTER, RGBValidator);
    m_tcGreen = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, inputSize, wxTE_CENTER, RGBValidator);
    m_tcBlue = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, inputSize, wxTE_CENTER, RGBValidator);
    m_tcRed->SetMaxLength(3);
    m_tcGreen->SetMaxLength(3);
    m_tcBlue->SetMaxLength(3);
    
    // Create a wxButton to export the animation
    wxButton* btnExport = new wxButton(this, wxID_ANY, "Export");
    btnExport->Bind(wxEVT_BUTTON, &ExportDialog::OnExport, this);
    
    // Use a main_sizer to layout the controls
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
        main_sizer->Add(stTitle, 0, wxEXPAND|wxALL, 5);

        wxStaticBoxSizer* imgDisplay_sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Image Display");
            imgDisplay_sizer->Add(m_cbCropInterior, 0, wxEXPAND|wxALL, 5);
            imgDisplay_sizer->Add(m_cbStackFrames, 0, wxEXPAND|wxALL, 5);
        main_sizer->Add(imgDisplay_sizer, 0, wxEXPAND|wxALL, 5);

        wxStaticBoxSizer* bgColor_sizer = new wxStaticBoxSizer(wxVERTICAL, this, "Background Settings");
            bgColor_sizer->Add(m_cbTransparent, 0, wxEXPAND|wxALL, 5);

            wxStaticLine* horizontalBar = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
            bgColor_sizer->Add(horizontalBar, 0, wxEXPAND|wxALL, 5);

            wxStaticBoxSizer* RGB_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "RGB Value");
                wxBoxSizer* sizerRed = new wxBoxSizer(wxVERTICAL);
                    sizerRed->Add(new wxStaticText(this, wxID_ANY, "Red"), 0, wxEXPAND|wxALL, 5);
                    sizerRed->Add(m_tcRed, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);
                RGB_sizer->Add(sizerRed, 0, wxEXPAND|wxALL, 5);
                wxBoxSizer* sizerGreen = new wxBoxSizer(wxVERTICAL);
                    sizerGreen->Add(new wxStaticText(this, wxID_ANY, "Green"), 0, wxEXPAND|wxALL, 5);
                    sizerGreen->Add(m_tcGreen, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);
                RGB_sizer->Add(sizerGreen, 0, wxEXPAND|wxALL, 5);
                wxBoxSizer* sizerBlue = new wxBoxSizer(wxVERTICAL);
                    sizerBlue->Add(new wxStaticText(this, wxID_ANY, "Blue"), 0, wxEXPAND|wxALL, 5);
                    sizerBlue->Add(m_tcBlue, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);
                RGB_sizer->Add(sizerBlue, 0, wxEXPAND|wxALL, 5);
            bgColor_sizer->Add(RGB_sizer, 0, wxEXPAND|wxALL, 5);
        main_sizer->Add(bgColor_sizer, 0, wxEXPAND|wxALL, 5);

        main_sizer->Add(btnExport, 0, wxEXPAND|wxLEFT|wxRIGHT|wxBOTTOM, 5);
    SetSizerAndFit(main_sizer);

    this->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event)
    {
        OnClose(event);
    });
}

void ExportDialog::OnExport(wxCommandEvent& event)
{
    // Gather the export options...
    Animator::ExportModifier modifiers = Animator::ExportModifier::None;
    if (m_cbCropInterior->IsChecked())
        modifiers = modifiers | Animator::ExportModifier::CropInterior;
    if (m_cbStackFrames->IsChecked())
        modifiers = modifiers | Animator::ExportModifier::StackFrames;
    if (m_cbTransparent->IsChecked())
    {
        modifiers = modifiers | Animator::ExportModifier::Transparent;
        // Export the animation...
        EndModal(Animator::ExportAnimation(&m_layout_manager, "default", modifiers));
    }
    else
    {
        wxColor color = wxColour(wxAtoi(m_tcRed->GetValue()), wxAtoi(m_tcGreen->GetValue()), wxAtoi(m_tcBlue->GetValue()));
        // Export the animation...
        EndModal(Animator::ExportAnimation(&m_layout_manager, "default", modifiers, color));
    }
}

void ExportDialog::ToggleRGB(bool enable)
{
    m_tcRed->Enable(enable);
    m_tcGreen->Enable(enable);
    m_tcBlue->Enable(enable);
}

void ExportDialog::OnClose(wxCloseEvent& event)
{
    // 2 is the return code for the export being cancelled
    EndModal(2);
}