#include "layoutmanager.h"

LayoutManager::LayoutManager()
{
    wxXmlDocument doc;
    if (!doc.Load(LAYOUTS_PATH + LAYOUTS_FILENAME))
    {
        wxLogError("Failed to load layout file: %s\nPlease load a layout before attempting any actions.", LAYOUTS_PATH + LAYOUTS_FILENAME);
        return;
    }

    wxXmlNode *root = doc.GetDocumentNode()->GetChildren();

    if (root->GetName() != "layouts")
    {
        wxLogError("Invalid layout file: %s", LAYOUTS_PATH + LAYOUTS_FILENAME);
    }

    wxXmlNode *layoutsNode = root->GetChildren();
    while (layoutsNode)
    {
        if (layoutsNode->GetName() == "layout")
        {
            wxString name = layoutsNode->GetAttribute("name");
            wxString layoutFile = layoutsNode->GetAttribute("layoutFile");
            wxString layoutImg = layoutsNode->GetAttribute("layoutImg");
            m_layouts.Add(name);
            m_layoutsFileArray.Add(layoutFile);
            m_layoutsImgArray.Add(layoutImg);
        }
        layoutsNode = layoutsNode->GetNext();
    }

    m_defaultLayout = root->GetAttribute("default");

    m_layouts.Shrink();
    m_layoutsFileArray.Shrink();
    m_layoutsImgArray.Shrink();
}

bool LayoutManager::loadLayout(wxString layoutName)
{
    if (layoutName == "default") {
        layoutName = m_defaultLayout;
    }

    m_currentLayoutIndex = m_layouts.Index(layoutName);
    if (m_currentLayoutIndex == wxNOT_FOUND)
    {
        wxLogError("Layout not found: %s", layoutName);
        return false;
    }

    wxXmlDocument doc;
    if (!doc.Load(LAYOUTS_PATH + m_layoutsFileArray[m_currentLayoutIndex]))
    {
        wxLogError("Failed to load layout file: %s", LAYOUTS_PATH + m_layoutsFileArray[m_currentLayoutIndex]);
        return false;
    }

    wxXmlNode *root = doc.GetDocumentNode()->GetChildren();

    if (root->GetName() != "layout")
    {
        wxLogError("Invalid layout file: %s", LAYOUTS_PATH + m_layoutsFileArray[m_currentLayoutIndex]);
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
                    m_layoutName = layoutInfo->GetNodeContent();
                } else if (layoutInfo->GetName() == "frames") {
                    m_frameCount = wxAtoi(layoutInfo->GetNodeContent());
                } else if (layoutInfo->GetName() == "layoutSize") {
                    int width, height;

                    wxXmlNode *dimensions = layoutInfo->GetChildren();
                    while (dimensions)
                    {
                        if (dimensions->GetName() == "width")
                        {
                            width = wxAtoi(dimensions->GetNodeContent());
                        } else if (dimensions->GetName() == "height") {
                            height = wxAtoi(dimensions->GetNodeContent());
                        }
                        dimensions = dimensions->GetNext();
                    }
                    m_layoutSize = wxSize(width, height);
                }
                layoutInfo = layoutInfo->GetNext();
            }
        }
        // Animation Info Node Logic (frameTiming, animationOrder)
        else if (layoutNode->GetName() == "animationInfo") {
            wxXmlNode *animationInfo = layoutNode->GetChildren();
            while (animationInfo)
            {
                if (animationInfo->GetName() == "frameTiming") {
                    m_frameTimings.clear();
                    wxXmlNode *frameTiming = animationInfo->GetChildren();
                    while (frameTiming)
                    {
                        wxString name = frameTiming->GetName();

                        std::vector<int> delays;
                        wxXmlNode *timeNode = frameTiming->GetChildren();
                        while (timeNode)
                        {
                            if (timeNode->GetName() == "time") {
                                delays.push_back(wxAtoi(timeNode->GetNodeContent()));
                            }
                            timeNode = timeNode->GetNext();
                        }

                        m_frameTimings[name] = delays;

                        frameTiming = frameTiming->GetNext();
                    }
                } else if (animationInfo->GetName() == "animationOrder") {
                    m_frameSequences.clear();
                    wxXmlNode *order = animationInfo->GetChildren();
                    while (order)
                    {
                        wxString name = order->GetName();
                        int totalFrames = wxAtoi(order->GetAttribute("totalFrames"));

                        wxArrayString frames;
                        wxXmlNode *frameNode = order->GetChildren();
                        while (frameNode)
                        {
                            if (frameNode->GetName() == "frame") {
                                frames.Add(frameNode->GetNodeContent());
                            }
                            frameNode = frameNode->GetNext();
                        }

                        FrameSequence sequence = {totalFrames, frames};
                        m_frameSequences[name] = sequence;

                        order = order->GetNext();
                    }
                }
                animationInfo = animationInfo->GetNext();
            }
        }
        // Frame Node Logic (name, x, y, width, height)
        else if (layoutNode->GetName() == "frameInfo") {
            m_frames.clear();
            
            wxXmlNode *frame = layoutNode->GetChildren();
            while (frame)
            {
                wxString name = frame->GetAttribute("name");
                int x, y, width, height;

                wxXmlNode *frameInfo = frame->GetChildren();
                while (frameInfo)
                {
                    if (frameInfo->GetName() == "x") {
                        x = wxAtoi(frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "y") {
                        y = wxAtoi(frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "width") {
                        width = wxAtoi(frameInfo->GetNodeContent());
                    } else if (frameInfo->GetName() == "height") {
                        height = wxAtoi(frameInfo->GetNodeContent());
                    }
                    frameInfo = frameInfo->GetNext();
                }
                m_frames[name] = SubBmpRect(x, y, width, height);
                frame = frame->GetNext();
            }
        }
        layoutNode = layoutNode->GetNext();
    }
    return true;
}

wxString LayoutManager::getLayoutName()
{
    return m_layoutName;
}

wxArrayString LayoutManager::getLayouts()
{
    return m_layouts;
}

std::vector<int> LayoutManager::getFrameTiming(wxString frameTimingName)
{
    return m_frameTimings[frameTimingName];
}

FrameSequence LayoutManager::getFrameSequence(wxString frameSequenceName)
{
    return m_frameSequences[frameSequenceName];
}

int LayoutManager::getFrameCount()
{
    return m_frameCount;
}

SubBmpRect LayoutManager::getFrame(wxString frameName)
{
    return m_frames[frameName];
}

LayoutManager::~LayoutManager()
{
    
}