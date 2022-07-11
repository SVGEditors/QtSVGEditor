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
#include "SVGElementUse.h"
#include "SVGDocument.h"

CSVGElementUse::CSVGElementUse(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"use", SVG_USE)
{
	x = 0;
	y = 0;
}

CSVGElementUse::~CSVGElementUse()
{

}

void CSVGElementUse::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementUse::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else if (attrname == L"xlink:href")
	{
		xlinkhref = value;
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

void CSVGElementUse::calBBox()
{
	m_bbox.Empty();
	std::wstring id = global.UrlStringGetId(xlinkhref.c_str());
	CSVGElement *pElement = m_pownDocument->getElementById(id.c_str());
	if (pElement && pElement != this && pElement->IsShapeElement())
	{
		CSVGElementShape* pShape = (CSVGElementShape*)pElement;
		m_bbox = pShape->getBBox();
	}
	CSVGMatrix mat;
	mat.Translate(x, y);
	m_bbox = m_bbox.matrixTransform(mat);
}

bool CSVGElementUse::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	CSVGRectF rectf = m_bbox.Inflate(fwidth, fwidth);
	bool bret = rectf.PtInRect(pt);
	if (bret)
	{
		retshapeid = getAttributeId();
	}
	return bret;
}


void CSVGElementUse::resetPath()
{
	if (xlinkhref.length() > 0)
	{
		std::wstring id = global.UrlStringGetId(xlinkhref.c_str());
		CSVGElement *pElement = m_pownDocument->getElementById(id.c_str());
		if (pElement && pElement != this && pElement->IsShapeElement())
		{
			CSVGElementShape* pShape = (CSVGElementShape*)pElement;
			pShape->resetUseMatrix(this);
			pShape->ResetStyleUsed(this);
		}
	}
	CSVGElementShape::resetPath();
}
