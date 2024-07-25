// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/wfstream.h>
#include <wx/image.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <stdio.h>
#include <wx/stdpaths.h>
#include <wx/combobox.h>

#include "ui/croppedimgwindow.h"
#include "loadedimage.h"
#include "tools/bitmaptransforms.h"
#include "tools/layoutmanager.h"
#include "tools/animator.h"
#include "ui/exportdialog.h"

class MyApp : public wxApp 
{
public:
    virtual bool OnInit(); 
}; 

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnOpen(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);

    void WriteSaveFile(const wxString& filepath);
    void DocumentModified(bool titleModified = false);

    void OnImport(wxCommandEvent& event);
    void ImageImport();
    void DisplayBitmap(wxStaticBitmap *bitmap, wxString imagepath);

    void OnExport(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnOverlayToggle(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    LayoutManager layout_manager;
    std::map<wxString, int> m_layoutIDs;
    CroppedImgWindow *croppedImgWindow;

    wxMenuBar *menuBar;
    wxString m_filename;
    wxFileName m_filepath;
    wxFileName m_imagepath;
    Image image;

    wxComboBox* m_combo_frame_sequence;
    wxComboBox* m_combo_frame_timing;

    wxStaticBitmap *m_static_bitmap;
    wxBitmap m_source_bitmap;
    wxStaticBitmap *m_overlay_bitmap;
    wxSize m_imagepanel_size = wxSize(1200, 400);

    wxString m_app_title = "FilmScanMotion";
    bool m_unsaved_changes = false;
};

enum
{
    ID_Overlay = 1,
    ID_IMPORT = 2,
    ID_EXPORT = 3
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler( new wxPNGHandler );
    wxImage::AddHandler( new wxJPEGHandler );
    wxImage::AddHandler( new wxTIFFHandler );

    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "FilmScanMotion")
{
    // MENU BAR
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_NEW);
    menuFile->Append(wxID_OPEN);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_SAVE);
    menuFile->Append(wxID_SAVEAS);
    menuFile->AppendSeparator();
    menuFile->Append(ID_IMPORT, "&Import... \tCtrl-I", "Import a file");
    menuFile->Append(ID_EXPORT, "&Export... \tCtrl-E", "Export a file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuEdit = new wxMenu;
    menuEdit->Append(wxID_UNDO);
    menuEdit->Append(wxID_REDO);

    wxMenu *menuView = new wxMenu;
    wxMenu *submenuLayouts = new wxMenu;
    for (auto &&layoutName : layout_manager.getLayouts())
    {
        m_layoutIDs[layoutName] = wxWindow::NewControlId();
        submenuLayouts->Append(m_layoutIDs[layoutName], layoutName, "Load layout: " + layoutName);
    }
    menuView->AppendSubMenu(submenuLayouts, "&Layouts");
    menuView->Append(ID_Overlay, "&Layout Overlay", "Toggle Alignement Overlay", wxITEM_CHECK);


    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );
    // END MENU BAR

    // START MENU BAR EVENT BINDINGS
    // File menu event bindings
    Bind(wxEVT_MENU, &MyFrame::OnNew, this, wxID_NEW);
    Bind(wxEVT_MENU, &MyFrame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MyFrame::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MyFrame::OnSaveAs, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MyFrame::OnImport, this, ID_IMPORT);
    Bind(wxEVT_MENU, &MyFrame::OnExport, this, ID_EXPORT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    // Edit menu event bindings
    Bind(wxEVT_MENU, &MyFrame::OnUndo, this, wxID_UNDO);
    Bind(wxEVT_MENU, &MyFrame::OnRedo, this, wxID_REDO);
    // View menu event bindings
    for (auto &&layoutName : layout_manager.getLayouts())
    {
        Bind(wxEVT_MENU, [this, layoutName](wxCommandEvent& event) {
                if (layout_manager.loadLayout(layoutName)) {
                    SetStatusText("Loaded layout: " + layoutName);
                    m_combo_frame_sequence->Set(layout_manager.getFrameSequenceNames());
                    m_combo_frame_sequence->Append("custom");
                    m_combo_frame_sequence->SetSelection(0);
                    m_combo_frame_timing->Set(layout_manager.getFrameTimingNames());
                    m_combo_frame_timing->Append("custom");
                    m_combo_frame_timing->SetSelection(0);
                } else {
                    wxLogError("Failed to load layout: " + layoutName);
                }
            },
            m_layoutIDs[layoutName]);
    }
    Bind(wxEVT_MENU, &MyFrame::OnOverlayToggle, this, ID_Overlay);
    // Help menu event bindings
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    // END MENU BAR EVENT BINDINGS
    
    // INIT APP STATUS BAR
    CreateStatusBar();

    SetStatusText("Welcome to FilmScanMotion!");
    // END INIT APP STATUS BAR

    if (layout_manager.loadLayout()) {
        SetStatusText("Welcome to FilmScanMotion! Loaded default layout: " + layout_manager.getLayoutName());
    } else {
        wxLogError("Failed to load default layout!");
    }
    
    wxPanel *panel_image = new wxPanel(this, wxID_ANY, wxDefaultPosition, m_imagepanel_size, wxBORDER_NONE, "Panel");
    panel_image->AlwaysShowScrollbars(true, true);
    panel_image->SetBackgroundColour(wxColour("WHITE"));

    // wxImage overlay_image;
    // wxStandardPaths &stdPaths = wxStandardPaths::Get();
    // overlay_image.LoadFile(stdPaths.GetResourcesDir() + "/rss/Nimslo3DLayout.png");
    // m_overlay_bitmap = new wxStaticBitmap(panel_image, wxID_ANY, wxNullBitmap, wxPoint(0,0), wxDefaultSize, wxBORDER_NONE, "Overlay");
    // m_overlay_bitmap->SetBitmap(overlay_image);
    // m_overlay_bitmap->SetBackgroundColour(wxBG_STYLE_TRANSPARENT);
    // wxMenuItem* menuItem = menuView->FindItem(ID_Overlay);
    // if(menuItem != NULL) {
    //     if (menuItem->IsChecked())
    //     {
    //         m_overlay_bitmap->Hide();
    //     }
    //     else
    //     {
    //         m_overlay_bitmap->Show();
    //     }
    // }

    m_static_bitmap = new wxStaticBitmap(panel_image, wxID_ANY, wxNullBitmap, wxDefaultPosition, m_imagepanel_size, wxBORDER_NONE, "Image");
    m_static_bitmap->SetBackgroundColour(wxColour(wxTransparentColour));
    m_static_bitmap->Show();
   
    // BUTTON CONTROLS
    BitmapTransforms controls_(m_static_bitmap, &image, this);
    controls_.loadAllControls();

    // Import Image
    wxButton *button_upload_image = new wxButton(this, wxID_ANY, "Upload Image", wxDefaultPosition, wxSize(100, 40));
    button_upload_image->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnImport, this);
    // END BUTTON CONTROLS

    // IMAGE SELECTOR CONTROLS
    wxBoxSizer *sizer_image_selector = new wxBoxSizer(wxHORIZONTAL);
    wxRadioButton *radio_button_image_choice = new wxRadioButton(this, wxID_ANY, "Image 1");
    wxRadioButton *radio_button_image_choice2 = new wxRadioButton(this, wxID_ANY, "Image 2");
    wxRadioButton *radio_button_image_choice3 = new wxRadioButton(this, wxID_ANY, "Image 3");
    wxButton *button_image_choice = new wxButton(this, wxID_ANY, "Select Image", wxDefaultPosition, wxSize(100, 40));
    button_image_choice->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event) {
        Image subImage = image.CustomSubImage(layout_manager.getFrame("1"));
        croppedImgWindow = new CroppedImgWindow(this, wxString("Cropped Image"), &subImage);
        croppedImgWindow->Show(true);
        });
    sizer_image_selector->Add(radio_button_image_choice, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    sizer_image_selector->Add(radio_button_image_choice2, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    sizer_image_selector->Add(radio_button_image_choice3, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    sizer_image_selector->AddStretchSpacer();
    sizer_image_selector->Add(button_image_choice, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    // END IMAGE SELECTOR CONTROLS

    // ANIMATION CONTROLS
    // ComboBox for Possible Frame Sequences
    m_combo_frame_sequence = new wxComboBox(this, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, layout_manager.getFrameSequenceNames(), wxCB_READONLY);
    m_combo_frame_sequence->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedSequence = m_combo_frame_sequence->GetValue();
        this->SetStatusText("Selected Frame Sequence: " + selectedSequence);
        });
    m_combo_frame_sequence->Append("custom");
    // ComboBox for Possible Frame Timings
    m_combo_frame_timing = new wxComboBox(this, wxID_ANY, "default", wxDefaultPosition, wxDefaultSize, layout_manager.getFrameTimingNames(), wxCB_READONLY);
    m_combo_frame_timing->Bind(wxEVT_COMBOBOX, [this](wxCommandEvent& event) {
        wxString selectedTiming = m_combo_frame_timing->GetValue();
        this->SetStatusText("Selected Frame Timing: " + selectedTiming);
        });
    m_combo_frame_timing->Append("custom");
    // Checkbox for Variable Focus Point
    wxCheckBox* checkbox_stationary_focus = new wxCheckBox(this, wxID_ANY, "Stationary Focus Point?");
    checkbox_stationary_focus->SetValue(true);
    // Button to Align Frames
    wxButton* button_align_anim = new wxButton(this, wxID_ANY, "Align - Single", wxDefaultPosition, wxSize(100, 40));
    button_align_anim->Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this, checkbox_stationary_focus](wxCommandEvent& event) {
        if (!image.IsOk()) {
            wxLogError("No image loaded!");
            return;
        }
        if (layout_manager.getFrameCount() == 0) {
            wxLogError("No layout with frames loaded!");
            return;
        }

        Animator::FrameAlignment(&layout_manager, &image, m_combo_frame_sequence->GetValue(), checkbox_stationary_focus->IsChecked());

        });
    // Button to Export Animation
    wxButton* button_export_anim = new wxButton(this, wxID_ANY, "Export\nAnimation", wxDefaultPosition, wxSize(100, 40));
    button_export_anim->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnExport, this);
    // END ANIMATION CONTROLS

    // MAIN SIZER
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(panel_image, 0, wxEXPAND | wxALL, 10);
        
        // Button Controls Sizer
        wxBoxSizer *sizer_image_controls = controls_.createControlSizer(this);
            sizer_image_controls->Add(button_upload_image, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM | wxRIGHT, 5);
        sizer->Add(sizer_image_controls, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
        
        sizer->Add(sizer_image_selector, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

        // Animation Controls Sizer
        wxStaticBoxSizer* sizer_animation_controls = new wxStaticBoxSizer(wxHORIZONTAL, this, "Animation Controls");
            sizer_animation_controls->Add(m_combo_frame_sequence, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
            sizer_animation_controls->Add(m_combo_frame_timing, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
            sizer_animation_controls->AddStretchSpacer();
            sizer_animation_controls->Add(checkbox_stationary_focus, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
            sizer_animation_controls->Add(button_align_anim, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
            sizer_animation_controls->Add(button_export_anim, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
        sizer->Add(sizer_animation_controls, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    this->SetSizerAndFit(sizer);
}

void MyFrame::OnNew(wxCommandEvent& event)
{
    SetStatusText("Creating new file...");
    // Do real new file creation here
    wxLogMessage("New file created!");
    // End of new file creation
    SetStatusText("New file created!");
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
    SetStatusText("Opening file...");
    
    if (m_unsaved_changes)
    {
        // Ask user to save changes
        wxMessageDialog dialog(this, "Do you want to save changes?", "Save changes", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = dialog.ShowModal();

        if (result == wxID_YES)
        {
            this->OnSave(event);
        }
        else if (result == wxID_CANCEL)
        {
            SetStatusText("File open canceled!");
            return;
        }
    }
    
    wxFileDialog openFileDialog(this, _("Open XFSM file"), "", "",
                                "xfsm files (*.xfsm)|*.xfsm", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        SetStatusText("File open canceled!");
        return;
    }

    m_filename = openFileDialog.GetFilename();
    m_filepath = openFileDialog.GetPath();

    m_unsaved_changes = false;
    this->SetLabel(m_app_title + " - " + m_filename);

    wxFileInputStream input_stream(m_filepath.GetFullPath());

    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", m_filepath.GetFullPath());
        return;
    }

    input_stream.GetFile();

    SetStatusText("File opened!");
}

void MyFrame::OnSave(wxCommandEvent& event)
{    
    if (!m_filepath.IsOk())
    {
        this->OnSaveAs(event);
        return;
    }

    SetStatusText("Saving file...");

    this->WriteSaveFile(m_filepath.GetFullPath());
}

void MyFrame::OnSaveAs(wxCommandEvent& event)
{
    SetStatusText("Saving file...");

    wxString* filename;

    if (m_filename.IsEmpty())
    {
        filename = new wxString("Untitled.xfsm");
    }
    else
    {
        filename = &m_filename;
    }

    wxFileDialog saveFileDialog(this, _("Save XFSM file"), "", *filename,
                                "xfsm files (*.xfsm)|*.xfsm", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
 
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        SetStatusText("File save canceled!");
        return;
    }

    m_filename = saveFileDialog.GetFilename();
    m_filepath = saveFileDialog.GetPath();

    this->WriteSaveFile(m_filepath.GetFullPath());
}

void MyFrame::WriteSaveFile(const wxString& filepath)
{
    wxFileOutputStream output_stream(filepath);

    // Save the object
    // For example:
    int obj = 10;
    output_stream.Write(&obj, sizeof(obj));

    if (!output_stream.IsOk())
    {
        wxLogError("Cannot save current contents in file '%s'.", filepath);
        return;
    }

    m_unsaved_changes = false;
    this->SetLabel(m_app_title + " - " + m_filename);

    SetStatusText("File saved as " + m_filename + "!");
}

void MyFrame::DocumentModified(bool titleModified)
{
    if (m_unsaved_changes && !titleModified)
    {
        return;
    }
    
    m_unsaved_changes = true;
    this->SetLabel(m_app_title + " - " + m_filename + " *");
}

void MyFrame::OnImport(wxCommandEvent& event)
{
    SetStatusText("Importing file...");
    
    // Warn user about image replacement
    wxMessageDialog dialog(this, "This will replace any currently loaded image.", "Import Image", wxOK | wxCANCEL | wxICON_WARNING);
    int result = dialog.ShowModal();

    if (result == wxID_CANCEL)
    {
        SetStatusText("File import canceled!");
        return;
    }

    this->ImageImport();
}

void MyFrame::ImageImport()
{
    wxFileDialog openFileDialog(this, _("Import Image"), "", "",
                                "Image files (*.jpg;*.png;*.tiff)|*.jpg;*.png;*.tiff;*.tif|JPG files(*.jpg;*.jpeg)|*.jpg;*.jpeg|PNG files (*.png)|*.png|TIFF files (*.tiff)|*.tiff;*.tif", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        SetStatusText("File open canceled!");
        return;
    }

    m_imagepath = openFileDialog.GetPath();

    image.setPanelSize(m_imagepanel_size);
    image.LoadAndScaleImage(m_imagepath.GetFullPath(), m_static_bitmap);

    if (!m_filepath.IsOk())
    {
        m_filename = m_imagepath.GetName();
    }

    this->DocumentModified(true);

    SetStatusText("File imported!");
}

void MyFrame::OnExport(wxCommandEvent& event)
{
    SetStatusText("Exporting file...");

    ExportDialog exportDialog(this, "Export Image", layout_manager);
    switch (exportDialog.ShowModal())
    {
    case 0:
        SetStatusText("Error exporting file!");
        break;

    case 1:
        SetStatusText("File exported!");
        break;

    case 2:
        SetStatusText("File export canceled!");
        break;
    
    default:
        SetStatusText("Unknown error exporting file!");
        break;
    }
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    if (m_unsaved_changes)
    {
        // Ask user to save changes
        wxMessageDialog dialog(this, "Do you want to save changes?", "Save changes", wxYES_NO | wxCANCEL | wxICON_QUESTION);
        int result = dialog.ShowModal();

        if (result == wxID_YES)
        {
            this->OnSave(event);
        }
        else if (result == wxID_CANCEL)
        {
            SetStatusText("Exit canceled!");
            return;
        }
    }

    Close(true);
}

void MyFrame::OnUndo(wxCommandEvent& event)
{
    SetStatusText("Undoing last action...");
    // Do real undo here
    wxLogMessage("Last action undone!");
    // CroppedImgWindow *croppedImgWindow = new CroppedImgWindow(this, "Cropped Image");
    // croppedImgWindow->Show(true);
    this->DocumentModified();
    // End of undo
    SetStatusText("Last action undone!");
}

void MyFrame::OnRedo(wxCommandEvent& event)
{
    SetStatusText("Redoing last action...");
    // Do real redo here
    wxLogMessage("Last action redone!");
    this->DocumentModified();
    // End of redo
    SetStatusText("Last action redone!");
}

void MyFrame::OnOverlayToggle(wxCommandEvent& event)
{
    SetStatusText("Toggling overlay...");
    // Do real overlay toggle here
    wxMenuItem* menuItem = menuBar->FindItem(ID_Overlay);
    if(menuItem != NULL) {
        if (menuItem->IsChecked())
        {
            m_overlay_bitmap->Show();
        }
        else
        {
            m_overlay_bitmap->Hide();
        }
    }
    // End of overlay toggle
    SetStatusText("Overlay toggled!");
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}