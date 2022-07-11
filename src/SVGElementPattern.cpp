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
#include "SVGElementPattern.h"
#include "SVGElementShape.h"
#include "SVGDocument.h"

CSVGElementPattern::CSVGElementPattern(CSVGDocument* ownerDoc)
	:CSVGElementStyleable(ownerDoc, L"pattern", SVG_PATTERN)
{
	x = 0;
	y = 0;
	height = 0;
	width = 0;
	m_bShow = false;
}


CSVGElementPattern::~CSVGElementPattern()
{
}

void CSVGElementPattern::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementPattern::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else if (attrname == L"width")
	{
		width = global.atof_3(value);
	}
	else if (attrname == L"height")
	{
		height = global.atof_3(value);
	}
	else if (attrname == L"patternContentUnits")
	{
		m_patternContentUnits = value;
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

bool  CSVGElementPattern::Toxml(std::wstring &retstrxml, int nLevel)
{
	std::wstring strid = getAttribute(L"id");
	if (m_pownDocument->IsUrlIdUsedByShape(strid))
	{
		return CSVGElement::Toxml(retstrxml, nLevel);
	}
	return false;
}

QBrush CSVGElementPattern::getBrush(const CSVGRectF &bbox)
{
	int nx = x;
	int ny = y;
	int nimgwidth = width;
	int nimgheight = height;
	if (m_patternContentUnits == L"objectBoundingBox")
	{
		nx = x * bbox.x;
		ny = y * bbox.y;
		nimgwidth = width * bbox.width;
		nimgheight = height * bbox.height;
	}
	QImage Image(nimgwidth, nimgheight, QImage::Format_ARGB32);
	Image.fill(0);
	QPainter painter(&Image);
	painter.setPen(Qt::NoPen);
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		CSVGElement *pChild = (CSVGElement*)m_ChildNodes[i];
		if (pChild->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)pChild;
			if (m_patternContentUnits == L"objectBoundingBox")
			{
				pShape->pathHasChange();
				pShape->resetpatternPath(nimgwidth, nimgheight);
			}
			CSVGMatrix mat = pShape->getUsedMatrix();
			mat.Inverse();
			pShape->Draw(&painter, &mat);
		}
	}
	QBrush brush(Qt::TexturePattern);
	brush.setTextureImage(Image);
	QTransform mat;
	mat.translate(bbox.x, bbox.y);
	brush.setTransform(mat);
	return brush;
}
