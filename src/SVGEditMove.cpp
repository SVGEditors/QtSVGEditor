
#include "SVGEditMove.h"
#include "SVGDocument.h"
#include "resource.h"
#include "ToolFun.h"

CSVGEditMove::CSVGEditMove(CSVGDocument *pDocument)
	:CSVGEditBase(pDocument)
{
}

CSVGEditMove::~CSVGEditMove()
{
}

void CSVGEditMove::OnLButtonDown(const POINT &point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_HANDCLOSE)));
}

void CSVGEditMove::OnMButtonDown(const POINT &point)
{
	OnLButtonDown(point);
}

void CSVGEditMove::OnLButtonUp(const POINT &point)
{
	CSVGEditBase::OnLButtonUp(point);
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_HANDOPEN)));
}

void CSVGEditMove::OnMouseMove(const POINT &point)
{
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_HANDOPEN)));
	if (m_bLButtonDown)
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_HANDCLOSE)));
		if(fabs(m_lastPoint.x-point.x) > 0 || fabs(m_lastPoint.y-point.y) > 0)
		{
			m_pDocument->Move(m_lastPoint, point);
		}
	}
	CSVGEditBase::OnMouseMove(point);
}
void CSVGEditMove::OnRButtonUp(const POINT &point)
{
	CSVGEditBase::OnRButtonUp(point);
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	m_bFinish = true;
}