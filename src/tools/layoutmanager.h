#pragma once

#include "subbmprect.h"
#include "framesequence.h"
#include "frametiming.h"

#include <map>

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <wx/stdpaths.h>

class LayoutManager
{
public:
    LayoutManager();
    ~LayoutManager();

    bool loadLayout(wxString layoutName = "default");
    wxString getLayoutName();
    SubBmpRect getFrame(wxString frameName);   

private:
    const wxString LAYOUTS_FILENAME = "layouts.xml";
    // CHANGE BELOW TO (for release):                  Get().GetUserDataDir() + "\\settings\\";
    const wxString LAYOUTS_PATH     = wxStandardPaths::Get().GetResourcesDir() + "\\settings\\";

    // Initial variables
    wxString m_defaultLayout;
    wxArrayString m_layouts;
    wxArrayString m_layoutsFileArray;
    wxArrayString m_layoutsImgArray;

    // LayoutInfo variables
    wxString m_layoutName;
    int m_frameCount;
    wxSize m_layoutSize;

    // Animation variables
    FrameTimingMap m_frameTimings;
    FrameSequenceMap m_frameSequences;

    // Frame variables
    std::map<wxString, SubBmpRect> m_frames;

    int m_currentLayoutIndex;
    
};
