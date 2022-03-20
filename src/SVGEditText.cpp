
#include "SVGEditText.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"

CSVGEditText::CSVGEditText(CSVGDocument *pDocument, int nType)
	:CSVGEditBase(pDocument)
	,m_nType(nType)
{
	m_pToolTip = new CToolTipText(pDocument);
}

CSVGEditText::~CSVGEditText()
{
}

void CSVGEditText::OnLButtonDown(const POINT& point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	m_pShape = (CSVGElementShape*)(m_pDocument->createElement("text"));
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

	m_pShape->setAttribute("stroke", "#FF0000");
	m_pShape->setAttribute("stroke-width", "1");

	m_pShape->setAttribute("x", ptx);
	m_pShape->setAttribute("y", pty);

	std::string strstroke = ColorToStr(m_pDocument->GetDefaultStroke());
	COLORREF fontclr = m_pDocument->GetDefaultFontColor();
	std::string strfontclr = ColorToStr(fontclr);

	if (m_nType == TOOLTYPE_TEXTV)
	{
		m_pShape->setAttribute("writing-mode", "tb");
	}
	m_pShape->setAttribute("xml:space", "preserve");
	m_pShape->setAttribute("stroke-opacity", "0");
	m_pShape->setAttribute("fill-opacity", "1");
	m_pShape->setAttribute("fill", strfontclr.c_str());
	m_pShape->setAttribute("font-family", m_pDocument->GetDefaultFontName());
	m_pShape->setFloatAttribute("font-size", (float)m_pDocument->GetDefaultFontSize());
	
	CSVGElement *pTextContent = m_pDocument->createElement("textcontent");
	m_pShape->addChild(pTextContent);
	m_pShape->InitUpdate();
	pTextContent->InitUpdate();

	m_pDocument->unSelectAll();
	m_pDocument->addSelectShape(m_pShape);

	m_pShape->resetPath();
	m_pShape->resetFocus();
	m_pShape->cssChange();
	m_pDocument->ReSetElementToArea(m_pShape);

	m_pDocument->Draw(NULL);
	m_pDocument->SetModify();
	m_pDocument->ReSetAllElementIndex();
}

void CSVGEditText::OnLButtonUp(const POINT& point)
{
	CSVGEditBase::OnLButtonUp(point);
	EndAction();
	m_pDocument->ActionElementAdd(m_pShape);
	m_pDocument->Draw(NULL);
}

void CSVGEditText::OnMouseMove(const POINT& point)
{
	CSVGEditBase::OnMouseMove(point);
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