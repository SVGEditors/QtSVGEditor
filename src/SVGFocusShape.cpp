
#include "SVGFocusShape.h"
#include "SVGElementShape.h"

CSVGFocusShape::CSVGFocusShape(CSVGElementShape *pShape)
{
	m_pShape = pShape;
	m_strokeclr = RGB(0,255,0);
	m_fillclr  = RGB(0, 255,0);
	m_frameclr = RGB(0,0,0);
	m_strokewidth = 1;
	m_pRotateHandle = NULL;
}

CSVGFocusShape::~CSVGFocusShape()
{
	if (m_pRotateHandle)
	{
		delete m_pRotateHandle;
		m_pRotateHandle = NULL;
	}
}

void CSVGFocusShape::SetFillColor(COLORREF clr)
{
	m_fillclr = clr;
}

void CSVGFocusShape::SetStrokeColor(COLORREF clr)
{
	m_strokeclr = clr;
}

void CSVGFocusShape::SetStrokeWidth(float fwidth)
{
	m_strokewidth = fwidth;
}

void CSVGFocusShape::DrawShape(Graphics *pGraphic, const CSVGMatrix *screenmat)
{
	CSVGMatrix mat = *screenmat;
	mat.Add(m_pShape->m_usedmatrix);
	resetPath(&mat);
	pGraphic->ResetTransform();
	Gdiplus::Color lineclr;
	lineclr.SetFromCOLORREF(m_strokeclr);
	Gdiplus::Pen linepen(lineclr, m_strokewidth);
	linepen.SetDashStyle(DashStyleDash);
	pGraphic->DrawPath(&linepen, &m_Linepath);


	Gdiplus::Color fillclr;
	fillclr.SetFromCOLORREF(m_fillclr);
	Gdiplus::SolidBrush brush(fillclr);
	pGraphic->FillPath(&brush, &m_Pointpath);

	Gdiplus::Color frameclr;
	frameclr.SetFromCOLORREF(m_frameclr);
	Gdiplus::Pen framepen(frameclr, m_strokewidth);
	pGraphic->DrawPath(&framepen, &m_Pointpath);
}

void CSVGFocusShape::resetPath(const CSVGMatrix * mat)
{
	m_Linepath.Reset();
	m_Pointpath.Reset();
	for (size_t i = 0; i < m_vcLine.size(); ++i)
	{
		m_Linepath.StartFigure();//不闭合开始新的图形
		CSVGPoint pt1(m_vcLine[i].x1, m_vcLine[i].y1);
		CSVGPoint pt2(m_vcLine[i].x2, m_vcLine[i].y2);
		pt1 = pt1.matrixTransform(*mat);
		pt2 = pt2.matrixTransform(*mat);
		m_Linepath.AddLine(pt1.x, pt1.y, pt2.x, pt2.y);
	}
	for (size_t i = 0; i < m_vcPoint.size(); ++i)
	{
		m_Pointpath.StartFigure();
		CSVGPoint pt1 = m_vcPoint[i];
		pt1 = pt1.matrixTransform(*mat);
		m_Pointpath.AddRectangle(RectF(pt1.x-3, pt1.y-3,6,6));
	}
	if (m_pRotateHandle)
	{
		m_Pointpath.StartFigure();
		CSVGPoint pt1 = *m_pRotateHandle;
		pt1 = pt1.matrixTransform(*mat);
		m_Pointpath.AddEllipse(RectF(pt1.x - 5, pt1.y - 5, 10, 10));
	}
}

void CSVGFocusShape::AddPoint(float x, float y)
{
	m_vcPoint.push_back(CSVGPoint(x,y));
}

void CSVGFocusShape::AddRoteateHandle(float x, float y)
{
	if (!m_pRotateHandle)
	{
		m_pRotateHandle = new CSVGPoint(x, y);
	}
	else
	{
		m_pRotateHandle->x = x;
		m_pRotateHandle->y = y;
	}
}

void CSVGFocusShape::AddLine(float x1, float y1, float x2, float y2)
{
	m_vcLine.push_back(CSVGLine(x1, y1, x2, y2));
}

void CSVGFocusShape::Clear()
{
	m_vcLine.clear();
	m_vcPoint.clear();
}

CSVGRect CSVGFocusShape::GetBBox()
{
	float minx = 0;
	float miny = 0;
	float maxx = 0;
	float maxy = 0;
	for (size_t i = 0; i < m_vcPoint.size(); ++i)
	{
		if (i == 0)
		{
			minx = m_vcPoint[i].x;
			maxx = minx;
			miny = m_vcPoint[i].y;
			maxy = miny;
		}
		else
		{
			minx = min(minx, m_vcPoint[i].x);
			maxx = max(maxx, m_vcPoint[i].x);
			miny = min(miny, m_vcPoint[i].y);
			maxy = max(maxy, m_vcPoint[i].y);
		}		
	}
	CSVGRect bbox(minx, miny, maxx - minx, maxy - miny);
	bbox.Inflate(3, 3);
	if (m_pRotateHandle)
	{
		if (bbox.x > (m_pRotateHandle->x - 5))
		{
			bbox.x = m_pRotateHandle->x - 5;
		}
		if (bbox.y > (m_pRotateHandle->y - 5))
		{
			bbox.y = m_pRotateHandle->y - 5;
		}
		if ((bbox.x + bbox.width) < (m_pRotateHandle->x + 5))
		{
			bbox.width = m_pRotateHandle->x + 5 - bbox.x;
		}
		if ((bbox.y + bbox.height) < (m_pRotateHandle->y + 5))
		{
			bbox.height = m_pRotateHandle->y + 5 - bbox.y;
		}
	}
	return bbox;
}