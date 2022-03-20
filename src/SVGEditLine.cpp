
#include "SVGEditLine.h"
#include "SVGDocument.h"
#include "resource.h"
#include "SVGElementLine.h"

CSVGEditLine::CSVGEditLine(CSVGDocument *pDocument)
	:CSVGEditBase(pDocument)
{
	m_pToolTip = new CToolTipText(pDocument);
}

CSVGEditLine::~CSVGEditLine()
{
}

void CSVGEditLine::OnLButtonDown(const POINT& point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	m_pShape = (CSVGElementShape*)(m_pDocument->createElement("line"));
	m_pDocument->getRootElement()->addChild(m_pShape);
	const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
	CSVGMatrix mat = *matrix;
	mat.Inverse();
	m_pt1 = CSVGPoint((float)point.x, (float)point.y);
	m_pt1 = m_pt1.matrixTransform(mat);
	char ptx[32];
	char pty[32];
	sprintf_s(ptx, "%d", (int)m_pt1.x);
	sprintf_s(pty, "%d", (int)m_pt1.y);
	
	m_pShape->setAttribute("stroke", "#FF0000");
	m_pShape->setAttribute("stroke-width", "1");
	
	m_pShape->setAttribute("x1", ptx);
	m_pShape->setAttribute("y1", pty);

	m_pShape->setAttribute("x2", ptx);
	m_pShape->setAttribute("y2", pty);

	std::string strstroke = ColorToStr(m_pDocument->GetDefaultStroke());
	float stroketwidth = m_pDocument->GetDefaultStrokeWidth();
	const char *pDashLine = m_pDocument->GetDefaultDashLine();

	m_pShape->setAttribute("stroke", strstroke.c_str());
	m_pShape->setFloatAttribute("stroke-width", stroketwidth);
	if (pDashLine &&strlen(pDashLine) > 0)
	{
		m_pShape->setAttribute("stroke-dasharray", pDashLine);
	}
	m_pDocument->unSelectAll();
	m_pDocument->addSelectShape(m_pShape);
	m_pShape->setAttribute("stroke-opacity", "1");
	m_pShape->resetPath();
	m_pShape->cssChange();
	m_pDocument->ReSetElementToArea(m_pShape);
	m_pDocument->SetModify();
	m_pDocument->ReSetAllElementIndex();
}

void CSVGEditLine::OnLButtonUp(const POINT& point)
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
		m_pShape = NULL;
		return;
	}
	EndTo(point);
	m_pDocument->ActionElementAdd(m_pShape);
}

void CSVGEditLine::OnMouseMove(const POINT& point)
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

void CSVGEditLine::EndTo(const POINT& point)
{
	if (m_pShape)
	{
		const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
		CSVGMatrix mat = *matrix;
		mat.Inverse();
		CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
		pt = pt.matrixTransform(mat);
		char ptx[32];
		char pty[32];
		sprintf_s(ptx, "%.2f", pt.x);
		sprintf_s(pty, "%.2f", pt.y);
		m_pShape->setAttribute("x2", ptx);
		m_pShape->setAttribute("y2", pty);

		if (GetKeyState(VK_SHIFT) < 0)
		{
			char buf[128];
			memset(buf, 0, 128);
			float bdiv = pt.y - m_pt1.y;
			float div = pt.x - m_pt1.x;

			if (abs(div) < 0.000000001)
				div = (float)0.000000001;
			float k = bdiv / div;
			float newy2 = pt.y;
			if (k >= 0)
			{
				if (k < 0.84)//40度
				{
					const char *strpy1 = m_pShape->getAttribute("y1");
					m_pShape->setAttribute("y2", strpy1);
				}
				else if (k >= 0.84 && k <= 1.2)//50度
				{
					newy2 = (pt.x - m_pt1.x) + m_pt1.y;
					memset(buf, 0, 128);
					sprintf_s(buf, "%.2f", newy2);
					m_pShape->setAttribute("y2", buf);
				}
				else if (k > 1.2)
				{
					const char *strpx1 = m_pShape->getAttribute("x1");
					m_pShape->setAttribute("x2", strpx1);
				}
			}
			if (k < 0)
			{
				if (k < -1.2)
				{
					const char *strpx1 = m_pShape->getAttribute("x1");
					m_pShape->setAttribute("x2", strpx1);
				}
				else if (k >= -1.2 && k <= -0.84)
				{
					newy2 = -(pt.x - m_pt1.x) + m_pt1.y;
					memset(buf, 0, 128);
					sprintf_s(buf, "%.2f", newy2);
					m_pShape->setAttribute("y2", buf);
				}
				else if (k > -0.84)
				{
					const char *strpy1 = m_pShape->getAttribute("y1");
					m_pShape->setAttribute("y2", strpy1);
				}
			}
		}
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);

		m_pDocument->Draw(NULL);
	}
}