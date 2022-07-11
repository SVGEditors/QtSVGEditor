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
#include "SVGElementEllipse.h"
#include "SVGDocument.h"

CSVGElementEllipse::CSVGElementEllipse(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"ellipse", SVG_ELLIPSE)
{
	cx = 0;
	cy = 0;
	rx = 0;
	ry = 0;
}


CSVGElementEllipse::~CSVGElementEllipse()
{
}

void CSVGElementEllipse::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementEllipse::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"cx")
	{
		cx = global.atof_3(value);
	}
	else if (attrname == L"cy")
	{
		cy = global.atof_3(value);
	}
	else if (attrname == L"rx")
	{
		rx = global.atof_3(value);
	}
	else if (attrname == L"ry")
	{
		ry = global.atof_3(value);
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

void CSVGElementEllipse::calBBox()
{
	m_bbox.x = cx - rx;
	m_bbox.y = cy - ry;
	m_bbox.width = 2 * rx;
	m_bbox.height = 2 * ry;
}

void CSVGElementEllipse::resetPath()
{
	m_Path = QPainterPath();
	m_Path.addEllipse(cx - rx, cy - ry, 2 * rx, 2 * ry);
	CSVGElementShape::resetPath();
}

bool CSVGElementEllipse::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());

	float fa = rx + fwidth;
	float fb = ry + fwidth;
	if (fabs(fa) <= 0.0001)
		fa = (float)0.0001;
	if (fabs(fb) <= 0.0001)
		fb = (float)0.0001;
	float val = (cx - pt.x)*(cx - pt.x) / (fa*fa)
		+ (cy - pt.y)*(cy - pt.y) / (fb*fb);
	if (val <= 1)
	{
		retshapeid = getAttributeId();
		return true;
	}
	return false;
}
