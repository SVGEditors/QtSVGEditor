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
#include "SVGElementTspan.h"
#include "SVGElementTextContent.h"
#include "SVGDocument.h"


CSVGElementTspan::CSVGElementTspan(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"tspan", SVG_TSPAN)
{
	fx = 0;
	fy = 0;
	fdx = 0;
	fdy = 0;
	m_bShow = false;
}

CSVGElementTspan::~CSVGElementTspan()
{
}

void CSVGElementTspan::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (addorsetAttribute(attrname, value, bSetModfiyFlag))
	{
		CSVGElement::setAttribute(attrname, value, bSetModfiyFlag);
	}
	else
	{
		CSVGElementStyleable::setAttribute(attrname, value, bSetModfiyFlag);
	}
}

void  CSVGElementTspan::setNodeValue(const std::wstring &value, bool bSetModfiyFlag)
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		delete m_ChildNodes[i];
	}
	m_ChildNodes.clear();

	CSVGElement *pElement = m_pownDocument->createElement(L"textcontent");
	addChild(pElement, false, bSetModfiyFlag);
	pElement->setNodeValue(value, bSetModfiyFlag);
}

bool CSVGElementTspan::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x")
	{
		x = value;
	}
	else if (attrname == L"y")
	{
		y = value;
	}
	else if (attrname == L"dx")
	{
		dx = value;
	}
	else if (attrname == L"dy")
	{
		dy = value;
	}
	else
	{
		return false;
	}
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
	return true;
}

void CSVGElementTspan::resetTextPath(float nx, float ny)
{
	ResetStyleUsed();
	m_Path = QPainterPath();
	if (x.length() > 0)
	{
		fx = global.atof_3(x);
	}
	else
	{
		fx = nx;
	}
	if (y.length() > 0)
	{
		fy = global.atof_3(y);
	}
	else
	{
		fy = ny;
	}
	if (dx.length() > 0)
		fdx = global.atof_3(dx);
	if (dy.length() > 0)
		fdy = global.atof_3(dy);
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		CSVGElement *pElement = m_ChildNodes[i];
		if (pElement->getNodeType() == SVG_TEXTCONTENT)
		{
			CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)pElement;
			pTextContent->resetTextPath(fx + fdx, fy + fdy);
			m_Path.addPath(pTextContent->m_Path);
		}
	}
	calBBox();
}

void  CSVGElementTspan::calBBox()
{
	m_bbox.Empty();
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->getNodeType() == SVG_TEXTCONTENT)
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			CSVGRectF childRect = pShape->getBBox();

			if (childRect.IsEmpty())
			{
				continue;
			}

			if (m_bbox.IsEmpty())
			{
				m_bbox = childRect;
			}
			else
			{
				CSVGPointF rectRb(m_bbox.x + m_bbox.width, m_bbox.y + m_bbox.height);
				CSVGPointF childRb(childRect.x + childRect.width, childRect.y + childRect.height);
				float minx = SVGmin(SVGmin(SVGmin(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float miny = SVGmin(SVGmin(SVGmin(m_bbox.y, childRect.y), rectRb.y), childRb.y);
				float maxx = SVGmax(SVGmax(SVGmax(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float maxy = SVGmax(SVGmax(SVGmax(m_bbox.y, childRect.y), rectRb.y), childRb.y);

				m_bbox.x = minx;
				m_bbox.y = miny;
				m_bbox.width = maxx - minx;
				m_bbox.height = maxy - miny;
			}
		}
	}
}
