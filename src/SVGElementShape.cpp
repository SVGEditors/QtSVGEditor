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
#include "SVGElementShape.h"
#include "SVGElementStyleClass.h"
#include "SVGDocument.h"
#include "SVGGlobal.h"

#include "SVGElementLinearGradient.h"
#include "SVGElementRadialGradient.h"
#include "SVGElementImage.h"
#include "SVGElementClipPath.h"
#include "SVGElementPattern.h"


#define ROTATEHANDLE_HEIGHT 30

CSVGElementShape::CSVGElementShape(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype)
	:CSVGElementStyleable(ownerDoc, tagname, nodetype)
{
	m_bbox.Empty();
}

CSVGElementShape::~CSVGElementShape()
{

}

CSVGRectF CSVGElementShape::getBBox()
{
	resetPath();
	CSVGRectF bbox = m_bbox;
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	bbox = bbox.Inflate(fwidth, fwidth);
	return bbox;
}


std::wstring  CSVGElementShape::getUseStyle(const std::wstring &attrname)
{
	return GetUseStyle()[global.getIndexByAttrName(attrname)].c_str();
}

void CSVGElementShape::resetPath()
{
	calBBox();
	m_bPathHasChanged = false;
}

void  CSVGElementShape::calBBox()
{
	if (m_ChildNodes.size() == 0)
		return;
	m_bbox.Empty();
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			CSVGRectF childRect = pShape->getBBox();

			CSVGMatrix pMat = pShape->getUsedMatrix();
			CSVGMatrix mat = m_usedmatrix;
			mat.Inverse();
			mat.Add(pMat);

			if (!childRect.IsEmpty())
			{
				childRect = childRect.matrixTransform(mat);
			}
			else
			{
				continue;
			}

			if (m_bbox.IsEmpty())
			{
				m_bbox = childRect;
			}
			else
			{
				CSVGPointF rectRb(m_bbox.x + m_bbox.width, m_bbox.y + m_bbox.height);
				CSVGPointF childRb(childRect.x + childRect.width, childRect.y + childRect.height);
				float minx = std::min(std::min(std::min(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float miny = std::min(std::min(std::min(m_bbox.y, childRect.y), rectRb.y), childRb.y);
				float maxx = std::max(std::max(std::max(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float maxy = std::max(std::max(std::max(m_bbox.y, childRect.y), rectRb.y), childRb.y);

				m_bbox.x = minx;
				m_bbox.y = miny;
				m_bbox.width = maxx - minx;
				m_bbox.height = maxy - miny;
			}
		}
	}
}

void CSVGElementShape::DoClipPath(QPainter *painter, const CSVGMatrix &clipmat)
{
	std::wstring strstyle = GetSelfStyle()[ATTR_CLIP_PATH];
	if (strstyle.length() > 0)
	{
		CSVGMatrix mat = clipmat;
		mat.Add(m_usedmatrix);
		QMatrix areamat(mat.m_a, mat.m_b, mat.m_c, mat.m_d, mat.m_e, mat.m_f);
		painter->setMatrix(areamat);
		std::wstring id = global.UrlStringGetId(strstyle);
		CSVGElement *pElement = m_pownDocument->getElementById(id.c_str());
		if (pElement && pElement->getNodeType() == SVG_CLIPPATH)
		{
			CSVGElementClipPath *pPath = (CSVGElementClipPath*)pElement;
			pPath->resetPath();
			painter->setClipPath(pPath->m_Path, Qt::IntersectClip);
		}
	}
	if (m_parentNode && m_parentNode->getNodeType() == SVG_G)
	{
		((CSVGElementShape*)m_parentNode)->DoClipPath(painter, clipmat);
	}
}

void CSVGElementShape::Draw(QPainter *painter, const CSVGMatrix *screenmat)
{
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setRenderHint(QPainter::TextAntialiasing, true);
	setAttrChange(false);

	if (GetUseStyle()[ATTR_TYVISIBILITY] == L"visible")
	{

	}
	else if (GetUseStyle()[ATTR_VISIBILITY] == L"hidden" || GetUseStyle()[ATTR_DISPLAY] == L"none")
	{
		return;
	}

	if (m_nodeType == SVG_G
		|| m_nodeType == SVG_SVG
		|| m_nodeType == SVG_A
		|| m_nodeType == SVG_SYMBOL
		|| m_nodeType == SVG_SWITCH
		|| m_nodeType == SVG_TEXT)
	{
		for (size_t i = 0; i < m_ChildNodes.size(); ++i)
		{
			if (m_ChildNodes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = ((CSVGElementShape*)m_ChildNodes[i]);
				pShape->Draw(painter, screenmat);
			}
		}
		return;
	}
	if (m_nodeType == SVG_USE)
	{
		std::wstring xlinkref = getAttribute(L"xlink:href");
		std::wstring id = global.UrlStringGetId(xlinkref);
		CSVGElement *pElement = m_pownDocument->getElementById(id.c_str());
		if (pElement && pElement != this && pElement->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)pElement;
			pShape->Draw(painter, screenmat);
		}
	}
	if (GetUseStyle()[ATTR_CLIP_PATH].length() > 0)
	{
		painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
		DoClipPath(painter, *screenmat);
	}

	CSVGMatrix mat = *screenmat;
	mat.Add(m_usedmatrix);
	QMatrix areamat(mat.m_a, mat.m_b, mat.m_c, mat.m_d, mat.m_e, mat.m_f);
	painter->setMatrix(areamat);

	if (HasSingleBrush())
	{
		QPainterPath paintpath = GetPainterPath();
		painter->fillPath(paintpath, GetSingleBrush());
	}
	else
	{
		std::wstring id = global.UrlStringGetId(GetUseStyle()[ATTR_FILL].c_str());
		CSVGElement *pElement = m_pownDocument->getElementById(id.c_str());
		if (pElement)
		{
			painter->setRenderHint(QPainter::SmoothPixmapTransform);
			painter->setBrush(Qt::NoBrush);
			if (pElement->getNodeType() == SVG_LINEARGRADIENT)
			{
				CSVGElementLinearGradient *pLinear = (CSVGElementLinearGradient*)pElement;
				CSVGRectF box = getBBox();
				QBrush brush = pLinear->getBrush(box, global.atof_3(GetUseStyle()[ATTR_FILL_OPACITY].c_str()));
				painter->fillPath(GetPainterPath(), brush);
			}
			else if (pElement->getNodeType() == SVG_RADIALGRADIENT)
			{
				CSVGElementRadialGradient *pRadial = (CSVGElementRadialGradient*)pElement;
				QBrush brush = pRadial->getBrush(getBBox(), global.atof_3(GetUseStyle()[ATTR_FILL_OPACITY].c_str()));
				painter->fillPath(GetPainterPath(), brush);
			}
			else if (pElement->getNodeType() == SVG_PATTERN)
			{
				CSVGElementPattern *pPattern = (CSVGElementPattern*)pElement;
				QBrush brush = pPattern->getBrush(getBBox());
				painter->fillPath(GetPainterPath(), brush);
			}
		}
	}
	if (getNodeType() == SVG_IMAGE)
	{
		CSVGElementImage *pImage = (CSVGElementImage*)this;
		QRectF rect(pImage->x, pImage->y, pImage->getBBox().width, pImage->getBBox().height);
		painter->drawImage(rect, pImage->m_Image);
	}
	painter->setPen(GetPen());
	painter->drawPath(GetPainterPath());
}

void CSVGElementShape::resetpatternPath(int nNewWidth, int nNewHeight)
{
	Q_UNUSED(nNewWidth);
	Q_UNUSED(nNewHeight);
}

CSVGRectF  CSVGElementShape::getMatrixBBox()
{
	CSVGMatrix mat = m_usedmatrix;
	CSVGRectF bbox = getBBox();
	CSVGRectF rectf = bbox.matrixTransform(mat);
	return rectf;
}

bool CSVGElementShape::IsShapeInRect(const CSVGRectF &rectf)
{
	if (this == m_pownDocument->GetRootSVGElement())
		return false;

	CSVGMatrix mat = m_usedmatrix;
	CSVGRectF bbox = m_bbox;
	bbox = bbox.matrixTransform(mat);
	if (rectf.PtInRect(CSVGPointF(bbox.x, bbox.y))
		|| rectf.PtInRect(CSVGPointF(bbox.x, bbox.y + bbox.height))
		|| rectf.PtInRect(CSVGPointF(bbox.x + bbox.width, bbox.y))
		|| rectf.PtInRect(CSVGPointF(bbox.x + bbox.width, bbox.y + bbox.height))
		)
	{
		return true;
	}
	return false;
}

int CSVGElementShape::HitTest(int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGRectF focusbbox = getBBox();

	CSVGPointF pt((float)nx, (float)ny);
	CSVGMatrix matrix = *screenmat;
	matrix.Add(m_usedmatrix);
	matrix.Inverse();
	pt = pt.matrixTransform(matrix);
	int x = focusbbox.x;
	int y = focusbbox.y;
	int width = focusbbox.width;
	int height = focusbbox.height;

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

	if ((std::abs(x - pt.x) < HITTEST_SIZE) && (std::abs(y + height / 2 - pt.y) < HITTEST_SIZE))
	{
		return LEFT_MID;
	}
	else if ((std::abs(x + width / 2 - pt.x) < HITTEST_SIZE) && (std::abs(y - pt.y) < HITTEST_SIZE))
	{
		return TOP_MID;
	}
	else if ((std::abs(x + width - pt.x) < HITTEST_SIZE) && (std::abs(y + height / 2 - pt.y) < HITTEST_SIZE))
	{
		return RIGHT_MID;
	}
	else if ((std::abs(x + width / 2 - pt.x) < HITTEST_SIZE) && (std::abs(y + height - pt.y) < HITTEST_SIZE))
	{
		return BOTTOM_MID;
	}
	else if ((std::abs(x + width / 2 - pt.x) < HITTEST_SIZE) && (std::abs(y - pt.y - ROTATEHANDLE_HEIGHT) < HITTEST_SIZE))
	{
		return ROTATE_HANDLE;
	}

	return -1;
}

void  CSVGElementShape::resetBBox()
{
	m_bbox.Empty();
}

std::set<std::wstring> CSVGElementShape::getAttrNames()
{
	std::set<std::wstring> vcName = CSVGElementStyleable::getAttrNames();
	vcName.insert(vcName.begin(), L"transform");
	return vcName;
}

QPainterPath  &CSVGElementShape::GetPainterPath()
{
	resetPath();
	return m_Path;
}
