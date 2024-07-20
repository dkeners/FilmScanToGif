#include "croppedimgwindow.h"

#include "../tools/bitmaptransforms.h"

CroppedImgWindow::CroppedImgWindow(wxFrame *parent, wxString title, Image *subImage)
    : wxFrame(parent, wxID_ANY, title)
{
    SetSize(600, 800);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    // wxBitmap bitmap(wxT("D:\\Nextcloud\\Documents\\Code\\wxWidgetsTests\\src\\settings\\Nimslo3DLayout.png"), wxBITMAP_TYPE_PNG);
    wxStaticBitmap *sb = new wxStaticBitmap(this, wxID_ANY, *subImage);
    sb->SetSize(wxSize(600, 600));
    subImage->setPanelSize(600, 600);
    subImage->FitImage(sb);
    vbox->Add(sb, 0, wxALL, 10);

    // ascii loading spinner
    m_loading = new wxStaticText(this, wxID_ANY, wxT("Loading..."));
    vbox->Add(m_loading, 0, wxRIGHT, 8);

    m_timer.Bind(wxEVT_TIMER, &CroppedImgWindow::OnTimer, this);
    m_timer.Start(150);  // Start the timer to fire every 100ms
    
    // BUTTON CONTROLS
    BitmapTransforms controls_(sb, subImage, this);
    controls_.loadMoveControls();

    // Export Image
    wxButton *button_upload_image = new wxButton(this, wxID_ANY, "Export Image", wxDefaultPosition, wxSize(100, 40));
    button_upload_image->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &CroppedImgWindow::OnComplete, this);

    // Button Controls Sizer
    wxBoxSizer *sizer_image_controls = controls_.createControlSizer();

    sizer_image_controls->Add(button_upload_image, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    // END BUTTON CONTROLS
    
    vbox->Add(sizer_image_controls, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxBOTTOM, 5);
    // wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    // sizer->Add(hbox, 0, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(vbox);

    Centre();
}

void CroppedImgWindow::OnComplete(wxCommandEvent& event)
{
    m_timer.Stop();
    m_loading->SetLabel("Complete");
}

void CroppedImgWindow::OnTimer(wxTimerEvent& event)
{
    static const char spinnerChars[] = "|/-\\";
    wxString text = "Loading ";
    text += spinnerChars[m_spinnerPos];
    m_loading->SetLabel(text);

    m_spinnerPos = (m_spinnerPos + 1) % (sizeof(spinnerChars) - 1);
}