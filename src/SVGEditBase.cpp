
#include "SVGEditBase.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "resource.h"

CSVGEditBase::CSVGEditBase(CSVGDocument *pDocument)
	:m_bCapture(false)
	,m_bLButtonDown(false)
	,m_pDocument(pDocument)
	,m_pShape(NULL)
{
	m_downPoint.x = 0;
	m_downPoint.y = 0;
	m_lastPoint.x = 0;
	m_lastPoint.y = 0;
	m_bFinish = false;
	m_pToolTip = NULL;
}

CSVGEditBase::~CSVGEditBase()
{
	if (m_pToolTip)
	{
		delete m_pToolTip;
	}
}

void CSVGEditBase::OnLButtonDown(const POINT& point)
{
	m_downPoint = point;
	m_lastPoint = point;
	m_bLButtonDown = true;
}

void  CSVGEditBase::BeginAction()
{
	m_bFinish = false;
	if (!m_bCapture)
	{
		SetCapture(m_pDocument->GetHwnd());
		m_bCapture = true;
	}
}

void  CSVGEditBase::EndAction()
{
	m_bFinish = true;
	if (m_bCapture)
	{
		m_bCapture = false;
		ReleaseCapture();
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	}
}

void CSVGEditBase::OnLButtonUp(const POINT& point)
{
	m_bLButtonDown = false;
}

void CSVGEditBase::OnRButtonDown(const POINT &point)
{
	m_downPoint = point;
	m_lastPoint = point;
}

void CSVGEditBase::OnMButtonDown(const POINT &point)
{

}

void CSVGEditBase::OnRButtonUp(const POINT &point)
{

}

void CSVGEditBase::OnMouseMove(const POINT& point)
{
	m_lastPoint = point;
}

void CSVGEditBase::OnLButtonDblClk(const POINT& point)
{
	
}

void CSVGEditBase::OnContextMenu(const POINT &point)
{

}

bool CSVGEditBase::IsFinish()
{
	return m_bFinish;
}
