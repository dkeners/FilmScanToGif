#pragma once

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/listctrl.h>
#include <wx/tokenzr.h>

#include "../tools/layoutmanager.h"
#include "../tools/animator.h"

struct AnimationData
{
    int frameCount;
    wxArrayString frameSequence;
    std::vector<int> frameTiming;
};

class AnimationControls
{
public:
    AnimationControls(wxFrame* parent, LayoutManager* layoutManager);
    ~AnimationControls();

    /**
     * @brief Creates the animation controls.
     * 
     * This function is responsible for creating the user interface controls for the animation.
     * It sets up the necessary buttons, sizers, and other elements required for controlling the animation.
     * 
     * @return void
     */
    void CreateControls();
    /**
     * @brief Updates the controls of the animation.
     * 
     * This function is responsible for updating the frame and timing comboboxes, then the list view.
     * It should be called whenever there is a change affecting all three controls.
     * 
     * @return void
     */
    void UpdateControls();
    /**
     * Retrieves the selected frame sequence.
     *
     * @return A `wxArrayString` containing the selected frame sequence.
     */
    wxArrayString GetSelectedFrameSequence();
    /**
     * Retrieves the selected frame timing.
     *
     * @param frameCount The number of frames to retrieve the timing for. Defaults to `-1` which calls `GetSelectedFrameSequence().GetCount()`. 
     * @return A `std::vector<int>` containing the selected frame timing.
     */
    std::vector<int> GetSelectedFrameTiming(int frameCount = -1);

    /**
     * @brief Returns the animation data.
     * 
     * @return The `AnimationData` struct containing the animation data.
     */
    inline AnimationData GetAnimationData() { return m_animationData; }
    /**
     * @brief Returns the wxStaticBoxSizer associated with the animation controls.
     * 
     * @return The wxStaticBoxSizer object.
     */
    inline wxStaticBoxSizer* getSizer() { return sizer_main; }
    /**
     * Returns whether the single focus checkbox is checked.
     *
     * @return `true` if the single focus checkbox is checked, `false` otherwise.
     */
    inline bool IsSingleFocusChecked() { return m_checkbox_single_focus->IsChecked(); }

private:
    /**
     * @brief Handles the event when a frame sequence is selected.
     * 
     * This function is called when the user selects a frame sequence in the UI.
     * It takes a reference to a wxCommandEvent object as a parameter.
     * 
     * @param event The event object representing the frame sequence selection event.
     */
    void OnFrameSequenceSelected(wxCommandEvent& event);
    /**
     * @brief Handles the selection of frame timing in the user interface.
     * 
     * This function is called when the user selects a frame timing option in the user interface.
     * It takes a `wxCommandEvent` object as a parameter, which contains information about the event.
     * 
     * @param event The event object that triggered the function call.
     */
    void OnFrameTimingSelected(wxCommandEvent& event);
    /**
     * @brief Updates the frame sequences.
     * 
     * This function is responsible for updating the combobox options.
     * 
     * @note This function should be called whenever there is a change in available frame sequences.
     */
    void UpdateFrameSequences();
    /**
     * @brief Updates the frame timings.
     * 
     * This function is responsible for updating the frame timing combobox options.
     * 
     * @note This function should be called whenever there is a change in available frame timings.
     */
    void UpdateFrameTimings();
    /**
     * @brief Updates the animation table.
     * 
     * This function is responsible for updating the animation table.
     * 
     * @note This function should be called whenever there is a change in frame or sequence orders.
     */
    void UpdateAnimationTable();
    wxArrayString ParseCustomFrameSequence(wxString customSequence);
    /**
     * @brief Propogates the frame timing to the specified number of frames.
     *
     * This function takes a vector of frame timing values and propagates them to the specified number of frames.
     *
     * @param frameTiming The vector of frame timing values to be propagated.
     * @param frameCount The number of frames to propagate the timing to.
     */
    void PropogateFrameTiming(std::vector<int>& frameTiming, int frameCount);

private:
    wxFrame* m_parent;
    LayoutManager* m_lManager;

    wxStaticBoxSizer* sizer_main;

    wxComboBox* m_combo_frame_sequence;
    wxTextCtrl* m_text_frame_sequence_custom;
    wxComboBox* m_combo_frame_timing;
    wxTextCtrl* m_text_frame_timing_custom;

    wxListView* m_listview_animation;
    bool m_listview_generated = false;

    wxCheckBox* m_checkbox_single_focus;

    AnimationData m_animationData;
};