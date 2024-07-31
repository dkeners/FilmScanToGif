#include "animationcontrols.h"

AnimationControls::AnimationControls(wxFrame* parent, LayoutManager* layoutManager)
    : m_parent(parent), m_lManager(layoutManager)
{
}

AnimationControls::~AnimationControls()
{
}

void AnimationControls::CreateControls()
{
    // CREATE CONTROLS
    // ComboBox for Possible Frame Sequences
    m_combo_frame_sequence = new wxComboBox(m_parent, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, m_lManager->getFrameSequenceNames(), wxCB_READONLY);
    m_combo_frame_sequence->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedSequence = m_combo_frame_sequence->GetValue();
        UpdateAnimationTable();
        m_text_frame_sequence_custom->Enable(selectedSequence == "custom");  // Enable if custom sequence is selected
        m_parent->SetStatusText("Selected Frame Sequence: " + selectedSequence);
        });
    m_combo_frame_sequence->Append("custom");
    // TextControl for Custom Frame Sequence
    m_text_frame_sequence_custom = new wxTextCtrl(m_parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_text_frame_sequence_custom->Enable(false);
    m_text_frame_sequence_custom->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
        UpdateAnimationTable();
        });
    // ComboBox for Possible Frame Timings
    m_combo_frame_timing = new wxComboBox(m_parent, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, m_lManager->getFrameTimingNames(), wxCB_READONLY);
    m_combo_frame_timing->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedTiming = m_combo_frame_timing->GetValue();
        UpdateAnimationTable();
        m_text_frame_timing_custom->Enable(selectedTiming == "custom");  // Enable if custom timing is selected
        m_parent->SetStatusText("Selected Frame Timing: " + selectedTiming);
        });
    m_combo_frame_timing->Append("custom");
    // TextControl for Custom Frame Timing
    m_text_frame_timing_custom = new wxTextCtrl(m_parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_text_frame_timing_custom->Enable(false);
    m_text_frame_timing_custom->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
        UpdateAnimationTable();
        });
    // ListView for Frame and Delay Selection
    m_listview_animation = new wxListView(m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    m_listview_animation->AppendColumn("Frame", wxLIST_FORMAT_LEFT, 75);
    m_listview_animation->AppendColumn("Delay", wxLIST_FORMAT_LEFT, 75);
    UpdateAnimationTable();
    m_listview_animation->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent& event) {
        m_parent->SetStatusText("Selected Frame: " + m_listview_animation->GetItemText(event.GetIndex()) + " with Delay: " + m_listview_animation->GetItemText(event.GetIndex(), 1));
        });
    // Checkbox for Single Focus Point
    m_checkbox_single_focus = new wxCheckBox(m_parent, wxID_ANY, "Set Moving Focus Point");
    m_checkbox_single_focus->SetValue(true);
    
    // CREATE SIZERS
    sizer_main = new wxStaticBoxSizer(wxHORIZONTAL, m_parent, "Animation Controls");
        wxBoxSizer* sizer_frameOptions = new wxBoxSizer(wxVERTICAL);
            wxStaticBoxSizer* sizer_frameSequence = new wxStaticBoxSizer(wxVERTICAL, m_parent, "Frame Sequence:");
                sizer_frameSequence->Add(m_combo_frame_sequence, 0, wxEXPAND | wxALL, 5);
                sizer_frameSequence->Add(m_text_frame_sequence_custom, 0, wxEXPAND | wxRIGHT | wxBOTTOM | wxLEFT, 5);
            sizer_frameOptions->Add(sizer_frameSequence, 0, wxEXPAND | wxALL, 0);
            wxStaticBoxSizer* sizer_frameTiming = new wxStaticBoxSizer(wxVERTICAL, m_parent, "Frame Timing:");
                sizer_frameTiming->Add(m_combo_frame_timing, 0, wxEXPAND | wxALL, 5);
                sizer_frameTiming->Add(m_text_frame_timing_custom, 0, wxEXPAND | wxRIGHT | wxBOTTOM | wxLEFT, 5);
            sizer_frameOptions->Add(sizer_frameTiming, 0, wxEXPAND | wxALL, 0);
        sizer_main->Add(sizer_frameOptions, 0, wxEXPAND | wxALL, 5);

        sizer_main->Add(m_listview_animation, 0, wxEXPAND | wxALL, 5);
        sizer_main->AddStretchSpacer(1);
        sizer_main->Add(m_checkbox_single_focus, 0, wxEXPAND | wxALL, 5);
        // ALLOW FOR EXPANSION
    // Add sizer to parent
}

wxArrayString AnimationControls::GetSelectedFrameSequence()
{
    if (m_combo_frame_sequence->GetValue() != "custom")
    {
        return m_lManager->getFrameSequence(m_combo_frame_sequence->GetValue()).frames;
    }
    else
    {
        return ParseCustomFrameSequence(m_text_frame_sequence_custom->GetValue());
    }
}

std::vector<int> AnimationControls::GetSelectedFrameTiming(int frameCount)
{
    // Deal with no frameCount passed
    if (frameCount == -1)
    {
        frameCount = GetSelectedFrameSequence().GetCount();
    }

    std::vector<int> frameTiming = std::vector<int>();
    if (m_combo_frame_timing->GetValue() != "custom")
    {
        frameTiming = m_lManager->getFrameTiming(m_combo_frame_timing->GetValue());
    }
    else if (m_text_frame_timing_custom->GetValue() != "")
    {
        wxStringTokenizer tokenizer(m_text_frame_timing_custom->GetValue(), ",");
        
        while (tokenizer.HasMoreTokens())
        {
            frameTiming.push_back(wxAtoi(tokenizer.GetNextToken()));
        }
    }
    else
    {
        m_parent->SetStatusText("No frame timing found! Defaulting to 100ms.");
        frameTiming.resize(frameCount, 100);
    }

    if (frameTiming.size() == 0)
    {
        m_parent->SetStatusText("No frame timing found! Defaulting to 100ms.");
        frameTiming.resize(frameCount, 100);
    }

    if (frameTiming.size() != frameCount)
    {
        PropogateFrameTiming(frameTiming, frameCount);
    }

    return frameTiming;
}

void AnimationControls::UpdateControls()
{
    UpdateFrameSequences();
    UpdateFrameTimings();
    UpdateAnimationTable();
}

void AnimationControls::OnFrameSequenceSelected(wxCommandEvent& event)
{
    wxString selectedSequence = m_combo_frame_sequence->GetValue();
    m_parent->SetStatusText("Selected Frame Sequence: " + selectedSequence);
    UpdateAnimationTable();
    m_text_frame_sequence_custom->Enable(false);
}

void AnimationControls::OnFrameTimingSelected(wxCommandEvent& event)
{
    wxString selectedTiming = m_combo_frame_timing->GetValue();
    m_parent->SetStatusText("Selected Frame Timing: " + selectedTiming);
    UpdateAnimationTable();
    m_text_frame_timing_custom->Enable(false);
}

void AnimationControls::UpdateFrameSequences()
{
    m_combo_frame_sequence->Set(m_lManager->getFrameSequenceNames());
    m_combo_frame_sequence->Append("custom");
    m_combo_frame_sequence->SetSelection(0);
}

void AnimationControls::UpdateFrameTimings()
{
    m_combo_frame_timing->Set(m_lManager->getFrameTimingNames());
    m_combo_frame_timing->Append("custom");
    m_combo_frame_timing->SetSelection(0);
}

void AnimationControls::UpdateAnimationTable()
{
    m_animationData.frameSequence = GetSelectedFrameSequence();  // Get the selected frame sequence, either from the combo box or the text control, add to m_animationData
    if ((m_animationData.frameCount = m_animationData.frameSequence.GetCount()) == 0)  // Set the frame count to the number of frames in the sequence in m_animationData
    {
        wxLogError("No frames in sequence!");
        return;
    }
    m_animationData.frameTiming = GetSelectedFrameTiming(m_animationData.frameCount);  // Get the selected frame timing, either from the combo box or the text control, add to m_animationData
    
    if (m_listview_generated == true)
    {
        m_listview_animation->DeleteAllItems();
    }
    else
    {
        m_listview_generated = true;
    }

    for (int i = 0; i < m_animationData.frameCount; i++)
    {
        m_listview_animation->InsertItem(i, m_animationData.frameSequence[i]);
        m_listview_animation->SetItem(i, 1, std::to_string(m_animationData.frameTiming[i]));
    }
}

wxArrayString AnimationControls::ParseCustomFrameSequence(wxString customSequence)
{
    wxArrayString frameSequence;
    wxStringTokenizer tokenizer(customSequence, ",");
    while (tokenizer.HasMoreTokens())
    {
        wxString token = tokenizer.GetNextToken();
        if (token.Contains("-"))
        {
            wxStringTokenizer rangeTokenizer(token, "-");
            wxString start = rangeTokenizer.GetNextToken();
            wxString end = rangeTokenizer.GetNextToken();
            int startInt = wxAtoi(start);
            int endInt = wxAtoi(end);
            if (startInt > endInt)
            {
                for (int i = startInt; i >= endInt; i--)
                {
                    frameSequence.Add(wxString::Format("%d", i));
                }
            }
            else
            {
                for (int i = startInt; i <= endInt; i++)
                {
                    frameSequence.Add(wxString::Format("%d", i));
                }
            }
        }
        else
        {
            frameSequence.Add(token);
        }
    }
    return frameSequence;
}

void AnimationControls::PropogateFrameTiming(std::vector<int>& frameTiming, int frameCount)
{
    if (frameTiming.size() == 0)
    {
        wxLogError("No frame timing found!");
        return;
    }
    else if (frameTiming.size() > frameCount)
    {
        frameTiming.resize(frameCount);
        return;
    }
    else if (frameTiming.size() < frameCount)
    {
        int lastTiming = frameTiming[frameTiming.size() - 1];
        for (int i = frameTiming.size(); i < frameCount; i++)
        {
            frameTiming.push_back(lastTiming);
        }
        return;
    }
}