#include "croppedimgwindow.h"

CroppedImgWindow::CroppedImgWindow(wxFrame *parent, wxString title)
    : wxFrame(parent, wxID_ANY, title)
{
    SetSize(600, 800);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *st = new wxStaticText(panel, wxID_ANY, wxT("Cropped Image"));
    hbox->Add(st, 0, wxRIGHT, 8);

    wxBitmap bitmap(wxT("D:\\Nextcloud\\Documents\\Code\\wxWidgetsTests\\src\\settings\\Nimslo3DLayout.png"), wxBITMAP_TYPE_PNG);
    wxStaticBitmap *sb = new wxStaticBitmap(panel, wxID_ANY, bitmap);
    hbox->Add(sb, 1);
    vbox->Add(hbox, 0, wxALL, 10);

    // ascii loading spinner
    wxStaticText *loading = new wxStaticText(panel, wxID_ANY, wxT("Loading..."));
    vbox->Add(loading, 0, wxRIGHT, 8);
    
    
    

    // wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    // sizer->Add(hbox, 0, wxEXPAND | wxALL, 10);
    panel->SetSizerAndFit(vbox);

    Centre();
}