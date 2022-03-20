
#include "SVGEditZoom.h"
#include "SVGDocument.h"
#include "resource.h"

CSVGEditZoom::CSVGEditZoom(CSVGDocument *pDocument, int nType)
	:CSVGEditBase(pDocument)
	,m_nType(nType)
{

}

CSVGEditZoom::~CSVGEditZoom()
{

}

void CSVGEditZoom::OnLButtonDown(const POINT& point)
{
	CSVGEditBase::OnLButtonDown(point);
	if (m_nType == TOOLTYPE_ZOOMOUT)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ZOOMOUT)));
	}
	else if (m_nType == TOOLTYPE_ZOOMIN)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ZOOMIN)));
	}
}

void CSVGEditZoom::OnLButtonUp(const POINT& point)
{
	CSVGEditBase::OnLButtonUp(point);
	if (m_nType == TOOLTYPE_ZOOMIN)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ZOOMIN)));
		if (abs(m_downPoint.x - point.x) <= 2 || abs(m_downPoint.y - point.y) <= 2)
		{
			m_pDocument->Zoom(float(1.2), float(1.2), point);
		}
		else
		{
			int x1 = m_downPoint.x <= point.x ? m_downPoint.x : point.x;
			int y1 = m_downPoint.y <= point.y ? m_downPoint.y : point.y;
			int x2 = x1 + abs(point.x - m_downPoint.x);
			int y2 = y1 + abs(point.y - m_downPoint.y);
			CSVGPoint pt1 = m_pDocument->mapScreenPoint(x1, y1);
			CSVGPoint pt2 = m_pDocument->mapScreenPoint(x2, y2);
			CSVGRect rect(pt1.x,pt1.y,pt2.x,pt2.y);
			m_pDocument->ZoomRect(rect);
		}
	}
	else if (m_nType == TOOLTYPE_ZOOMOUT)
	{
		m_pDocument->Zoom(float(0.9), float(0.9), point);
	}	
	m_pDocument->clearDragFocus();
	m_pDocument->Draw(NULL);
	m_bLButtonDown = false;
}

void CSVGEditZoom::OnRButtonDown(const POINT &point)
{
	CSVGEditBase::OnRButtonDown(point);
}

void CSVGEditZoom::OnRButtonUp(const POINT &point)
{
	CSVGEditBase::OnRButtonUp(point);
	m_bFinish = true;
}

void CSVGEditZoom::OnMouseMove(const POINT &point)
{
	CSVGEditBase::OnMouseMove(point);
	if (m_nType == TOOLTYPE_ZOOMIN && m_bLButtonDown)
	{
		int x1 = m_downPoint.x <= point.x ? m_downPoint.x : point.x;
		int y1 = m_downPoint.y <= point.y ? m_downPoint.y : point.y;
		int x2 = x1 + abs(point.x - m_downPoint.x);
		int y2 = y1 + abs(point.y - m_downPoint.y);
		CSVGPoint pt1 = m_pDocument->mapScreenPoint(x1, y1);
		CSVGPoint pt2 = m_pDocument->mapScreenPoint(x2, y2);
		m_pDocument->setDragFocus(pt1.x, pt1.y, pt2.x, pt2.y);
		m_pDocument->Draw(NULL);
	}
	else if (m_nType == TOOLTYPE_ZOOMOUT)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ZOOMOUT)));
	}
}
