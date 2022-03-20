#pragma once
#include "UseNamespace.h"

void GetLinePath(Gdiplus::GraphicsPath &path,LineElement* pElement);
void GetRectPath(Gdiplus::GraphicsPath &path,RectElement* pElement);
void GetCirclePath(Gdiplus::GraphicsPath &path,CircleElement* pElement);
void GetEllipsePath(Gdiplus::GraphicsPath &path,EllipseElement* pElement);
void GetImagePath(Gdiplus::GraphicsPath &path,ImageElement* pElement);
void GetPolylinePath(Gdiplus::GraphicsPath &path,PolylineElement* pElement);
void GetPolygonPath(Gdiplus::GraphicsPath &path,PolygonElement* pElement);
void GetPathPath(Gdiplus::GraphicsPath &path,PathElement* pElement);
void GetTextPath(Gdiplus::GraphicsPath &path,TextElement* pElement);
void ReSetMarkPath(LineElement* pElement);
void GetMarkPath(Gdiplus::GraphicsPath &path,LineElement* pElement);
void ReSetMarkPath(LineElement* pElement);

void GetTextPath(Gdiplus::GraphicsPath &path,std::string text);

Gdiplus::Image *GetImage(ImageElement* pElement,std::string workpath);