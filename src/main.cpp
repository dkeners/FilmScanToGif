// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/wfstream.h>
#include <wx/image.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <stdio.h>

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
    void OnHello(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);

    void WriteSaveFile(const wxString& filepath);
    void DocumentModified();

    void OnImport(wxCommandEvent& event);
    void ImageImport();
    void DisplayBitmap(wxStaticBitmap *bitmap, wxString imagepath);
    void MoveBitmapUp(wxStaticBitmap *bitmap);
    void MoveBitmapDown(wxStaticBitmap *bitmap);
    void MoveBitmapLeft(wxStaticBitmap *bitmap);
    void MoveBitmapRight(wxStaticBitmap *bitmap);

    void OnExport(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnRedo(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxString m_filename;
    wxFileName m_filepath;
    wxFileName m_imagepath;
    // wxPanel *m_panel_image;
    wxStaticBitmap *m_static_bitmap;
    wxBitmap m_source_bitmap;
    wxSize m_imagepanel_size = wxSize(1200, 400);
    wxSize m_image_size = wxSize(1200, 400);
    double m_image_scale = 1.0;
    wxPoint m_image_position = wxPoint(0, 0);
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
    menuView->Append(ID_Overlay, "&Overlay", "Alignement Overlay Toggle", wxITEM_CHECK);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar( menuBar );

    CreateStatusBar();

    SetStatusText("Welcome to FilmScanMotion!");

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
    // Bind(wxEVT_MENU, &MyFrame::OnOverlay, this, ID_Overlay);
    // Help menu event bindings
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    // App event bindings
    // Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnExit, this);
    
    wxPanel *panel_image = new wxPanel(this, wxID_ANY, wxDefaultPosition, m_imagepanel_size, wxBORDER_SIMPLE, "Panel");
    panel_image->SetBackgroundColour(wxColour(0, 0, 200));
    m_static_bitmap = new wxStaticBitmap(panel_image, wxID_ANY, wxNullBitmap, m_image_position, m_image_size, wxBORDER_SIMPLE, "Image");

    // wxPanel *panel_image_controls = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(800, 50), wxBORDER_SIMPLE, "Overlay");
    // panel_image_controls->SetBackgroundColour(wxColour(200, 0, 0));

    // Button Controls
    wxButton *button_zoom = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(40, 40));
    button_zoom->SetBackgroundColour(wxColour(0, 200, 0));

    wxButton *button_zoom_out = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(40, 40));
    button_zoom_out->SetBackgroundColour(wxColour(200, 0, 0));

    wxButton *button_left = new wxButton(this, wxID_ANY, "<", wxDefaultPosition, wxSize(40, 40));
    button_left->SetBackgroundColour(wxColour(0, 200, 0));

    wxButton *button_right = new wxButton(this, wxID_ANY, ">", wxDefaultPosition, wxSize(40, 40));
    button_right->SetBackgroundColour(wxColour(200, 0, 0));

    wxButton *button_up = new wxButton(this, wxID_ANY, "^", wxDefaultPosition, wxSize(40, 40));
    button_up->SetBackgroundColour(wxColour(0, 200, 0));

    wxButton *button_down = new wxButton(this, wxID_ANY, "v", wxDefaultPosition, wxSize(40, 40));
    button_down->SetBackgroundColour(wxColour(200, 0, 0));

    wxButton *button_rotate_left = new wxButton(this, wxID_ANY, "↺", wxDefaultPosition, wxSize(40, 40));

    wxButton *button_rotate_right = new wxButton(this, wxID_ANY, "↻", wxDefaultPosition, wxSize(40, 40));

    // Import Image
    wxButton *button_upload_image = new wxButton(this, wxID_ANY, "Upload Image", wxDefaultPosition, wxSize(100, 40));
    button_upload_image->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::ImageImport, this);

    wxBoxSizer *sizer_image_controls = new wxBoxSizer(wxHORIZONTAL);
    sizer_image_controls->Add(button_zoom, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_image_controls->Add(button_zoom_out, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);

    sizer_image_controls->Add(button_left, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_image_controls->Add(button_right, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_image_controls->Add(button_up, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_image_controls->Add(button_down, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);

    sizer_image_controls->Add(button_rotate_left, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);
    sizer_image_controls->Add(button_rotate_right, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxTOP | wxBOTTOM, 5);

    sizer_image_controls->AddStretchSpacer();

    sizer_image_controls->Add(button_upload_image, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);
    // End of Button Controls

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(panel_image, 0, wxEXPAND | wxALL, 10);
    sizer->Add(sizer_image_controls, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

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

void MyFrame::DocumentModified()
{
    if (m_unsaved_changes)
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

    // if (m_filename.IsEmpty())
    // {
    //     m_filename = openFileDialog.GetFilename();
    //     SetStatusText("Filename: " + m_filename);
    // }

    m_imagepath = openFileDialog.GetPath();
    if (m_filename.IsEmpty())
    {
        m_filename = m_imagepath.GetName();
    }
    m_unsaved_changes = false;
    // this->SetLabel(m_app_title + " - " + m_filename); // Label will be set in DocumentModified()

    wxFileInputStream input_stream(m_imagepath.GetFullPath());

    if (!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", m_imagepath.GetFullPath());
        return;
    }

    this->DisplayBitmap(m_static_bitmap, m_imagepath.GetFullPath());

    // wxImage image;
    // image.LoadFile(input_stream, wxBITMAP_TYPE_ANY, -1);

    // wxBitmap bitmap(image);

    // wxStaticBitmap *static_bitmap = new wxStaticBitmap(m_panel_image, wxID_ANY, bitmap);
    // static_bitmap->SetSize(image.GetWidth(), image.GetHeight());

    this->DocumentModified();

    SetStatusText("File imported!");
}

void MyFrame::DisplayBitmap(wxStaticBitmap *bitmapDisplay, wxString l_imagepath)
{
    wxImage image;
    image.LoadFile(l_imagepath);

    wxBitmap newBitmap(image);

    int width = image.GetWidth();
    int height = image.GetHeight();
    int panel_width = m_imagepanel_size.GetWidth();
    int panel_height = m_imagepanel_size.GetHeight();
    double scale = 1.0;

    if (width > panel_width && height <= panel_height)
    {
        scale = panel_width / (double)width;
    }
    else if (height > panel_height && width <= panel_width)
    {
        scale = panel_height / (double)height;
    }
    else if (width > panel_width && height > panel_height)
    {
        double scale_width = panel_width / (double)width;
        double scale_height = panel_height / (double)height;
        scale = (scale_width < scale_height) ? scale_width : scale_height;
    }

    m_image_size = wxSize(width, height) * scale;

    m_image_position = wxPoint((panel_width - m_image_size.GetWidth()) / 2, (panel_height - m_image_size.GetHeight()) / 2);
    
    wxBitmap::Rescale(newBitmap, m_image_size);
    // static_bitmap->SetSize(image.GetWidth(), image.GetHeight());

    bitmapDisplay->SetBitmap(newBitmap);
    bitmapDisplay->SetSize(m_image_size);
    bitmapDisplay->SetPosition(m_image_position);
}

void MyFrame::MoveBitmapUp(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y -= 10;
    bitmapDisplay->SetPosition(position);
}

void MyFrame::MoveBitmapDown(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.y += 10;
    bitmapDisplay->SetPosition(position);
}

void MyFrame::MoveBitmapLeft(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x -= 10;
    bitmapDisplay->SetPosition(position);
}

void MyFrame::MoveBitmapRight(wxStaticBitmap *bitmapDisplay)
{
    wxPoint position = bitmapDisplay->GetPosition();
    position.x += 10;
    bitmapDisplay->SetPosition(position);
}

void MyFrame::OnExport(wxCommandEvent& event)
{
    SetStatusText("Exporting file...");
    // Do real exporting here
    wxLogMessage("File exported!");
    // End of exporting
    SetStatusText("File exported!");
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

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}