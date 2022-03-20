
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
#include "SVGElementCircle.h"
#include "SVGDocument.h"

CSVGElementCircle::CSVGElementCircle(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"circle", SVG_CIRCLE)
{
	cx = 0;
	cy = 0;
	r = 0;
}

CSVGElementCircle::~CSVGElementCircle()
{
}

void CSVGElementCircle::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementCircle::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"cx")
	{
		cx = global.atof_3(value);
	}
	else if (attrname == L"cy")
	{
		cy = global.atof_3(value);
	}
	else if (attrname == L"r")
	{
		r = global.atof_3(value);
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

std::set<std::wstring> CSVGElementCircle::getAttrNames()
{
	std::set<std::wstring>vcName = CSVGElementShape::getAttrNames();
	vcName.insert(vcName.begin(), L"r");
	vcName.insert(vcName.begin(), L"cy");
	vcName.insert(vcName.begin(), L"cx");
	return vcName;
}

void CSVGElementCircle::calBBox()
{
	m_bbox.x = cx - r;
	m_bbox.y = cy - r;
	m_bbox.width = 2 * r;
	m_bbox.height = 2 * r;
}

void CSVGElementCircle::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		m_Path.addEllipse(cx - r, cy - r, 2 * r, 2 * r);
		CSVGElementShape::resetPath();
	}
}

bool CSVGElementCircle::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());

	float fb = r + fwidth;
	float fa = r + fwidth;
	if (fabs(fb) <= 0.0001)
		fb = (float)0.0001;
	if (fabs(fa) <= 0.0001)
		fa = (float)0.0001;
	float val = (cx - pt.x)*(cx - pt.x) / (fa*fa)
		+ (cy - pt.y)*(cy - pt.y) / (fb*fb);
	if (val <= 1)
	{
		retshapeid = getAttributeId();
		return true;
	}
	return false;
}


int CSVGElementCircle::HitTest(int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGPointF pt((float)nx, (float)ny);
	CSVGMatrix matrix = *screenmat;
	matrix.Add(m_usedmatrix);
	matrix.Inverse();
	pt = pt.matrixTransform(matrix);
	int x = m_bbox.x;
	int y = m_bbox.y;
	int width = m_bbox.width;
	int height = m_bbox.height;

	if ((std::abs(x - pt.x) < HITTEST_SIZE) && (std::abs(y - pt.y) < HITTEST_SIZE))
	{
		return LEFT_TOP;
	}
	else if ((std::abs(x + width - pt.x) < HITTEST_SIZE) && (std::abs(y - pt.y) < HITTEST_SIZE))
	{
		return RIGHT_TOP;
	}
	else if ((std::abs(x - pt.x) < HITTEST_SIZE) && (std::abs(y + height - pt.y) < HITTEST_SIZE))
	{
		return LEFT_BOTTOM;
	}
	else if ((std::abs(x + width - pt.x) < HITTEST_SIZE) && (std::abs(y + height - pt.y) < HITTEST_SIZE))
	{
		return RIGHT_BOTTOM;
	}

	return -1;
}
