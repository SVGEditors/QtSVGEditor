
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
#include "SVGElementClipPath.h"


CSVGElementClipPath::CSVGElementClipPath(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"clipPath", SVG_CLIPPATH)
{
	m_bShow = false;
	m_bInit = false;
}


CSVGElementClipPath::~CSVGElementClipPath()
{
}

void  CSVGElementClipPath::resetPath()
{
	if (!m_bInit)
	{
		m_bInit = true;
		m_Path = QPainterPath();
		for (size_t i = 0; i < m_ChildNodes.size(); ++i)
		{
			CSVGElement *pElement = (CSVGElement*)m_ChildNodes[i];
			if (pElement->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)pElement;
				pShape->resetPath();
				m_Path.addPath(pShape->m_Path);
			}
		}
		CSVGElementShape::resetPath();
	}
}
