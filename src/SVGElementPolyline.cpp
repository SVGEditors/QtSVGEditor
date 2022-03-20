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
#include "SVGElementPolyline.h"
#include "SVGDocument.h"


CSVGElementPolyline::CSVGElementPolyline(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"polyline", SVG_POLYLINE)
{
}

CSVGElementPolyline::CSVGElementPolyline(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype)
	: CSVGElementShape(ownerDoc, tagname, nodetype)
{
}

CSVGElementPolyline::~CSVGElementPolyline()
{
}

bool CSVGElementPolyline::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	float fval = global.atof_3(GetUseStyle()[ATTR_FILL_OPACITY].c_str());
	bool bret = false;
	if (fval > 0.1)
	{
		bret = global.PtInPolygonEx(m_items, pt);
		if (bret)
		{
			retshapeid = getAttributeId();
		}
		return bret;
	}
	else
	{
		int i = 0;
		CSVGPointF prept(0, 0);
		for (std::list<CSVGPointF>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
		{
			if (i != 0)
			{
				float halfpenwidth = fwidth;
				CSVGPointF pt1, pt2, pt3, pt4;
				global.GetparallelLine(prept, *iter, pt1, pt2, -2 - halfpenwidth);
				global.GetparallelLine(prept, *iter, pt3, pt4, 2 + halfpenwidth);
				std::vector<CSVGPointF>vcpt;
				vcpt.push_back(pt1);
				vcpt.push_back(pt2);
				vcpt.push_back(pt4);
				vcpt.push_back(pt3);
				if (global.PtInPolygonEx(vcpt, pt))
				{
					retshapeid = getAttributeId();
					bret = true;
					break;
				}
			}
			prept.x = iter->x;
			prept.y = iter->y;
			++i;
		}
		return bret;
	}
	return false;
}

bool  CSVGElementPolyline::IsShapeInRect(const CSVGRectF& rectf)
{
	CSVGMatrix mat = m_usedmatrix;
	for (std::list<CSVGPointF>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
	{
		CSVGPointF pt(*iter);
		pt = pt.matrixTransform(mat);
		if (rectf.PtInRect(pt))
		{
			return true;
		}
	}
	return false;
}

void CSVGElementPolyline::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

std::list<CSVGPointF> CSVGElementPolyline::LoadFrompoints(const std::wstring& value)
{
	std::list<CSVGPointF> items;
	size_t sz = value.length();
	if (sz > 2)
	{
		wchar_t* pBuf = new wchar_t[sz + 1];
		const wchar_t* sep = L", ";
		std::swprintf(pBuf, sz + 1, L"%s", value.c_str());
		float dx = 0.0;
		float dy = 0.0;
		wchar_t *pnext = NULL;
		wchar_t* token = std::wcstok(pBuf, sep, &pnext);
		while (token)
		{
			dx = global.atof_3(token);
			token = std::wcstok(NULL, sep, &pnext);
			if (token)
			{
				dy = global.atof_3(token);
				items.push_back(CSVGPointF(dx, dy));
				token = std::wcstok(NULL, sep, &pnext);
			}
		}
		delete[] pBuf;
	}
	return items;
}

bool  CSVGElementPolyline::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"points")
	{
		m_items.clear();
		QString strValue = QString::fromStdWString(value);
		strValue.replace("&#xD;", "");//n
		strValue.replace("&#xA;", "");//r
		strValue.replace("&#x9;", "");//Tab 
		strValue.replace("&#20;", "");//space 
		m_items = LoadFrompoints(strValue.toStdWString());
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

void  CSVGElementPolyline::CSVGElementPolyline::calBBox()
{
	m_bbox.Empty();
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	std::list<CSVGPointF>::iterator iter = m_items.begin();
	for (int i = 0; iter != m_items.end(); ++iter, ++i)
	{
		if (i == 0)
		{
			x1 = iter->x;
			y1 = iter->y;
			x2 = iter->x;
			y2 = iter->y;
		}
		else
		{
			x1 = SVGmin(x1, iter->x);
			y1 = SVGmin(y1, iter->y);
			x2 = SVGmax(x2, iter->x);
			y2 = SVGmax(y2, iter->y);
		}
	}
	m_bbox.x = x1;
	m_bbox.y = y1;
	m_bbox.width = x2 - x1;
	m_bbox.height = y2 - y1;
}

void  CSVGElementPolyline::CSVGElementPolyline::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		if (m_items.size() > 0)
		{
			QPolygonF ptf;
			std::list<CSVGPointF>::iterator iter = m_items.begin();
			for (int i = 0; iter != m_items.end(); ++iter, ++i)
			{
				ptf.append(QPointF(iter->x, iter->y));
			}
			m_Path.addPolygon(ptf);
		}
		CSVGElementShape::resetPath();
	}
}

int   CSVGElementPolyline::HitTest(int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGPointF pt((float)nx, (float)ny);
	CSVGMatrix matrix = *screenmat;
	matrix.Add(m_usedmatrix);
	matrix.Inverse();
	pt = pt.matrixTransform(matrix);
	std::list<CSVGPointF>::iterator iter = m_items.begin();
	for (int nIndex = 0; iter != m_items.end(); ++iter, ++nIndex)
	{
		if ((std::abs(iter->x - pt.x) < 3) && (std::abs(iter->y - pt.y) < 3))
		{
			return nIndex;
		}
	}
	return -1;
}

std::list<CSVGPointF>& CSVGElementPolyline::getPoints()
{
	return m_items;
}

std::wstring CSVGElementPolyline::PointsToValue(std::list<CSVGPointF>& pts)
{
	std::wstring str;
	std::list<CSVGPointF>::iterator iter = pts.begin();
	for (; iter != pts.end(); ++iter)
	{
		if (str.length() > 0)
		{
			str += L" ";
		}
		str += global.ftostring(iter->x);
		str += L" ";
		str += global.ftostring(iter->y);
	}
	return str;
}
