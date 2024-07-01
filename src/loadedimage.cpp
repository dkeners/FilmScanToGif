#include "LoadedImage.h"

// #include <wx/wx.h>
// #include <wx/image.h>
// #include <wx/univ/statbmp.h>


// Constructors
Image::Image() : wxImage() {}

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

wxPoint Image::getMarkedPoint() const {
    return m_markedPoint;
}

int Image::getMarkedPointX() const {
    return m_markedPoint.x;
}

int Image::getMarkedPointY() const {
    return m_markedPoint.y;
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

void Image::setPositionX(int x) {
    m_position.x = x;
}

void Image::setPositionY(int y) {
    m_position.y = y;
}

void Image::setMarkedPoint(const wxPoint &point) {
    m_markedPoint = point;
}

void Image::setMarkedPoint(int x, int y) {
    m_markedPoint = wxPoint(x, y);
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
    if (!this->IsOk())
    {
        wxMessageBox("Image is not loaded properly.", "Error", wxICON_ERROR);
        return;
    }

    m_scaledSize = m_fullSize * m_scale;

    m_position = wxPoint((m_panelSize.x - m_scaledSize.x) / 2, (m_panelSize.y - m_scaledSize.y) / 2);
    
    wxImage scaledImage = (*this).Copy();
    scaledImage.Rescale(m_scaledSize.GetWidth(), m_scaledSize.GetHeight(), wxIMAGE_QUALITY_HIGH);

    bitmapDisplay->SetBitmap(scaledImage);
    bitmapDisplay->SetSize(m_scaledSize);
    bitmapDisplay->SetPosition(m_position);
}

void Image::FitImage(wxStaticBitmap *bitmapDisplay)
{
    if (!this->IsOk())
    {
        wxMessageBox("Image is not loaded properly.", "Error", wxICON_ERROR);
        return;
    }

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

Image Image::CustomSubImage(SubBmpRect rect)
{
    if (!this->IsOk())
    {
        wxMessageBox("Image is not loaded properly.", "Error", wxICON_ERROR);
        return Image();
    } else {
        rect.Offset(-m_position.x, -m_position.y);
        rect.scaleFrame((double)(1 / m_scale));
        }
    
    // This following bit may be bad but i don't know how to get Image type returned otherwise than writing 
    // (new Image())->SetData((*this).GetSubImage(rect).GetData());
    // Ok new plan, I will just copy the code from the wxImage::GetSubImage method and modify it to return Image type, plus feature cropping the rect smaller if it goes out of bounds.
    Image image;

    wxCHECK_MSG( IsOk(), image, wxT("invalid image") );

    int left = rect.GetLeft();
    int top = rect.GetTop();
    int right = rect.GetRight();
    int bottom = rect.GetBottom();

    if (left < 0)
    {
        left = 0;
    }
    if (top < 0)
    {
        top = 0;
    }
    if (right > GetWidth())
    {
        right = GetWidth();
    }
    if (bottom > GetHeight())
    {
        bottom = GetHeight();
    }

    wxCHECK_MSG( (left>=0) && (top>=0) &&
                 (right<=GetWidth()) && (bottom<=GetHeight()),
                 image, wxT("invalid subimage size") );

    const int subwidth = right - left;
    const int subheight = bottom - top;

    image.Create( subwidth, subheight, false );

    const unsigned char *src_data = GetData();
    // const unsigned char *src_alpha = M_IMGDATA->m_alpha;
    unsigned char *subdata = image.GetData();
    // unsigned char *subalpha = NULL;

    wxCHECK_MSG( subdata, image, wxT("unable to create image") );

    // if ( src_alpha ) {
    //     image.SetAlpha();
    //     subalpha = image.GetAlpha();
    //     wxCHECK_MSG( subalpha, image, wxT("unable to create alpha channel"));
    // }

    // if (M_IMGDATA->m_hasMask)
    //     image.SetMaskColour( M_IMGDATA->m_maskRed, M_IMGDATA->m_maskGreen, M_IMGDATA->m_maskBlue );

    const int width = GetWidth();
    const int pixsoff = left + width * top;

    src_data += 3 * pixsoff;
    // src_alpha += pixsoff; // won't be used if was NULL, so this is ok

    for (long j = 0; j < subheight; ++j)
    {
        memcpy( subdata, src_data, 3 * subwidth );
        subdata += 3 * subwidth;
        src_data += 3 * width;
        // if (subalpha != NULL) {
        //     memcpy( subalpha, src_alpha, subwidth );
        //     subalpha += subwidth;
        //     src_alpha += width;
        // }
    }

    return image;
    // return (*this).GetSubImage(rect);
}

void Image::zoomIn(wxStaticBitmap *bitmapDisplay)
{
    this->m_scale *= 1.1;
    this->ScaleImage(bitmapDisplay);
}

void Image::zoomOut(wxStaticBitmap *bitmapDisplay)
{
    this->m_scale *= 0.9;
    this->ScaleImage(bitmapDisplay);
}

void Image::moveLeft(wxStaticBitmap *bitmapDisplay, int step)
{
    m_position.x -= step;
    bitmapDisplay->SetPosition(m_position);
}

void Image::moveRight(wxStaticBitmap *bitmapDisplay, int step)
{
    m_position.x += step;
    bitmapDisplay->SetPosition(m_position);
}

void Image::moveUp(wxStaticBitmap *bitmapDisplay, int step)
{
    m_position.y -= step;
    bitmapDisplay->SetPosition(m_position);
}

void Image::moveDown(wxStaticBitmap *bitmapDisplay, int step)
{
    m_position.y += step;
    bitmapDisplay->SetPosition(m_position);
}