#pragma once

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/valnum.h>
#include <wx/statline.h>

#include "../tools/animator.h"

class ExportDialog : public wxDialog
{
public:
    ExportDialog(wxWindow* parent, const wxString& title, LayoutManager& layout_manager);

private:
    LayoutManager& m_layout_manager;

    wxCheckBox* m_cbTransparent;
    wxCheckBox* m_cbStackFrames;
    wxCheckBox* m_cbCropInterior;

    wxTextCtrl* m_tcRed;
    wxTextCtrl* m_tcGreen;
    wxTextCtrl* m_tcBlue;

    /**
     * @brief Event handler for the export command event.
     * 
     * This function is called when the user selects the export command from the UI.
     * It takes a reference to a `wxCommandEvent` object as a parameter.
     * 
     * @param event The event object representing the export command event.
     */
    void OnExport(wxCommandEvent& event);

    /**
     * @brief Toggles the RGB input.
     *
     * This function enables or disables the RGB input.
     *
     * @param enable If true, RGB input is enabled. If false, RGB input is disabled.
     */
    void ToggleRGB(bool enable);

    /**
     * @brief Handles the close event for the export dialog.
     * 
     * This function is called when the export dialog is closed. It is responsible for handling any necessary cleanup or actions before the dialog is closed.
     * 
     * @param event The close event that triggered the function call.
     */
    void OnClose(wxCloseEvent& event);
};