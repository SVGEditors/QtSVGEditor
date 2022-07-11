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
#include "SVGElementLine.h"
#include "SVGDocument.h"

CSVGElementLine::CSVGElementLine(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"line", SVG_LINE)
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
}

CSVGElementLine::~CSVGElementLine()
{
}

void CSVGElementLine::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementLine::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x1")
	{
		x1 = global.atof_3(value);
	}
	else if (attrname == L"y1")
	{
		y1 = global.atof_3(value);
	}
	else if (attrname == L"x2")
	{
		x2 = global.atof_3(value);
	}
	else if (attrname == L"y2")
	{
		y2 = global.atof_3(value);
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

std::set<std::wstring> CSVGElementLine::getAttrNames()
{
	std::set<std::wstring>vcName = CSVGElementShape::getAttrNames();
	vcName.insert(vcName.begin(), L"y2");
	vcName.insert(vcName.begin(), L"x2");
	vcName.insert(vcName.begin(), L"y1");
	vcName.insert(vcName.begin(), L"x1");
	return vcName;
}

void CSVGElementLine::calBBox()
{
	m_bbox.x = SVGmin(x1, x2);
	m_bbox.y = SVGmin(y1, y2);
	m_bbox.width = SVGmax(x1, x2) - SVGmin(x1, x2);
	m_bbox.height = SVGmax(y1, y2) - SVGmin(y1, y2);
}

void CSVGElementLine::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		m_Path.moveTo(x1, y1);
		m_Path.lineTo(x2, y2);
		CSVGElementShape::resetPath();
	}
}

bool CSVGElementLine::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	CSVGPointF pt1, pt2, pt3, pt4;
	global.GetparallelLine(CSVGPointF(x1, y1), CSVGPointF(x2, y2), pt1, pt2, -2 - fwidth);
	global.GetparallelLine(CSVGPointF(x1, y1), CSVGPointF(x2, y2), pt3, pt4, 2 + fwidth);
	std::vector<CSVGPointF>vcpt;
	vcpt.push_back(pt1);
	vcpt.push_back(pt2);
	vcpt.push_back(pt4);
	vcpt.push_back(pt3);
	bool bRet = global.PtInPolygonEx(vcpt, pt);
	if (bRet)
	{
		retshapeid = getAttributeId();
	}
	return bRet;
}

bool  CSVGElementLine::IsShapeInRect(CSVGRectF rectf)
{
	CSVGMatrix mat = m_usedmatrix;

	CSVGPointF pt1(x1, y1);
	CSVGPointF pt2(x2, y2);
	pt1 = pt1.matrixTransform(mat);
	pt2 = pt2.matrixTransform(mat);
	if (rectf.PtInRect(pt1) || rectf.PtInRect(pt2))
	{
		return true;
	}
	return false;
}

void  CSVGElementLine::resetpatternPath(int nNewWidth, int nNewHeight)
{
	if (m_bPathHasChanged)
	{
		calBBox();
		m_bPathHasChanged = false;

		float scalex = nNewWidth * 1.0 / m_bbox.width + 0.00001;
		float scaley = nNewHeight * 1.0 / m_bbox.height + 0.00001;

		m_bbox.x *= scalex;
		m_bbox.y *= scaley;
		m_bbox.width *= scalex;
		m_bbox.height *= scaley;

		m_Path = QPainterPath();
		m_Path.moveTo(x1*scalex, y1*scalex);
		m_Path.lineTo(x2*scalex, y2*scalex);
	}
}

int CSVGElementLine::HitTest(int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGPointF pt((float)nx, (float)ny);
	CSVGMatrix matrix = *screenmat;
	matrix.Add(m_usedmatrix);
	matrix.Inverse();
	pt = pt.matrixTransform(matrix);
	if ((abs(x1 - pt.x) < HITTEST_SIZE) && (abs(y1 - pt.y) < HITTEST_SIZE))
	{
		return 0;
	}
	else if ((abs(x2 - pt.x) < HITTEST_SIZE) && (abs(y2 - pt.y) < HITTEST_SIZE))
	{
		return 1;
	}
	return -1;
}
