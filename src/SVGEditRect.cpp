
#include "SVGEditRect.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "resource.h"

CSVGEditRect::CSVGEditRect(CSVGDocument *pDocument)
	:CSVGEditBase(pDocument)
{
	m_pToolTip = new CToolTipText(pDocument);
}

CSVGEditRect::~CSVGEditRect()
{
}

void CSVGEditRect::OnLButtonDown(const POINT& point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	m_pShape = (CSVGElementShape*)(m_pDocument->createElement("rect"));
	m_pDocument->getRootElement()->addChild(m_pShape);
	const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
	CSVGMatrix mat = *matrix;
	mat.Inverse();
	CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
	pt = pt.matrixTransform(mat);
	char ptx[32];
	char pty[32];
	sprintf_s(ptx, "%d", (int)pt.x);
	sprintf_s(pty, "%d", (int)pt.y);

	
	m_pShape->setAttribute("x", ptx);
	m_pShape->setAttribute("y", pty);

	std::string strfill = ColorToStr(m_pDocument->GetDefaultFill());
	std::string strstroke = ColorToStr(m_pDocument->GetDefaultStroke());
	float stroketwidth = m_pDocument->GetDefaultStrokeWidth();
	const char *pDashLine = m_pDocument->GetDefaultDashLine();

	m_pShape->setAttribute("stroke", strstroke.c_str());
	m_pShape->setAttribute("fill", strfill.c_str());
	m_pShape->setFloatAttribute("stroke-width", stroketwidth);
	if (pDashLine &&strlen(pDashLine) > 0)
	{
		m_pShape->setAttribute("stroke-dasharray", pDashLine);
	}
	m_pDocument->unSelectAll();
	m_pDocument->addSelectShape(m_pShape);
	m_pShape->setAttribute("stroke-opacity", "1");
	m_pShape->setAttribute("fill-opacity", "1");
	m_pShape->resetPath();
	m_pShape->cssChange();
	m_pDocument->ReSetElementToArea(m_pShape);
	m_pDocument->SetModify();
	m_pDocument->ReSetAllElementIndex();
}

void CSVGEditRect::OnLButtonUp(const POINT& point)
{
	CSVGEditBase::OnLButtonUp(point);
	EndAction();
	if (m_lastPoint.x == m_downPoint.x && m_lastPoint.y == m_downPoint.y)
	{//起点和终点相同,移除原来绘制的图形
		m_pDocument->unSelectShape(m_pShape);
		m_pShape->remove();
		m_pDocument->ReMoveElementFromArea(m_pShape);
		delete m_pShape;
		m_pShape = NULL;
		return;
	}
	m_pDocument->ActionElementAdd(m_pShape);
	EndTo(point);
}

void CSVGEditRect::OnMouseMove(const POINT& point)
{
	CSVGEditBase::OnMouseMove(point);
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	EndTo(point);

	const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
	CSVGMatrix mat = *matrix;
	if (m_pShape)
	{
		mat.Add(*m_pShape->getUsedMatrix());
	}
	mat.Inverse();
	CSVGPoint pt = CSVGPoint((float)point.x, (float)(point.y));
	pt = pt.matrixTransform(mat);
	m_pToolTip->SetPoint(point.x, point.y - 20);
	m_pToolTip->SetTextByPoint(pt.x, pt.y);
	m_pDocument->Draw(NULL);

}

void CSVGEditRect::EndTo(const POINT& point)
{
	if (m_pShape)
	{
		const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
		CSVGMatrix mat = *matrix;
		mat.Inverse();
		CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
		pt = pt.matrixTransform(mat);

		CSVGPoint downpt = CSVGPoint(float(m_downPoint.x), (float)(m_downPoint.y));
		downpt = downpt.matrixTransform(mat);

		char szBuf[128];
		memset(szBuf, 0, 128);
		sprintf_s(szBuf, 127, "%d", (int)pt.x);
		if (pt.x < downpt.x)
			m_pShape->setAttribute("x", szBuf);
		memset(szBuf, 0, 128);
		sprintf_s(szBuf, 127, "%d", (int)pt.y);
		if (pt.y < downpt.y)
			m_pShape->setAttribute("y", szBuf);

		float width = (float)abs(pt.x - downpt.x);
		float height = (float)abs(pt.y - downpt.y);

		memset(szBuf, 0, 128);
		sprintf_s(szBuf, 127, "%d", (int)width);
		m_pShape->setAttribute("width", szBuf);
		memset(szBuf, 0, 128);
		sprintf_s(szBuf, 127, "%d", (int)height);
		m_pShape->setAttribute("height", szBuf);

		if (GetKeyState(VK_SHIFT) < 0)
		{
			m_pShape->setAttribute("width", szBuf);
		}
		m_pShape->clearFocus();
		m_pShape->resetFocus();
		m_pShape->resetPath();
		m_pDocument->ReSetElementToArea(m_pShape);

		m_pDocument->Draw(NULL);
	}
}