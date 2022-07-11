/**
*
* @license MIT
*
* @copyright: 2022 LJ
*
* @technical support: http://www.svgsvg.cn/support/tech/
*

*
* @module: QtSVGEditor
*
* 版权申明:
*
* 本源码开源基于MIT协议。
*
* 该软件及其相关文档对所有人免费，可以任意处置，包括使用，复制，修改，合并，发表，分发，再授权，或者销售。
*
* 唯一的限制是需要保留我们的许可申明。
*
*/
#include "FocusForElement.h"
#include "SVGElementPolyline.h"
#include "SVGElementPath.h"
#include "SVGElementText.h"

CFocusForElement::CFocusForElement(CSVGElement *pElement, QColor clr)
{
	m_pElement = pElement;
	m_clr = clr;
	m_FlashLine = NULL;
}

CFocusForElement::~CFocusForElement()
{
	if (m_FlashLine)
	{
		delete m_FlashLine;
	}
}

CSVGElement *CFocusForElement::GetElement()
{
	return m_pElement;
}

void CFocusForElement::ResetFocus(int nIndex)
{
	if (m_FlashLine)
	{
		delete m_FlashLine;
		m_FlashLine = NULL;
	}
	m_vcFocusKeyPoints.clear();
	m_vcFocusLine.clear();
	CSVGMatrix usedmat = m_pElement->getUsedMatrix();
	if (m_pElement->getNodeType() == SVG_LINE)
	{
		float fx1 = m_pElement->getFloatAttribute(L"x1");
		float fy1 = m_pElement->getFloatAttribute(L"y1");
		float fx2 = m_pElement->getFloatAttribute(L"x2");
		float fy2 = m_pElement->getFloatAttribute(L"y2");
		CSVGPointF pt1(fx1, fy1);
		CSVGPointF pt2(fx2, fy2);
		pt1 = pt1.matrixTransform(usedmat);
		pt2 = pt2.matrixTransform(usedmat);
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, 0, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, 1, -1, m_pElement, m_clr));
	}
	else if (m_pElement->getNodeType() == SVG_RECT
		|| m_pElement->getNodeType() == SVG_IMAGE)
	{
		float fx = m_pElement->getFloatAttribute(L"x");
		float fy = m_pElement->getFloatAttribute(L"y");
		float fwidth = m_pElement->getFloatAttribute(L"width");
		float fheight = m_pElement->getFloatAttribute(L"height");
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt2(fx + fwidth / 2, fy);
		CSVGPointF pt3(fx + fwidth, fy);
		CSVGPointF pt4(fx + fwidth, fy + fheight / 2);
		CSVGPointF pt5(fx + fwidth, fy + fheight);
		CSVGPointF pt6(fx + fwidth / 2, fy + fheight);
		CSVGPointF pt7(fx, fy + fheight);
		CSVGPointF pt8(fx, fy + fheight / 2);
		pt1 = pt1.matrixTransform(usedmat);
		pt2 = pt2.matrixTransform(usedmat);
		pt3 = pt3.matrixTransform(usedmat);
		pt4 = pt4.matrixTransform(usedmat);
		pt5 = pt5.matrixTransform(usedmat);
		pt6 = pt6.matrixTransform(usedmat);
		pt7 = pt7.matrixTransform(usedmat);
		pt8 = pt8.matrixTransform(usedmat);

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, 10000, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, 10001, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt3, 10002, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt4, 10003, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt5, 10004, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt6, 10005, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt7, 10006, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt8, 10007, -1, m_pElement, m_clr));
	}
	else if (m_pElement->getNodeType() == SVG_CIRCLE)
	{
		float fcx = m_pElement->getFloatAttribute(L"cx");
		float fcy = m_pElement->getFloatAttribute(L"cy");
		float fr = m_pElement->getFloatAttribute(L"r");

		float fx = fcx - fr;
		float fy = fcy - fr;
		float fwidth = 2 * fr;
		float fheight = 2 * fr;
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt2(fx + fwidth / 2, fy);
		CSVGPointF pt3(fx + fwidth, fy);
		CSVGPointF pt4(fx + fwidth, fy + fheight / 2);
		CSVGPointF pt5(fx + fwidth, fy + fheight);
		CSVGPointF pt6(fx + fwidth / 2, fy + fheight);
		CSVGPointF pt7(fx, fy + fheight);
		CSVGPointF pt8(fx, fy + fheight / 2);

		pt1 = pt1.matrixTransform(usedmat);
		pt2 = pt2.matrixTransform(usedmat);
		pt3 = pt3.matrixTransform(usedmat);
		pt4 = pt4.matrixTransform(usedmat);
		pt5 = pt5.matrixTransform(usedmat);
		pt6 = pt6.matrixTransform(usedmat);
		pt7 = pt7.matrixTransform(usedmat);
		pt8 = pt8.matrixTransform(usedmat);

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, 10000, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, 10001, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt3, 10002, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt4, 10003, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt5, 10004, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt6, 10005, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt7, 10006, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt8, 10007, -1, m_pElement, m_clr));

		CSVGLineF line1(pt1, pt3);
		CSVGLineF line2(pt3, pt5);
		CSVGLineF line3(pt5, pt7);
		CSVGLineF line4(pt1, pt7);
		m_vcFocusLine.push_back(FocusLineInfo(line1, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line2, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line3, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line4, Qt::blue));
	}
	else if (m_pElement->getNodeType() == SVG_ELLIPSE)
	{
		float fcx = m_pElement->getFloatAttribute(L"cx");
		float fcy = m_pElement->getFloatAttribute(L"cy");
		float frx = m_pElement->getFloatAttribute(L"rx");
		float fry = m_pElement->getFloatAttribute(L"ry");

		float fx = fcx - frx;
		float fy = fcy - fry;
		float fwidth = 2 * frx;
		float fheight = 2 * fry;
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt2(fx + fwidth / 2, fy);
		CSVGPointF pt3(fx + fwidth, fy);
		CSVGPointF pt4(fx + fwidth, fy + fheight / 2);
		CSVGPointF pt5(fx + fwidth, fy + fheight);
		CSVGPointF pt6(fx + fwidth / 2, fy + fheight);
		CSVGPointF pt7(fx, fy + fheight);
		CSVGPointF pt8(fx, fy + fheight / 2);

		pt1 = pt1.matrixTransform(usedmat);
		pt2 = pt2.matrixTransform(usedmat);
		pt3 = pt3.matrixTransform(usedmat);
		pt4 = pt4.matrixTransform(usedmat);
		pt5 = pt5.matrixTransform(usedmat);
		pt6 = pt6.matrixTransform(usedmat);
		pt7 = pt7.matrixTransform(usedmat);
		pt8 = pt8.matrixTransform(usedmat);

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, 10000, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, 10001, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt3, 10002, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt4, 10003, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt5, 10004, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt6, 10005, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt7, 10006, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt8, 10007, -1, m_pElement, m_clr));

		CSVGLineF line1(pt1, pt3);
		CSVGLineF line2(pt3, pt5);
		CSVGLineF line3(pt5, pt7);
		CSVGLineF line4(pt1, pt7);
		m_vcFocusLine.push_back(FocusLineInfo(line1, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line2, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line3, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line4, Qt::blue));

	}
	else if (m_pElement->getNodeType() == SVG_POLYGON
		|| m_pElement->getNodeType() == SVG_POLYLINE)
	{
		CSVGElementPolyline *pPolyLine = (CSVGElementPolyline*)m_pElement;
		std::list<CSVGPointF> pts = pPolyLine->getPoints();
		std::list<CSVGPointF>::iterator iter = pts.begin();
		for (int i = 0; iter != pts.end(); ++iter, ++i)
		{
			CSVGPointF pt = (*iter).matrixTransform(usedmat);
			m_vcFocusKeyPoints.push_back(FocusPointInfo(pt, i, -1, m_pElement, m_clr));
		}
	}
	else if (m_pElement->getNodeType() == SVG_PATH)
	{
		CSVGElementPath *pPath = (CSVGElementPath*)m_pElement;
		std::list<CSVGPathSeg *> pathseg = pPath->GetPathSeg();
		std::list<CSVGPathSeg *>::iterator iter = pathseg.begin();
		QVector<CSVGPointF>vcPoints;
		m_vcFocusLine.clear();
		for (int i = 0; iter != pathseg.end(); ++iter, ++i)
		{
			if ((*iter)->getType() == PATH_C)
			{
				CSVGPathSegCurvetoCubic *pCubic = (CSVGPathSegCurvetoCubic*)(*iter);
				if (i == nIndex)
				{
					CSVGPointF pt1(2 * pCubic->x - pCubic->x2, 2 * pCubic->y - pCubic->y2);
					pt1 = pt1.matrixTransform(usedmat);
					m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, i, 1, m_pElement, m_clr));

					CSVGPointF pt2(pCubic->x2, pCubic->y2);
					pt2 = pt2.matrixTransform(usedmat);
					m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, i, 0, m_pElement, m_clr));

					CSVGLineF line(pt1, pt2);
					m_vcFocusLine.push_back(FocusLineInfo(line, Qt::blue));
				}
			}
			CSVGPointF pt((*iter)->x, (*iter)->y);
			pt = pt.matrixTransform(usedmat);
			m_vcFocusKeyPoints.push_back(FocusPointInfo(pt, i, -1, m_pElement, m_clr));
		}
	}
	else if (m_pElement->getNodeType() == SVG_TEXT)
	{
		CSVGElementText *pTextElement = (CSVGElementText*)m_pElement;
		CSVGRectF rc = pTextElement->getBBox();
		float fx = rc.x - 5;
		float fy = rc.y - 5;
		float fwidth = rc.width + 10;
		float fheight = rc.height + 10;
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt3(fx + fwidth, fy);
		CSVGPointF pt5(fx + fwidth, fy + fheight);
		CSVGPointF pt7(fx, fy + fheight);
		pt1 = pt1.matrixTransform(usedmat);
		pt3 = pt3.matrixTransform(usedmat);
		pt5 = pt5.matrixTransform(usedmat);
		pt7 = pt7.matrixTransform(usedmat);

		CSVGLineF line1(pt1, pt3);
		CSVGLineF line2(pt3, pt5);
		CSVGLineF line3(pt5, pt7);
		CSVGLineF line4(pt1, pt7);
		m_vcFocusLine.push_back(FocusLineInfo(line1, Qt::white));
		m_vcFocusLine.push_back(FocusLineInfo(line2, Qt::white));
		m_vcFocusLine.push_back(FocusLineInfo(line3, Qt::white));
		m_vcFocusLine.push_back(FocusLineInfo(line4, Qt::white));

		CSVGLineF line5(CSVGPointF(pt1.x - 1, pt1.y - 1), CSVGPointF(pt3.x + 1, pt3.y - 1));
		CSVGLineF line6(CSVGPointF(pt3.x + 1, pt3.y - 1), CSVGPointF(pt5.x + 1, pt5.y + 1));
		CSVGLineF line7(CSVGPointF(pt5.x + 1, pt5.y + 1), CSVGPointF(pt7.x - 1, pt7.y + 1));
		CSVGLineF line8(CSVGPointF(pt1.x - 1, pt1.y - 1), CSVGPointF(pt7.x - 1, pt7.y + 1));
		m_vcFocusLine.push_back(FocusLineInfo(line5, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line6, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line7, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line8, Qt::blue));

		if (nIndex >= 0)
		{
			CSVGRectF leftbox = pTextElement->GetLeftTextBox(nIndex);
			CSVGPointF ptf1(rc.x + leftbox.width, rc.y - 3);
			CSVGPointF ptf2(rc.x + leftbox.width, rc.y + rc.height + 3);
			ptf1 = ptf1.matrixTransform(usedmat);
			ptf2 = ptf2.matrixTransform(usedmat);
			CSVGLineF lineFocus(ptf1, ptf2);
			m_FlashLine = new FlashLineInfo;
			m_FlashLine->m_FocusLine1 = lineFocus;
			m_FlashLine->m_FocusLine2 = CSVGLineF(CSVGPointF(ptf1.x + 1, ptf1.y), CSVGPointF(ptf2.x + 1, ptf2.y));
		}
	}
	else if (m_pElement->getNodeType() == SVG_G
		|| m_pElement->getNodeType() == SVG_USE)
	{
		CSVGElementShape *pShape = (CSVGElementShape*)m_pElement;
		CSVGRectF rc = pShape->getBBox();
		float fx = rc.x;
		float fy = rc.y;
		float fwidth = rc.width;
		float fheight = rc.height;
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt2(fx + fwidth / 2, fy);
		CSVGPointF pt3(fx + fwidth, fy);
		CSVGPointF pt4(fx + fwidth, fy + fheight / 2);
		CSVGPointF pt5(fx + fwidth, fy + fheight);
		CSVGPointF pt6(fx + fwidth / 2, fy + fheight);
		CSVGPointF pt7(fx, fy + fheight);
		CSVGPointF pt8(fx, fy + fheight / 2);
		pt1 = pt1.matrixTransform(usedmat);
		pt2 = pt2.matrixTransform(usedmat);
		pt3 = pt3.matrixTransform(usedmat);
		pt4 = pt4.matrixTransform(usedmat);
		pt5 = pt5.matrixTransform(usedmat);
		pt6 = pt6.matrixTransform(usedmat);
		pt7 = pt7.matrixTransform(usedmat);
		pt8 = pt8.matrixTransform(usedmat);

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt1, 10000, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt2, 10001, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt3, 10002, -1, m_pElement, m_clr));

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt4, 10003, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt5, 10004, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt6, 10005, -1, m_pElement, m_clr));

		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt7, 10006, -1, m_pElement, m_clr));
		m_vcFocusKeyPoints.push_back(FocusPointInfo(pt8, 10007, -1, m_pElement, m_clr));

		CSVGLineF line1(pt1, pt3);
		CSVGLineF line2(pt3, pt5);
		CSVGLineF line3(pt5, pt7);
		CSVGLineF line4(pt1, pt7);
		m_vcFocusLine.push_back(FocusLineInfo(line1, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line2, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line3, Qt::blue));
		m_vcFocusLine.push_back(FocusLineInfo(line4, Qt::blue));
	}
}

QVector<FocusPointInfo> CFocusForElement::GetFocusPoints()
{
	return m_vcFocusKeyPoints;
}

QVector<FocusLineInfo> CFocusForElement::GetFocusLines()
{
	return m_vcFocusLine;
}

FlashLineInfo * CFocusForElement::GetFlashFocus()
{
	return m_FlashLine;
}
