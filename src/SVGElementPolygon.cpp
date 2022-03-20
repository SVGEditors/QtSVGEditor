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
#include "SVGElementPolygon.h"


CSVGElementPolygon::CSVGElementPolygon(CSVGDocument* ownerDoc)
	:CSVGElementPolyline(ownerDoc, L"polygon", SVG_POLYGON)
{
}

CSVGElementPolygon::~CSVGElementPolygon()
{
}

bool CSVGElementPolygon::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	bool bRet = global.PtInPolygonEx(m_items, pt);
	if (bRet)
	{
		retshapeid = getAttributeId();
	}
	return bRet;
}

void CSVGElementPolygon::resetPath()
{
	if (m_bPathHasChanged)
	{
		CSVGElementPolyline::resetPath();
		if (m_items.size() >= 3)
		{
			m_Path.closeSubpath();
		}
		CSVGElementShape::resetPath();
	}
}

bool  CSVGElementPolygon::isValidShape()
{
	if (m_items.size() < 3)
		return false;
	return true;
}
