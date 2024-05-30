#pragma once

#include "subbmprect.h"

#include <map>

#include <wx/wx.h>
#include <wx/stdpaths.h>

class LayoutManager
{
private:
    const wxString layoutsFilename_ = "layouts.xml";
    // const wxString layoutsPath_     = wxStandardPaths::Get().GetUserDataDir() + "\\rss\\"; USE THIS WHEN MOVING TO CORRECT PATH
    const wxString layoutsPath_     = wxStandardPaths::Get().GetResourcesDir() + "\\settings\\";

    int currentLayoutIndex_;
    
public:
    wxString defaultLayout_;
    wxArrayString layouts_;
    wxArrayString layoutsFileArray_;
    wxArrayString layoutsImgArray_;

    std::map<wxString, SubBmpRect> frames_;

    LayoutManager();
    bool loadLayout(wxString layoutName = "default");
    ~LayoutManager();
};
