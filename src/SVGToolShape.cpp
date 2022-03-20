
#include "SVGToolShape.h"
#include "resource.h"

CSVGToolShape::CSVGToolShape(CSVGDocument *pDocument)
	:m_bCapture(false)
	,m_bLButtonDown(false)
	,m_pDocument(pDocument)
{
	m_downPoint.x = 0;
	m_downPoint.y = 0;
	m_lastPoint.x = 0;
	m_lastPoint.y = 0;
	m_bFinish = false;
}


CSVGToolShape::~CSVGToolShape()
{
}

void CSVGToolShape::OnLButtonDown(const POINT& point)
{
	m_downPoint = point;
	m_lastPoint = point;
	m_bLButtonDown = true;
	m_bFinish = false;
}

void CSVGToolShape::OnLButtonUp(const POINT& point)
{
	m_bLButtonDown = false;
}

void CSVGToolShape::OnRButtonDown(const POINT &point)
{

}

void CSVGToolShape::OnRButtonUp(const POINT &point)
{
	if (m_bCapture)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
		::ReleaseCapture();
		m_bCapture = false;
	}
	m_bFinish = true;
}

void CSVGToolShape::OnMouseMove(const POINT& point)
{
	m_lastPoint = point;
}

void CSVGToolShape::OnLButtonDblClk(const POINT& point)
{
	if (m_bCapture)
	{
		::ReleaseCapture();
		m_bCapture = false;
	}
	m_bFinish = true;
}

void CSVGToolShape::OnContextMenu(const POINT &point)
{

}
