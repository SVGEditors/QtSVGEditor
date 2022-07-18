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
#include "SVGElementG.h"
#include "SVGDocument.h"
#include <QFontMetrics>
#include <QDateTime>
#include "SVGElementPolygon.h"
#include "SVGElementText.h"
#include "SVGElementPolyline.h"

CSVGElementG::CSVGElementG(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"g", SVG_G)
{

}

CSVGElementG::CSVGElementG(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype)
	: CSVGElementShape(ownerDoc, tagname, nodetype)
{

}

CSVGElementG::~CSVGElementG()
{

}

bool CSVGElementG::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	for (int i = (int)m_ChildNodes.size() - 1; i >= 0; --i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			if (pShape->IsPtInShape(fx, fy, retgroupid, retshapeid))
			{
				retgroupid = this->getAttributeId();
				return true;
			}
		}
	}
	return  false;
}

bool CSVGElementG::IsShapeInRect(CSVGRectF rectf)
{
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			if (pShape->IsShapeInRect(rectf))
			{
				return true;
			}
		}
	}
	return false;
}

void  CSVGElementG::resetPath()
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			((CSVGElementShape*)m_ChildNodes[i])->resetPath();
		}
	}
	CSVGElementShape::resetPath();
}

bool  CSVGElementG::Toxml(std::wstring &strxml, int nLevel)
{
	CSVGElement::Toxml(strxml, nLevel);
	return true;
}
