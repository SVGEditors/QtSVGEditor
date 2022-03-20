
#include "SVGEditPath.h"
#include "resource.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "SVGPathSeg.h"
#include "SVGElementPath.h"

CSVGEditPath::CSVGEditPath(CSVGDocument *pDocument, int nType, CSVGElementShape* pShape)
	:CSVGEditBase(pDocument)
	, m_nType(nType)
{
	m_pPathLineto = NULL;
	m_bContinueEdit = false;
	m_pActionAttr = NULL;
	m_pShape = pShape;
	if (m_pShape &&m_pShape->getNodeType() == SVG_PATH)
	{
		m_bContinueEdit = true;
		m_pActionAttr = new CActionAttr(m_pDocument, m_pShape, "d");
		if (nType == TOOLTYPE_CONTINUE_PATH)
		{
			m_nType = TOOLTYPE_PATH;
			CSVGElementPath *pPathElement = (CSVGElementPath*)pShape;
			m_pPathSeg = pPathElement->getLastSeg();
			m_lstCtrl = pPathElement->getLastPoint();
		}
		else if (nType == TOOLTYPE_CONTINUE_ARC)
		{
			m_nType = TOOLTYPE_ARC;
			CSVGElementPath *pPathElement = (CSVGElementPath*)pShape;
			CSVGPoint pt = pPathElement->getLastPoint();
			m_pPathSeg = pPathElement->createSVGPathSegArc(1, 1, 0, pt.x, pt.y);
		}
	}
}

CSVGEditPath::~CSVGEditPath()
{
}

void CSVGEditPath::OnLButtonDown(const POINT& point)
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

	if (m_nType == TOOLTYPE_PATH)
	{
		if (m_pShape == NULL)
		{//第一次按下
			m_pShape = (CSVGElementShape*)(m_pDocument->createElement("path"));
			CSVGElementPath *pPathElement = (CSVGElementPath*)m_pShape;
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

			m_pPathSeg = pPathElement->createSVGPathSegMoveto((float)pt.x, (float)pt.y);
			m_lstCtrl = m_lstCtrlMove = pt;
			m_pDocument->unSelectAll();
			m_pDocument->addSelectShape(m_pShape);
			m_pShape->cssChange();
			m_pShape->resetPath();
			m_pShape->resetFocus();
			m_pDocument->ReSetElementToArea(m_pShape);
			m_pDocument->ReSetAllElementIndex();
		}
		else
		{//第二次按下
			CSVGElementPath *pPathElement = (CSVGElementPath*)m_pShape;
			if (m_pPathSeg && m_pPathSeg->getType() == PATH_C)
			{//如果上次的点是C
				CSVGPathSegCurvetoCubic *pSegAbs = (CSVGPathSegCurvetoCubic*)m_pPathSeg;
				if (DB_EQUAL(pSegAbs->x1, pSegAbs->x2)
					&& DB_EQUAL(pSegAbs->y1, pSegAbs->y2))
				{//如果上次的控制点相同,则这次创建L
					m_pPathSeg = pPathElement->createSVGPathSegLineto((float)pt.x, (float)pt.y);
				}
				else
				{//上次的控制点不同,则这次一定是C
					m_pPathSeg = pPathElement->createSVGPathSegCurvetoCubic(m_lstCtrl.x, m_lstCtrl.y, m_lstCtrl.x, m_lstCtrl.y, (float)pt.x, (float)pt.y);
				}
			}
			else
			{//创建L
				m_pPathSeg = pPathElement->createSVGPathSegLineto((float)pt.x, (float)pt.y);
			}
			m_pShape->resetPath();
			m_pShape->resetFocus();
			m_pDocument->ReSetElementToArea(m_pShape);
		}
	}
	else if (m_nType == TOOLTYPE_PATHARC)
	{
		m_pShape = (CSVGElementShape*)(m_pDocument->createElement("path"));
		CSVGElementPath *pPathElement = (CSVGElementPath*)m_pShape;
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

		pPathElement->createSVGPathSegMoveto((float)pt.x, (float)pt.y);
		m_pPathSeg = pPathElement->createSVGPathSegArc(1,1,0,pt.x,pt.y);

		m_pDocument->unSelectAll();
		m_pDocument->addSelectShape(m_pShape);
		m_pShape->cssChange();
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);
		m_pDocument->ReSetAllElementIndex();
	}
	else if (m_nType == TOOLTYPE_PATHFAN)
	{
		m_pShape = (CSVGElementShape*)(m_pDocument->createElement("path"));
		CSVGElementPath *pPathElement = (CSVGElementPath*)m_pShape;
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

		pPathElement->createSVGPathSegMoveto((float)pt.x, (float)pt.y);
		m_pPathSeg = pPathElement->createSVGPathSegArc(1, 1, 0, pt.x, pt.y);

		m_pPathLineto = pPathElement->createSVGPathSegLineto(pt.x, pt.y);

		pPathElement->createSVGPathSegClose();

		m_pDocument->unSelectAll();
		m_pDocument->addSelectShape(m_pShape);
		m_pShape->cssChange();
		m_pShape->resetPath();
		m_pShape->resetFocus();
		m_pDocument->ReSetElementToArea(m_pShape);
		m_pDocument->ReSetAllElementIndex();
	}
	m_pDocument->Draw(NULL);
	m_pDocument->SetModify();
}

void CSVGEditPath::OnLButtonUp(const POINT& point)
{
	CSVGEditBase::OnLButtonUp(point);
	m_lstCtrl = m_lstCtrlMove;
	if (m_nType == TOOLTYPE_PATHARC || m_nType == TOOLTYPE_PATHFAN)
	{
		EndAction();
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
		m_pShape = NULL;
		m_pPathSeg = NULL;
		m_pPathLineto = NULL;
		m_pDocument->ActionElementAdd(m_pShape);
	}
}

void CSVGEditPath::OnMouseMove(const POINT& point)
{
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	if (NULL == m_pShape)
	{
		return;
	}
	const CSVGMatrix *matrix = m_pDocument->getScreenMatrix();
	CSVGMatrix mat = *matrix;
	if (m_pShape)
	{
		mat.Add(*m_pShape->getUsedMatrix());
	}
	mat.Inverse();
	CSVGPoint pt = CSVGPoint((float)point.x, (float)point.y);
	pt = pt.matrixTransform(mat);

	CSVGPoint downpt = CSVGPoint((float)m_downPoint.x, (float)m_downPoint.y);
	downpt = downpt.matrixTransform(mat);

	CSVGElementPath *pPathElement = (CSVGElementPath*)m_pShape;

	if (!PT_EQUAL(m_lastPoint, point))
	{//点中的点和移动的点不同
		if (m_bLButtonDown && m_pPathSeg)
		{//鼠标左键按住
			if (m_pPathSeg->getType() == PATH_L)
			{//是L,变为C
				CSVGPathSegLineto *pLineAbs = (CSVGPathSegLineto*)m_pPathSeg;
				//移除最后一个点
				CSVGPoint temppt(pLineAbs->x, pLineAbs->y);
				pPathElement->removeSeg(m_pPathSeg);
				m_pPathSeg = NULL;				
				//增加新的点
				float x1 = (float)temppt.x * 2 - pt.x;
				float y1 = (float)temppt.y * 2 - pt.y;
				m_lstCtrlMove = pt;
				m_pPathSeg = pPathElement->createSVGPathSegCurvetoCubic(m_lstCtrl.x, m_lstCtrl.y, x1, y1, (float)temppt.x, (float)temppt.y);
				pPathElement->setControlPoint(CSVGPoint(x1, y1), pt, temppt);
				m_lstCtrl = pt;
			}
			else if (m_pPathSeg->getType() == PATH_C)
			{//原来是C,则改变控制点坐标
				CSVGPathSegCurvetoCubic *pCurveAbs = (CSVGPathSegCurvetoCubic*)m_pPathSeg;
				float x1 = pCurveAbs->x * 2 - pt.x;
				float y1 = pCurveAbs->y * 2 - pt.y;
				pPathElement->changeCurvetoCubic(pCurveAbs, m_lstCtrl.x, m_lstCtrl.y, x1, y1);
				pPathElement->setControlPoint(CSVGPoint(x1, y1), pt, CSVGPoint(pCurveAbs->x, pCurveAbs->y));
				m_lstCtrlMove = pt;
			}
			else if (m_pPathSeg->getType() == PATH_A)
			{
				CSVGPathSegArc *pArc = (CSVGPathSegArc*)m_pPathSeg;
				pArc->x = pt.x;
				pArc->y = pt.y;
				pArc->r1 = (pt.x - downpt.x) / 2;
				pArc->r2 = (pt.y - downpt.y) / 2;
				if (m_pPathLineto)
				{
					m_pPathLineto->x = downpt.x + pArc->r1;
					m_pPathLineto->y = downpt.y + pArc->r2;
				}
			}
		}
	}
	m_pShape->resetPath();
	m_pShape->resetFocus();
	m_pDocument->ReSetElementToArea(m_pShape);
	m_pDocument->Draw(NULL);
	CSVGEditBase::OnMouseMove(point);
}

void CSVGEditPath::OnRButtonUp(const POINT &point)
{
	CSVGEditBase::OnRButtonUp(point);
	EndAction();
	::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_DRAW)));
	m_pShape   = NULL;
	m_pPathSeg = NULL;
	m_pPathLineto = NULL;
	if (!m_bContinueEdit)
	{
		m_pDocument->ActionElementAdd(m_pShape);
	}
	else
	{
		m_pActionAttr->EndValue();
		m_pDocument->GetActionHistory()->AddAction(m_pActionAttr);
	}
}