#include "animationcontrols.h"

AnimationControls::AnimationControls(wxFrame* parent, LayoutManager* layoutManager)
    : m_parent(parent), m_lManager(layoutManager)
{
}

AnimationControls::~AnimationControls()
{
    delete sizer_main;
}

void AnimationControls::CreateControls()
{
    // CREATE CONTROLS
    // ComboBox for Possible Frame Sequences
    m_combo_frame_sequence = new wxComboBox(m_parent, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, m_lManager->getFrameSequenceNames(), wxCB_READONLY);
    m_combo_frame_sequence->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedSequence = m_combo_frame_sequence->GetValue();
        UpdateAnimationTable();
        m_parent->SetStatusText("Selected Frame Sequence: " + selectedSequence);
        });
    m_combo_frame_sequence->Append("custom");
    // TextControl for Custom Frame Sequence
    wxTextCtrl* text_frame_sequence_custom = new wxTextCtrl(m_parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    // ComboBox for Possible Frame Timings
    m_combo_frame_timing = new wxComboBox(m_parent, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, m_lManager->getFrameTimingNames(), wxCB_READONLY);
    m_combo_frame_timing->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedTiming = m_combo_frame_timing->GetValue();
        UpdateAnimationTable();
        m_parent->SetStatusText("Selected Frame Timing: " + selectedTiming);
        });
    m_combo_frame_timing->Append("custom");
    // TextControl for Custom Frame Timing
    wxTextCtrl* text_frame_timing_custom = new wxTextCtrl(m_parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    // ListView for Frame and Delay Selection
    m_listview_animation = new wxListView(m_parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    m_listview_animation->AppendColumn("Frame", wxLIST_FORMAT_LEFT, 75);
    m_listview_animation->AppendColumn("Delay", wxLIST_FORMAT_LEFT, 75);
    UpdateAnimationTable();
    m_listview_animation->Bind(wxEVT_LIST_ITEM_SELECTED, [this](wxListEvent& event) {
        m_parent->SetStatusText("Selected Frame: " + m_listview_animation->GetItemText(event.GetIndex()) + " with Delay: " + m_listview_animation->GetItemText(event.GetIndex(), 1));
        });
    // Checkbox for Single Focus Point
    m_checkbox_single_focus = new wxCheckBox(m_parent, wxID_ANY, "Single Focus Point?");
    m_checkbox_single_focus->SetValue(true);
    
    // CREATE SIZERS
    sizer_main = new wxStaticBoxSizer(wxHORIZONTAL, m_parent, "Animation Controls");
        wxBoxSizer* sizer_frameOptions = new wxBoxSizer(wxVERTICAL);
            wxStaticBoxSizer* sizer_frameSequence = new wxStaticBoxSizer(wxVERTICAL, m_parent, "Frame Sequence:");
                sizer_frameSequence->Add(m_combo_frame_sequence, 0, wxEXPAND | wxALL, 5);
                sizer_frameSequence->Add(text_frame_sequence_custom, 0, wxEXPAND | wxALL, 5);
            sizer_frameOptions->Add(sizer_frameSequence, 0, wxEXPAND | wxALL, 5);
            wxStaticBoxSizer* sizer_frameTiming = new wxStaticBoxSizer(wxVERTICAL, m_parent, "Frame Timing:");
                sizer_frameTiming->Add(m_combo_frame_timing, 0, wxEXPAND | wxALL, 5);
                sizer_frameTiming->Add(text_frame_timing_custom, 0, wxEXPAND | wxALL, 5);
            sizer_frameOptions->Add(sizer_frameTiming, 0, wxEXPAND | wxALL, 5);
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
    // Implement custom frame sequence parser
    return wxArrayString();
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
}

void AnimationControls::OnFrameTimingSelected(wxCommandEvent& event)
{
    wxString selectedTiming = m_combo_frame_timing->GetValue();
    m_parent->SetStatusText("Selected Frame Timing: " + selectedTiming);
    UpdateAnimationTable();
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
    FrameSequence frameSequence = m_lManager->getFrameSequence(m_combo_frame_sequence->GetValue());
    if (frameSequence.frameCount == 0)
    {
        wxLogError("No frames in sequence!");
        return;
    }
    std::vector<int> frameTiming = m_lManager->getFrameTiming(m_combo_frame_timing->GetValue());
    if (frameTiming.size() == 0)
    {
        m_parent->SetStatusText("No frame timing found!");
        return;
    }
    else if (frameTiming.size() != frameSequence.frameCount)
    {
        PropogateFrameTiming(frameTiming, frameSequence.frameCount);
    }
    
    if (m_listview_generated == true)
    {
        m_listview_animation->DeleteAllItems();
    }
    else
    {
        m_listview_generated = true;
    }

    for (int i = 0; i < frameSequence.frameCount; i++)
    {
            // Debug prints
            std::cout << "i: " << i << ", frameCount: " << frameSequence.frameCount << std::endl;
            std::cout << "frame: " << frameSequence.frames[i] << std::endl;
            std::cout << "frameTiming: " << frameTiming[i] << std::endl;

            m_listview_animation->InsertItem(i, frameSequence.frames[i]);
            m_listview_animation->SetItem(i, 1, std::to_string(frameTiming[i]));
    }
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
        wxLogError("Too many frame timings!");
        return;
    }
    else if (frameTiming.size() < frameCount)
    {
        int lastTiming = frameTiming[frameTiming.size() - 1];
        for (int i = frameTiming.size(); i < frameCount; i++)
        {
            frameTiming.push_back(lastTiming);
        }
    }
}