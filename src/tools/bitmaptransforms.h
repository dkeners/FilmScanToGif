#pragma once

#include <wx/wx.h>
#include "../LoadedImage.h"

/**
 * @brief The BitmapTransforms class provides a set of functions for transforming bitmaps.
 *
 * This class encapsulates various bitmap transformation operations, such as scaling, rotating,
 * flipping, and cropping. It provides a convenient interface for applying these transformations
 * to wxWidgets bitmaps.
 */
class BitmapTransforms
{
public:
    /**
     * @brief Constructs a BitmapTransforms object.
     *
     * @param bitmap The wxStaticBitmap object to be transformed.
     * @param image The Image object associated with the bitmap.
     * @param parent The parent window for the BitmapTransforms object.
     */
    BitmapTransforms(wxStaticBitmap *bitmap, Image *image = nullptr, wxWindow *parent = nullptr);

    /**
     * @brief Loads all controls.
     * 
     * @return true if all controls are successfully loaded, false otherwise.
     */
    bool loadAllControls();

    /**
     * @brief Loads all controls.
     * 
     * @param parent The parent window where the controls will be loaded.
     * @param bitmapDisplay The static bitmap to control.
     * @param image The image object containing the bitmap to be displayed.
     * @return True if all controls were successfully loaded and the bitmap was displayed, false otherwise.
     */
    bool loadAllControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image);

    /**
     * @brief Loads zoom controls.
     * 
     * @return true if zoom controls are successfully loaded, false otherwise.
     */
    bool loadZoomControls();

    /**
     * @brief Loads zoom controls.
     * 
     * @param parent The parent window where the controls will be loaded.
     * @param bitmapDisplay The static bitmap to control.
     * @param image The image object containing the bitmap to be displayed.
     * @return True if zoom controls were successfully loaded and the bitmap was displayed, false otherwise.
     */
    bool loadZoomControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay, Image *image);

    /**
     * @brief Loads move controls.
     * 
     * @return true if move controls are successfully loaded, false otherwise.
     */
    bool loadMoveControls();

    /**
     * @brief Loads move controls.
     * 
     * @param parent The parent window where the controls will be loaded.
     * @param bitmapDisplay The static bitmap to control.
     * @return True if move controls were successfully loaded and the bitmap was displayed, false otherwise.
     */
    bool loadMoveControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Loads rotate controls.
     * 
     * @return true if rotate controls are successfully loaded, false otherwise.
     */
    bool loadRotateControls();

    /**
     * @brief Loads rotate controls.
     * 
     * @param parent The parent window where the controls will be loaded.
     * @param bitmapDisplay The static bitmap to control.
     * @return True if rotate controls were successfully loaded and the bitmap was displayed, false otherwise.
     */
    bool loadRotateControls(wxWindow *parent, wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Creates a control sizer.
     * 
     * @param padding The padding value for the sizer.
     * @return The created wxBoxSizer object.
     */
    wxBoxSizer *createControlSizer(int padding = 5);

    /**
     * @brief Zooms in the bitmap.
     * 
     * @param bitmapDisplay The static bitmap to zoom.
     * @param image The image object containing the bitmap.
     */
    void zoomIn(wxStaticBitmap *bitmapDisplay, Image *image);

    /**
     * @brief Zooms out the bitmap.
     * 
     * @param bitmapDisplay The static bitmap to zoom.
     * @param image The image object containing the bitmap.
     */
    void zoomOut(wxStaticBitmap *bitmapDisplay, Image *image);

    /**
     * @brief Moves the bitmap to the left.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveLeft(wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Moves the bitmap to the right.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveRight(wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Moves the bitmap up.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveUp(wxStaticBitmap *bitmapDisplay);

    /**
     * @brief Moves the bitmap down.
     * 
     * @param bitmapDisplay The static bitmap to move.
     */
    void moveDown(wxStaticBitmap *bitmapDisplay);

private:
    wxWindow *parent_; // The parent window
    wxStaticBitmap *staticBitmap_; // The wxStaticBitmap object
    Image *image_; // The Image object associated with the bitmap

    bool loadedZoomControls_ = false; // Flag indicating if zoom controls are loaded
    bool loadedMoveControls_ = false; // Flag indicating if move controls are loaded
    bool loadedRotateControls_ = false; // Flag indicating if rotate controls are loaded

    wxButton *button_zoom; // The zoom button
    wxButton *button_zoom_out; // The zoom out button
    wxButton *button_left; // The left button
    wxButton *button_right; // The right button
    wxButton *button_up; // The up button
    wxButton *button_down; // The down button
    wxButton *button_rotate_left; // The rotate left button
    wxButton *button_rotate_right; // The rotate right button
};
