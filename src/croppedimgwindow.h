#pragma once 

#include <wx/wx.h>
#include "loadedimage.h"

class CroppedImgWindow : public wxFrame
{
public:
    CroppedImgWindow(wxFrame *parent, wxString title, Image *image);
    // ~CroppedImgWindow();
private:
    void OnComplete(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
    wxTimer m_timer;
    int m_spinnerPos = 0;
    wxStaticText *m_loading;
};