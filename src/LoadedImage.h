#pragma once

#include <wx/wx.h>
#include <wx/image.h>
// #include <wx/univ/statbmp.h>

/**
 * @class Image
 * @brief Represents an image object.
 *
 * The `Image` class is a subclass of `wxImage` and provides additional functionality for loading, scaling, and displaying images.
 */
class Image : public wxImage {
public:
    // Public member variables
    double m_scale;

    // Constructors
    Image() : wxImage() {
        // Default constructor
    }

    /**
     * @brief Constructs an Image object.
     *
     * This constructor initializes an Image object with the specified parameters.
     *
     * @param filename The filename of the image file to load.
     * @param position The position of the image.
     * @param imageSize The size of the image.
     * @param scaledSize The scaled size of the image.
     * @param scale The scale factor of the image.
     */
    Image(const wxString &filename, wxPoint position = wxPoint(0, 0), wxSize imageSize = wxSize(1200, 400), wxSize scaledSize = wxSize(1200, 400), double scale = 1.0)
        : m_filename(filename), m_position(position), m_fullSize(imageSize), m_scaledSize(scaledSize), m_scale(scale) {
        // Load the image from the file
        LoadFile(filename);
    }

    // Getters
    wxString getFilename() const {
        return m_filename;
    }

    wxPoint getPosition() const {
        return m_position;
    }

    int getPositionX() const {
        return m_position.x;
    }

    int getPositionY() const {
        return m_position.y;
    }

    wxSize getPanelSize() const {
        return m_panelSize;
    }

    int getPanelWidth() const {
        return m_panelSize.x;
    }

    int getPanelHeight() const {
        return m_panelSize.y;
    }

    wxSize getFullSize() const {
        return m_fullSize;
    }

    int getFullWidth() const {
        return m_fullSize.x;
    }

    int getFullHeight() const {
        return m_fullSize.y;
    }

    wxSize getScaledSize() const {
        return m_scaledSize;
    }

    int getScaledWidth() const {
        return m_scaledSize.x;
    }

    int getScaledHeight() const {
        return m_scaledSize.y;
    }

    double getScale() const {
        return m_scale;
    }

    // Setters
    void setFilename(const wxString &filename) {
        m_filename = filename;
    }

    void setPosition(const wxPoint &position) {
        m_position = position;
    }

    void setPosition(int x, int y) {
        m_position = wxPoint(x, y);
    }

    void setPanelSize(const wxSize &size) {
        m_panelSize = size;
    }

    void setPanelSize(int width, int height) {
        m_panelSize = wxSize(width, height);
    }

    void setFullSize(const wxSize &size) {
        m_fullSize = size;
    }

    void setFullSize(int width, int height) {
        m_fullSize = wxSize(width, height);
    }

    void setFullWidth(int width) {
        m_fullSize.x = width;
    }

    void setFullHeight(int height) {
        m_fullSize.y = height;
    }

    void setScaledSize(const wxSize &size) {
        m_scaledSize = size;
    }

    void setScaledSize(int width, int height) {
        m_scaledSize = wxSize(width, height);
    }

    void setScaledWidth(int width) {
        m_scaledSize.x = width;
    }

    void setScaledHeight(int height) {
        m_scaledSize.y = height;
    }

    void setScale(double scale) {
        m_scale = scale;
    }

    // Public methods
    /**
     * Scales the image and updates the display bitmap.
     * TODO: scale the image around the moved center point, not the panel center.
     * 
     * @param bitmapDisplay The wxStaticBitmap object used to display the image.
     */
    void ScaleImage(wxStaticBitmap *bitmapDisplay)
    {
        m_scaledSize = m_fullSize * m_scale;

        m_position = wxPoint((m_panelSize.x - m_scaledSize.x) / 2, (m_panelSize.y - m_scaledSize.y) / 2);
        
        wxImage scaledImage = (*this).Copy();
        scaledImage.Rescale(m_scaledSize.GetWidth(), m_scaledSize.GetHeight(), wxIMAGE_QUALITY_HIGH);

        bitmapDisplay->SetBitmap(scaledImage);
        bitmapDisplay->SetSize(m_scaledSize);
        bitmapDisplay->SetPosition(m_position);
    }

    /**
     * Loads and scales an image file to fit within the specified wxStaticBitmap control.
     * 
     * @param filename The path to the image file to load.
     * @param bitmapDisplay The wxStaticBitmap control to display the scaled image.
     */
    void LoadAndScaleImage(wxString filename, wxStaticBitmap *bitmapDisplay)
    {
        m_filename = filename;
        LoadFile(m_filename);

        m_fullSize = GetSize();
        int width = m_fullSize.x;
        int height = m_fullSize.y;

        // Check if the panel size is fully specified. TODO: Find a better way to handle this.
        if (m_panelSize == wxSize(0, 0))
        {
            throw std::runtime_error("Panel size is not fully specified in Image.");
        }
        
        int panelWidth = m_panelSize.x;
        int panelHeight = m_panelSize.y;

        m_scale = 1.0;
        
        if (width > panelWidth && height > panelHeight)
        {
            double scale_width = panelWidth / (double)width;
            double scale_height = panelHeight / (double)height;
            m_scale = (scale_width < scale_height) ? scale_width : scale_height;
        } else if (height > panelHeight)
        {
            m_scale = panelHeight / (double)height;
        } else if (width > panelWidth)
        {
            m_scale = panelWidth / (double)width;
        }

        ScaleImage(bitmapDisplay);
    }

    // Destructor
    // ~Image() : ~wxImage() {
    //     // Clean up
    //     // ...
    //     this->Destroy();
    // }

private:
    wxString m_filename;
    wxPoint m_position;
    wxSize m_panelSize;
    wxSize m_fullSize;
    wxSize m_scaledSize;
    wxStaticBitmap *m_bitmapDisplay;
};