/**
*
* @license MIT
*
* @copyright: 2022 LinJi
*
* @technical support: www.svgsvg.cn
*
* @email: 93681992@qq.com
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
#include "SVGElementRect.h"
#include "SVGDocument.h"
#include "SVGGlobal.h"

CSVGElementRect::CSVGElementRect(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"rect", SVG_RECT)
{
	x = 0;
	y = 0;
	rx = 0;
	ry = 0;
	width = 0;
	height = 0;
}

CSVGElementRect::~CSVGElementRect()
{
}

void CSVGElementRect::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementRect::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else if (attrname == L"rx")
	{
		rx = global.atof_3(value);
	}
	else if (attrname == L"ry")
	{
		ry = global.atof_3(value);
	}
	else if (attrname == L"width")
	{
		width = global.atof_3(value);
	}
	else if (attrname == L"height")
	{
		height = global.atof_3(value);
	}
	else
	{
		return false;
	}
	if (bSetModfiyFlag)
	{
		CSVGElement::pathHasChange();
	}
	return true;
}

std::set<std::wstring> CSVGElementRect::getAttrNames()
{
	std::set<std::wstring>vcName = CSVGElementShape::getAttrNames();
	vcName.insert(vcName.begin(), L"height");
	vcName.insert(vcName.begin(), L"width");
	vcName.insert(vcName.begin(), L"y");
	vcName.insert(vcName.begin(), L"x");
	vcName.insert(vcName.begin(), L"rx");
	vcName.insert(vcName.begin(), L"ry");
	return vcName;
}

void  CSVGElementRect::calBBox()
{
	m_bbox.x = x;
	m_bbox.y = y;
	m_bbox.width = width;
	m_bbox.height = height;
}

void CSVGElementRect::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		if (rx > 0 || ry > 0)
		{
			m_Path.addRoundedRect(x, y, width, height, rx, ry);
		}
		else
		{
			m_Path.addRect(x, y, width, height);
		}
		CSVGElementShape::resetPath();
	}
}

void  CSVGElementRect::resetpatternPath(int nNewWidth, int nNewHeight)
{
	if (m_bPathHasChanged)
	{
		calBBox();
		m_bPathHasChanged = false;
		float scalex = nNewWidth*1.0 / m_bbox.width + 0.00001;
		float scaley = nNewHeight*1.0 / m_bbox.height + 0.00001;

		m_bbox.x *= scalex;
		m_bbox.y *= scaley;
		m_bbox.width *= scalex;
		m_bbox.height *= scaley;

		m_Path = QPainterPath();
		if (rx > 0 || ry > 0)
		{
			m_Path.addRoundedRect(x*scalex, y*scalex, width*scalex, height*scalex, rx*scalex, ry*scalex);
		}
		else
		{
			m_Path.addRect(x*scalex, y*scalex, width*scalex, height*scalex);
		}
	}
}

bool CSVGElementRect::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	float fx1 = x;
	float fy1 = y;
	float fx2 = x + width;
	float fy2 = y + height;

	CSVGRectF rectf(std::min(fx1, fx2), std::min(fy1, fy2), std::fabs(width), std::fabs(height));
	rectf = rectf.Inflate(fwidth, fwidth);
	bool bret = rectf.PtInRect(pt);
	if (bret)
	{
		retshapeid = getAttributeId();
	}
	return bret;
}
