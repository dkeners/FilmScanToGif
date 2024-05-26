#include "LoadedImage.h"

// #include <wx/wx.h>
// #include <wx/image.h>
// #include <wx/univ/statbmp.h>


// Constructors
Image::Image() : wxImage() {
    m_filename = "";
    m_position = wxPoint(0, 0);
    m_panelSize = wxSize(0, 0);
    m_fullSize = wxSize(0, 0);
    m_scaledSize = wxSize(0, 0);
    m_scale = 1.0;
}

Image::Image(const wxString &filename, wxPoint position, wxSize imageSize, wxSize scaledSize, double scale)
    : m_filename(filename), m_position(position), m_fullSize(imageSize), m_scaledSize(scaledSize), m_scale(scale) {
    // Load the image from the file
    LoadFile(filename);
}

// Getters
wxString Image::getFilename() const {
    return m_filename;
}

wxPoint Image::getPosition() const {
    return m_position;
}

int Image::getPositionX() const {
    return m_position.x;
}

int Image::getPositionY() const {
    return m_position.y;
}

wxSize Image::getPanelSize() const {
    return m_panelSize;
}

int Image::getPanelWidth() const {
    return m_panelSize.x;
}

int Image::getPanelHeight() const {
    return m_panelSize.y;
}

wxSize Image::getFullSize() const {
    return m_fullSize;
}

int Image::getFullWidth() const {
    return m_fullSize.x;
}

int Image::getFullHeight() const {
    return m_fullSize.y;
}

wxSize Image::getScaledSize() const {
    return m_scaledSize;
}

int Image::getScaledWidth() const {
    return m_scaledSize.x;
}

int Image::getScaledHeight() const {
    return m_scaledSize.y;
}

double Image::getScale() const {
    return m_scale;
}

// Setters
void Image::setFilename(const wxString &filename) {
    m_filename = filename;
}

void Image::setPosition(const wxPoint &position) {
    m_position = position;
}

void Image::setPosition(int x, int y) {
    m_position = wxPoint(x, y);
}

void Image::setPanelSize(const wxSize &size) {
    m_panelSize = size;
}

void Image::setPanelSize(int width, int height) {
    m_panelSize = wxSize(width, height);
}

void Image::setFullSize(const wxSize &size) {
    m_fullSize = size;
}

void Image::setFullSize(int width, int height) {
    m_fullSize = wxSize(width, height);
}

void Image::setFullWidth(int width) {
    m_fullSize.x = width;
}

void Image::setFullHeight(int height) {
    m_fullSize.y = height;
}

void Image::setScaledSize(const wxSize &size) {
    m_scaledSize = size;
}

void Image::setScaledSize(int width, int height) {
    m_scaledSize = wxSize(width, height);
}

void Image::setScaledWidth(int width) {
    m_scaledSize.x = width;
}

void Image::setScaledHeight(int height) {
    m_scaledSize.y = height;
}

void Image::setScale(double scale) {
    m_scale = scale;
}

// Public methods
void Image::ScaleImage(wxStaticBitmap *bitmapDisplay)
{
    m_scaledSize = m_fullSize * m_scale;

    m_position = wxPoint((m_panelSize.x - m_scaledSize.x) / 2, (m_panelSize.y - m_scaledSize.y) / 2);
    
    wxImage scaledImage = (*this).Copy();
    scaledImage.Rescale(m_scaledSize.GetWidth(), m_scaledSize.GetHeight(), wxIMAGE_QUALITY_HIGH);

    bitmapDisplay->SetBitmap(scaledImage);
    bitmapDisplay->SetSize(m_scaledSize);
    bitmapDisplay->SetPosition(m_position);
}

void Image::LoadAndScaleImage(wxString filename, wxStaticBitmap *bitmapDisplay)
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