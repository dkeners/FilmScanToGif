#pragma once

#include <wx/wx.h>
#include <wx/image.h>
// #include <wx/univ/statbmp.h>

#include "tools/subbmprect.h"

/**
 * @class Image
 * @brief Represents an image object.
 *
 * The `Image` class is a subclass of `wxImage` and provides additional functionality for loading, scaling, and displaying images.
 */
class Image : public wxImage {
public:
    // Public member variables
    double m_scale = 1.0;

    // Constructors
    Image();

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
    Image(const wxString &filename, wxPoint position = wxPoint(0, 0), wxSize imageSize = wxSize(1200, 400), wxSize scaledSize = wxSize(1200, 400), double scale = 1.0);

    // Getters
    wxString getFilename() const;

    wxPoint getPosition() const;

    int getPositionX() const;

    int getPositionY() const;

    wxPoint getMarkedPoint() const;

    int getMarkedPointX() const;

    int getMarkedPointY() const;

    wxSize getPanelSize() const;

    int getPanelWidth() const;

    int getPanelHeight() const;

    wxSize getFullSize() const;

    int getFullWidth() const;

    int getFullHeight() const;

    wxSize getScaledSize() const;

    int getScaledWidth() const;

    int getScaledHeight() const;

    double getScale() const;

    // Setters
    void setFilename(const wxString &filename);

    void setPosition(const wxPoint &position);

    void setPosition(int x, int y);

    void setPositionX(int x);

    void setPositionY(int y);

    void setMarkedPoint(const wxPoint &point);

    void setMarkedPoint(int x, int y);

    void setPanelSize(const wxSize &size);

    void setPanelSize(int width, int height);

    void setFullSize(const wxSize &size);

    void setFullSize(int width, int height);

    void setFullWidth(int width);

    void setFullHeight(int height);

    void setScaledSize(const wxSize &size);

    void setScaledSize(int width, int height);

    void setScaledWidth(int width);

    void setScaledHeight(int height);

    void setScale(double scale);

    // Public methods
    /**
     * Scales the image and updates the display bitmap.
     * TODO: scale the image around the moved center point, not the panel center.
     * 
     * @param bitmapDisplay The wxStaticBitmap object used to display the image.
     */
    void ScaleImage(wxStaticBitmap *bitmapDisplay);

    void FitImage(wxStaticBitmap *bitmapDisplay);

    /**
     * Loads and scales an image file to fit within the specified wxStaticBitmap control.
     * 
     * @param filename The path to the image file to load.
     * @param bitmapDisplay The wxStaticBitmap control to display the scaled image.
     */
    void LoadAndScaleImage(wxString filename, wxStaticBitmap *bitmapDisplay);

    Image CustomSubImage(SubBmpRect rect);

    
    /**
     * @brief Zooms in the bitmap.
     * 
     * @param bitmapDisplay The static bitmap to zoom.
     * @param image The image object containing the bitmap.
     */
    void zoomIn(wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Zooms out the bitmap.
     * 
     * @param bitmapDisplay The static bitmap to zoom.
     * @param image The image object containing the bitmap.
     */
    void zoomOut(wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Moves the bitmap to the left.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveLeft(wxStaticBitmap *bitmapDisplay, int step = 10);

    /**
     * @brief Moves the bitmap to the right.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveRight(wxStaticBitmap *bitmapDisplay, int step = 10);

    /**
     * @brief Moves the bitmap up.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveUp(wxStaticBitmap *bitmapDisplay, int step = 10);

    /**
     * @brief Moves the bitmap down.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveDown(wxStaticBitmap *bitmapDisplay, int step = 10);

private:
    wxString m_filename = "";
    wxPoint m_position = wxPoint(0, 0);
    wxPoint m_markedPoint = wxPoint(0, 0);
    wxSize m_panelSize = wxSize(0, 0);
    wxSize m_fullSize = wxSize(0, 0);
    wxSize m_scaledSize = wxSize(0, 0);
    wxStaticBitmap *m_bitmapDisplay;
};