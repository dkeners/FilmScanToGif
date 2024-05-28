#include "layoutmanager.h"

#include <wx/xml/xml.h>

LayoutManager::LayoutManager()
{
    wxXmlDocument doc;
    if (!doc.Load(layoutsPath_ + layoutsFilename_))
    {
        wxLogError("Failed to load layout file: %s", layoutsPath_ + layoutsFilename_);
    }

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
            // wxString layoutFile_ = layoutsPath_ + layoutsNode->GetAttribute("layoutFile");
            // wxString layoutImg_ = layoutsPath_ + layoutsNode->GetAttribute("layoutImg");
            wxString layoutFile_ = layoutsNode->GetAttribute("layoutFile");
            wxString layoutImg_ = layoutsNode->GetAttribute("layoutImg");
            wxLogMessage("Layout: %s. Files located at: %s, %s", name, layoutFile_, layoutImg_);
            layouts_.Add(name);
            layoutsFileArray_.Add(layoutFile_);
            layoutsImgArray_.Add(layoutImg_);
        }
        layoutsNode = layoutsNode->GetNext();
    }

    defaultLayout_ = root->GetAttribute("default");
    // wxLogMessage("Default layout: %s", defaultLayout_);
    layouts_.Shrink();
    layoutsFileArray_.Shrink();
    layoutsImgArray_.Shrink();
    // wxLogMessage("Loaded %i layouts", layouts_.GetCount());
}

bool LayoutManager::loadLayout(wxString layoutName)
{
    if (layoutName == "default") {
        layoutName = defaultLayout_;
    }

    currentLayoutIndex_ = layouts_.Index(layoutName);
    if (currentLayoutIndex_ == wxNOT_FOUND)
    {
        wxLogError("Layout not found: %s", layoutName);
        return false;
    }

    wxXmlDocument doc;
    if (!doc.Load(layoutsPath_ + layoutsFileArray_[currentLayoutIndex_]))
    {
        wxLogError("Failed to load layout file: %s", layoutsPath_ + layoutsFileArray_[currentLayoutIndex_]);
        return false;
    }

    wxLogMessage("Loaded layout file: %s", layoutsPath_ + layoutsFileArray_[currentLayoutIndex_]);

    wxXmlNode *root = doc.GetDocumentNode()->GetChildren();

    if (root->GetName() != "layout")
    {
        wxLogError("Invalid layout file: %s", layoutsPath_ + layoutsFileArray_[currentLayoutIndex_]);
        return false;
    }

    wxXmlNode *layoutNode = root->GetChildren();
    while (layoutNode)
    {
        // Layout Info Node Logic (name, frames, layoutSize)
        if (layoutNode->GetName() == "layoutInfo")
        {
            wxXmlNode *layoutInfo = layoutNode->GetChildren();
            while (layoutInfo)
            {
                if (layoutInfo->GetName() == "name")
                {
                    wxLogMessage("Camera Info: %s", layoutInfo->GetNodeContent());
                } else if (layoutInfo->GetName() == "frames") {
                    wxLogMessage("Layout total frames: %s", layoutInfo->GetNodeContent());
                } else if (layoutInfo->GetName() == "layoutSize") {
                    wxXmlNode *dimensions = layoutInfo->GetChildren();
                    while (dimensions)
                    {
                        if (dimensions->GetName() == "width")
                        {
                            wxLogMessage("Layout width: %s", dimensions->GetNodeContent());
                        } else if (dimensions->GetName() == "height") {
                            wxLogMessage("Layout height: %s", dimensions->GetNodeContent());
                        }
                        dimensions = dimensions->GetNext();
                    }
                }
                layoutInfo = layoutInfo->GetNext();
            }
            wxLogMessage("Camera Info: %s", layoutNode->GetAttribute("name"));
        }
        // Animation Info Node Logic (frameTiming, animationOrder)
        else if (layoutNode->GetName() == "animationInfo") {
            wxXmlNode *animationInfo = layoutNode->GetChildren();
            while (animationInfo)
            {
                if (animationInfo->GetName() == "frameTiming") {
                    wxXmlNode *frameTiming = animationInfo->GetChildren();
                    while (frameTiming)
                    {
                        if (frameTiming->GetName() == "default") {
                            wxLogMessage("Default frame timing: %s", frameTiming->GetNodeContent());
                        } else {
                            wxLogMessage("%s Frame timing: %s", frameTiming->GetName(), frameTiming->GetNodeContent());
                        }
                        frameTiming = frameTiming->GetNext();
                    }
                } else if (animationInfo->GetName() == "animationOrder") {
                    wxXmlNode *order = animationInfo->GetChildren();
                    while (order)
                    {
                        if (order->GetName() == "default") {
                            wxXmlNode *orderNode = order->GetChildren();
                            while (orderNode)
                            {
                                wxLogMessage("Default order: %s", orderNode->GetNodeContent());
                                orderNode = orderNode->GetNext();
                            }
                        } else {
                            wxLogMessage("%s Order: %s", order->GetName(), order->GetNodeContent());
                        }
                        order = order->GetNext();
                    }
                }
                animationInfo = animationInfo->GetNext();
            }
        }
        // Frame Node Logic (name, x, y, width, height)
        else if (layoutNode->GetName() == "frameInfo") {
            wxXmlNode *frame = layoutNode->GetChildren();
            while (frame)
            {
                wxLogMessage("Frame: %s", frame->GetAttribute("name"));
                wxString name = frame->GetAttribute("name");
                int x, y, width, height;

                wxXmlNode *frameInfo = frame->GetChildren();
                while (frameInfo)
                {
                    if (frameInfo->GetName() == "x") {
                        x = wxAtoi(frameInfo->GetNodeContent());
                        wxLogMessage("X: %s", frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "y") {
                        y = wxAtoi(frameInfo->GetNodeContent());
                        wxLogMessage("Y: %s", frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "width") {
                        width = wxAtoi(frameInfo->GetNodeContent());
                        wxLogMessage("Width: %s", frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "height") {
                        height = wxAtoi(frameInfo->GetNodeContent());
                        wxLogMessage("Height: %s", frameInfo->GetNodeContent());
                    }
                    frameInfo = frameInfo->GetNext();
                }
                frames_[name] = SubBmpRect(x, y, width, height);
                wxLogMessage("Frame: %i, %i, %i, %i", frames_[name].GetX(), frames_[name].GetY(), frames_[name].GetWidth(), frames_[name].GetHeight());
                frame = frame->GetNext();
            }
        }
        layoutNode = layoutNode->GetNext();
    }
    

    return true;
}

LayoutManager::~LayoutManager()
{
    
}