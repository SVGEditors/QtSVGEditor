
#include "SVGEditPolyline.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "resource.h"
#include "SVGElementPolyline.h"
#include "ActionBase.h"

CSVGEditPolyline::CSVGEditPolyline(CSVGDocument *pDocument, int nType, CSVGElementShape *pShape)
	:CSVGEditBase(pDocument)
	,m_nType(nType)
{
	m_pShape = NULL;
	m_bContinueEdit = false;
	m_pActionAttr = NULL;
	if (pShape)
	{
		m_bContinueEdit = true; 
		m_pShape = pShape;
		m_pActionAttr = new CActionAttr(pDocument, m_pShape, "points");
	}
	m_pToolTip = new CToolTipText(pDocument);
}

CSVGEditPolyline::~CSVGEditPolyline()
{
}

void CSVGEditPolyline::OnLButtonDown(const POINT& point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
	CSVGMatrix mat = *matrix;
	if (m_pShape)
	{
		mat.Add(*m_pShape->getUsedMatrix());
	}
	mat.Inverse();
	CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
	pt = pt.matrixTransform(mat);
	pt.x = (int)pt.x;
	pt.y = (int)pt.y;

	if (m_pShape == NULL)
	{
		if (m_nType == TOOLTYPE_POLYLINE)
		{
			m_pShape = (CSVGElementShape*)(m_pDocument->createElement("polyline"));
		}
		else
		{
			m_pShape = (CSVGElementShape*)(m_pDocument->createElement("polygon"));
		}
		CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)m_pShape;
		m_pDocument->getRootElement()->addChild(m_pShape);
		

		std::string strstroke = ColorToStr(m_pDocument->GetDefaultStroke());
		float stroketwidth = m_pDocument->GetDefaultStrokeWidth();
		const char *pDashLine = m_pDocument->GetDefaultDashLine();

		m_pShape->setAttribute("stroke", strstroke.c_str());
		m_pShape->setFloatAttribute("stroke-width", stroketwidth);
		if (pDashLine &&strlen(pDashLine) > 0)
		{
			m_pShape->setAttribute("stroke-dasharray", pDashLine);
		}
		m_pShape->setAttribute("stroke-opacity", "1");
		m_pShape->setAttribute("fill-opacity", "0");

		pPolyline->appendPoint(pt);
		//增加另一个点作为橡皮效果
		pPolyline->appendPoint(pt);
		m_pDocument->unSelectAll();
		m_pShape->cssChange();
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);
		m_pDocument->ReSetAllElementIndex();
		m_pDocument->addSelectShape(m_pShape);
		m_pDocument->Draw(NULL);
	}
	else
	{//第二次按下
		CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)m_pShape;
		pPolyline->appendPoint(pt);
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);
		m_pDocument->Draw(NULL);
	}
	m_pDocument->SetModify();
}

void CSVGEditPolyline::OnLButtonUp(const POINT& point)
{
	CSVGEditBase::OnLButtonUp(point);
	EndTo(point);
}
void   CSVGEditPolyline::Finish()
{
	EndAction();
	CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)m_pShape;
	pPolyline->removeLast();
	if (!m_bContinueEdit)
	{//新建的图形
		if (!pPolyline->isValidShape())
		{
			m_pDocument->unSelectShape(m_pShape);
			m_pShape->remove();
			m_pDocument->ReMoveElementFromArea(m_pShape);
			delete m_pShape;
			m_pShape = NULL;
			m_pDocument->Draw(NULL);
		}
		else
		{
			pPolyline->resetPath();
			pPolyline->resetFocus();
			m_pDocument->ReSetElementToArea(pPolyline);
			m_pDocument->ActionElementAdd(m_pShape);
			m_pDocument->Draw(NULL);
		}
	}
	else
	{//继续绘制的情况
		if (!pPolyline->isValidShape())
		{
			m_pActionAttr->EndValue();
			CActionComposite *pComposite = new CActionComposite(m_pDocument);
			pComposite->PushBack(m_pActionAttr);
			CActionDel *pDelAct = new CActionDel(m_pDocument, m_pShape);
			pComposite->PushBack(pDelAct);
			m_pDocument->GetActionHistory()->AddAction(pComposite);

			m_pDocument->unSelectShape(m_pShape);
			m_pDocument->ReMoveElementFromArea(m_pShape);
			m_pShape->remove();
			m_pShape = NULL;
			m_pDocument->Draw(NULL);
		}
		else
		{
			m_pActionAttr->EndValue();
			m_pDocument->GetActionHistory()->AddAction(m_pActionAttr);
			pPolyline->resetPath();
			pPolyline->resetFocus();
			m_pDocument->ReSetElementToArea(pPolyline);
			m_pDocument->Draw(NULL);
		}
	}
}

void CSVGEditPolyline::OnLButtonDblClk(const POINT& point)
{
	CSVGEditBase::OnLButtonDblClk(point);
	Finish();
}

void CSVGEditPolyline::OnRButtonUp(const POINT &point)
{
	CSVGEditBase::OnRButtonUp(point);
	Finish();
}

void CSVGEditPolyline::OnMouseMove(const POINT& point)
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

void CSVGEditPolyline::EndTo(const POINT& point)
{
	if (m_pShape)
	{
		const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
		CSVGMatrix mat = *matrix;
		mat.Add(*m_pShape->getUsedMatrix());
		mat.Inverse();
		CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
		pt = pt.matrixTransform(mat);
		pt.x = (int)pt.x;
		pt.y = (int)pt.y;
		
		CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)m_pShape;
		pPolyline->setLastPoint(pt);	

		if (GetKeyState(VK_SHIFT) < 0)
		{
			CSVGPoint pt1(0, 0);
			if (pPolyline->getLast2Point(pt1))
			{
				float bdiv = pt.y - pt1.y;
				float div = pt.x - pt1.x;

				if (abs(div) < 0.000000001)
					div = (float)0.000000001;
				float k = bdiv / div;
				float newy2 = pt.y;
				if (k >= 0)
				{
					if (k < 0.84)//40度
					{
						pt.y = pt1.y;
					}
					else if (k >= 0.84 && k <= 1.2)//50度
					{
						newy2 = (pt.x - pt1.x) + pt1.y;
						pt.y = newy2;
					}
					else if (k > 1.2)
					{
						pt.x = pt1.x;
					}
				}
				if (k < 0)
				{
					if (k < -1.2)
					{
						pt.x = pt1.x;
					}
					else if (k >= -1.2 && k <= -0.84)
					{
						newy2 = -(pt.x - pt1.x) + pt1.y;
						pt.y = newy2;
					}
					else if (k > -0.84)
					{
						pt.y = pt1.y;
					}
				}
			}
			pPolyline->setLastPoint(pt);
		}
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);

		m_pDocument->Draw(NULL);
	}
}

void CSVGEditPolyline::OnEscPushDown()
{
	if (m_pShape)
	{
		POINT point;
		::GetCursorPos(&point);
		ScreenToClient(m_pDocument->GetHwnd() , &point);
		const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
		CSVGMatrix mat = *matrix;
		mat.Inverse();
		CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
		pt = pt.matrixTransform(mat);

		CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)m_pShape;
		if (pPolyline->getPointSize() > 2)
		{
			pPolyline->removeLast();
			pPolyline->setLastPoint(pt);
			m_pShape->resetPath();
			m_pShape->resetFocus();
			m_pDocument->ReSetElementToArea(m_pShape);
			m_pDocument->Draw(NULL);
		}
	}
}