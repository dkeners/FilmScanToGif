# Changes in wxWidgets source for project to compile
These changes were made to the following files in the wxWidgets source code to compile the project, hopefully they will be included in the next release of wxWidgets. Regardless, they are here for reference.

#### `include/wx/imaggif.h`
Creates a save animation function that takes a vector of delays for each frame, this allows for the frame delay to change at each frame. *Inserted after the first SaveAnimation function.*
```cpp
    /* After the first SaveAnimation - around line 52 */
    bool SaveAnimation(const wxImageArray& images, wxOutputStream *stream,
        bool verbose, std::vector<int>& delaysMilliSecs);
```
#### `src/common/imaggif.cpp`:
Creates a save animation function that takes a vector of delays for each frame, this allows for the frame delay to change at each frame. *Inserted after the first SaveAnimation function.*
```cpp
/* After the first SaveAnimation - around line 321 */
bool wxGIFHandler::SaveAnimation(const wxImageArray& images,
    wxOutputStream *stream, bool verbose, std::vector<int>& delaysMilliSecs)
{
#if wxUSE_PALETTE
    bool ok = true;
    size_t i;

    ok = ok && (delaysMilliSecs.size() == images.GetCount());
    wxSize size(0,0);
    for (i = 0; (i < images.GetCount()) && ok; i++)
    {
        const wxImage& image = images.Item(i);
        wxSize temp(image.GetWidth(), image.GetHeight());
        ok = ok && image.HasPalette();
        if (i)
        {
           ok = ok && (size == temp);
        }
        else
        {
           size = temp;
        }
    }

    for (i = 0; (i < images.GetCount()) && ok; i++)
    {
        const wxImage& image = images.Item(i);
        const int delayMilliSecs = delaysMilliSecs[i];

        wxRGB pal[256];
        int palCount;
        int maskIndex;

        ok = wxGIFHandler_GetPalette(image, pal, &palCount, &maskIndex)
          && DoSaveFile(image, stream, verbose, i == 0 /*first?*/, delayMilliSecs,
            true /*loop?*/, pal, palCount, maskIndex);
    }

    return ok && wxGIFHandler_WriteTerm(stream);
#else
    wxUnusedVar(images);
    wxUnusedVar(stream);
    wxUnusedVar(verbose);
    wxUnusedVar(delayMilliSecs);

    return false;
#endif
}
```

Updates the `wxGIFHandler_WriteControl` function to include the disposal method in the control block.
```diff
bool wxGIFHandler_WriteControl(wxOutputStream *stream,
    int maskIndex, int delayMilliSecs)
{
    wxUint8 buf[8];
    const wxUint16 delay = delayMilliSecs / 10;

    buf[0] = GIF_MARKER_EXT;    // extension marker
    buf[1] = GIF_MARKER_EXT_GRAPHICS_CONTROL;
    buf[2] = 4;     // length of block
-   buf[3] = (maskIndex != wxNOT_FOUND) ? 1 : 0;   // has transparency
+   buf[3] = (maskIndex != wxNOT_FOUND) ? 1 | 2 << 2 : 0;   // has transparency and disposal method
    buf[4] = delay & 0xff;  // delay time
    buf[5] = (delay >> 8) & 0xff;   // delay time second byte
    buf[6] = (maskIndex != wxNOT_FOUND) ? (wxUint8) maskIndex : 0;
    buf[7] = 0;
    return wxGIFHandler_Write(stream, buf, sizeof(buf));
}