#pragma once

#include <wx/wx.h>
#include <wx/stdpaths.h>

class LayoutManager
{
private:
    const wxString layoutsFilename_ = "layouts.xml";
    // const wxString layoutsPath_     = wxStandardPaths::Get().GetUserDataDir() + "\\rss\\"; USE THIS WHEN MOVING TO CORRECT PATH
    const wxString layoutsPath_     = wxStandardPaths::Get().GetResourcesDir() + "\\settings\\";
public:
    LayoutManager();
    ~LayoutManager();
};
