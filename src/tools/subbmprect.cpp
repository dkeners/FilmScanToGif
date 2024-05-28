#include "subbmprect.h"

SubBmpRect::SubBmpRect() : wxRect() {}

SubBmpRect::SubBmpRect(int x, int y, int width, int height)
    : wxRect(x, y, width, height) {}

bool SubBmpRect::scaleFrame(double scale)
{
    if (this->IsEmpty()) {
        return false;
    }
    wxPoint point1 = this->GetTopLeft();
    wxPoint point2 = this->GetBottomRight();
    double x1 = point1.x * scale;
    double y1 = point1.y * scale;
    double x2 = point2.x * scale;
    double y2 = point2.y * scale;
    this->SetTopLeft(wxPoint(x1, y1));
    this->SetBottomRight(wxPoint(x2, y2));
}