#include "layoutmanager.h"

#include <wx/xml/xml.h>

LayoutManager::LayoutManager()
{
    wxXmlDocument doc;
    if (!doc.Load(layoutsPath_ + layoutsFilename_))
    {
        wxLogError("Failed to load layout file: %s", layoutsPath_ + layoutsFilename_);
    }
    // if (!doc.Load("D:\\Nextcloud\\Documents\\Code\\wxWidgetsTests\\build\\rss\\layouts.xml")) {
    //     wxLogError("Failed to load layout file: %s", "D:\\Nextcloud\\Documents\\Code\\wxWidgetsTests\\build\\rss\\layouts.xml");
    // }

    wxLogMessage("Loaded layout file: %s", layoutsPath_ + layoutsFilename_);


    wxXmlNode *root = doc.GetDocumentNode()->GetChildren();

    if (root->GetName() != "layouts")
    {
        wxLogError("Invalid layout file: %s", layoutsPath_ + layoutsFilename_);
    }

    wxXmlNode *layoutsNode = root->GetChildren();
    while (layoutsNode)
    {
        if (layoutsNode->GetName() == "layout")
        {
            wxString name = layoutsNode->GetAttribute("name");
            wxString layoutFile_ = layoutsPath_ + layoutsNode->GetAttribute("layoutFile");
            wxString layoutImg_ = layoutsPath_ + layoutsNode->GetAttribute("layoutImg");
            wxLogMessage("Layout: %s. Files located at: %s, %s", name, layoutFile_, layoutImg_);
        }
        layoutsNode = layoutsNode->GetNext();
    }
}

LayoutManager::~LayoutManager()
{
    
}