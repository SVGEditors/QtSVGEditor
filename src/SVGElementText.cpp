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
#include "SVGElementText.h"
#include "SVGElementTspan.h"
#include "SVGElementTextContent.h"
#include "SVGDocument.h"

CSVGElementText::CSVGElementText(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"text", SVG_TEXT)
{
	x = 0;
	y = 0;
	m_ncursorpos = 0;
	m_nAllCharSize = 0;
	setAttribute(L"font-size", L"12pt", false);
}

CSVGElementText::~CSVGElementText()
{
}

void CSVGElementText::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (addorsetAttribute(attrname, value, bSetModfiyFlag))
	{
		CSVGElement::setAttribute(attrname, value, bSetModfiyFlag);
	}
	else
	{
		CSVGElementStyleable::setAttribute(attrname, value, bSetModfiyFlag);
	}
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
}

void  CSVGElementText::setNodeValue(const std::wstring &value, bool bSetModfiyFlag)
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		delete m_ChildNodes[i];
	}
	m_ChildNodes.clear();

	CSVGElement *pElement = m_pownDocument->createElement(L"textcontent");
	addChild(pElement, false, bSetModfiyFlag);
	pElement->setNodeValue(value, bSetModfiyFlag);
	this->setAttrChange(true);
}

std::wstring CSVGElementText::getNodeValue()
{
	std::wstring strVal;
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->getNodeType() == SVG_TEXTCONTENT)
		{
			CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)m_ChildNodes[i];
			strVal += pTextContent->getNodeValue();
		}
		else if (m_ChildNodes[i]->getNodeType() == SVG_TSPAN)
		{
			CSVGElementTspan *pTspan = (CSVGElementTspan*)m_ChildNodes[i];
			for (int j = 0; j < (int)pTspan->getChildCount(); ++j)
			{
				CSVGElement* pTSpanChild = pTspan->getChild(j);
				if (pTSpanChild->getNodeType() == SVG_TEXTCONTENT)
				{
					CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)pTSpanChild;
					strVal += pTextContent->getNodeValue();
				}
			}
		}
	}
	return strVal;
}

bool CSVGElementText::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	Q_UNUSED(bSetModfiyFlag);
	if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else
	{
		return false;
	}
	return true;
}


void  CSVGElementText::calBBox()
{
	m_bbox.Empty();
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->getNodeType() == SVG_TEXTCONTENT
			|| m_ChildNodes[i]->getNodeType() == SVG_TSPAN)
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			CSVGRectF childRect = pShape->getBBox();

			if (childRect.IsEmpty())
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
				float minx = SVGmin(SVGmin(SVGmin(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float miny = SVGmin(SVGmin(SVGmin(m_bbox.y, childRect.y), rectRb.y), childRb.y);
				float maxx = SVGmax(SVGmax(SVGmax(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float maxy = SVGmax(SVGmax(SVGmax(m_bbox.y, childRect.y), rectRb.y), childRb.y);

				m_bbox.x = minx;
				m_bbox.y = miny;
				m_bbox.width = maxx - minx;
				m_bbox.height = maxy - miny;
			}
		}
	}
	if (m_bbox.IsEmpty())
	{
		std::wstring fontName = GetUseStyle()[ATTR_FONT_FAMILY].c_str();
		if (fontName.length() == 0)
		{
			fontName = m_pownDocument->GetDefaultFontName();
		}

		int nfontpixelsize = 16;
		std::wstring fontsize = GetUseStyle()[ATTR_FONT_SIZE].c_str();
		if (fontsize.length() > 0)
		{
			QString strFontSize = QString::fromStdWString(fontsize);
			if (strFontSize.indexOf("pt", 0, Qt::CaseInsensitive) > 0)
			{
				strFontSize = strFontSize.replace("pt", "", Qt::CaseInsensitive);
				nfontpixelsize = global.m_fpt2pixel *(int)(global.atof_3(strFontSize.toStdWString().c_str()));
			}
			else
			{
				strFontSize = strFontSize.replace("px", "", Qt::CaseInsensitive);
				nfontpixelsize = (int)(global.atof_3(strFontSize.toStdWString().c_str()));
			}
		}

		QFont font;
		font.setFamily(QString::fromStdWString(fontName));
		font.setPixelSize(nfontpixelsize);
		QPainterPath path = QPainterPath();
		path.addText(QPointF(x, y), font, QString::fromStdWString(L"I"));
		QRectF box = path.boundingRect();
		m_bbox = CSVGRectF(box.x(), box.y(), box.width(), box.height());
	}
}

bool CSVGElementText::isTextBottom()
{
	if (GetUseStyle()[ATTR_WRITING_MODE] == L"tb")
	{
		return true;
	}
	return false;
}

void CSVGElementText::resetPath()
{
	if (m_bPathHasChanged)
	{
		ResetStyleUsed();
		m_Path = QPainterPath();
		float nx = x;
		float ny = y;
		float fxExt = 0;
		float fyExt = 0;
		for (size_t i = 0; i < m_ChildNodes.size(); ++i)
		{
			CSVGElement *pElement = m_ChildNodes[i];
			if (pElement->getNodeType() == SVG_TSPAN)
			{
				if (i > 0 && m_ChildNodes[i - 1]->getNodeType() == SVG_TEXTCONTENT)
				{
					CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)m_ChildNodes[i - 1];
					if (isTextBottom())
					{
						fyExt = pTextContent->m_bbox.height;
					}
					else
					{
						fxExt = pTextContent->m_bbox.width;
					}
				}
				CSVGElementTspan *pTSpan = (CSVGElementTspan*)pElement;
				pTSpan->resetTextPath(nx + fxExt, ny + fyExt);
				nx = pTSpan->fx + pTSpan->fdx;
				ny = pTSpan->fy + pTSpan->fdy;
				m_Path.addPath(pTSpan->m_Path);
			}
			if (pElement->getNodeType() == SVG_TEXTCONTENT)
			{
				if (i > 0 && m_ChildNodes[i - 1]->getNodeType() == SVG_TSPAN)
				{
					CSVGElementTspan *pTSpan = (CSVGElementTspan*)m_ChildNodes[i - 1];
					if (isTextBottom())
					{
						fyExt = pTSpan->m_bbox.height;
					}
					else
					{
						fxExt = pTSpan->m_bbox.width;
					}
				}
				CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)pElement;
				pTextContent->resetTextPath(nx + fxExt, ny + fyExt);
				m_Path.addPath(pTextContent->m_Path);
			}
		}
		CSVGElementShape::resetPath();
	}
}

void  CSVGElementText::resetpatternPath(int nNewWidth, int nNewHeight)
{
	resetPath();
	float scalex = nNewWidth*1.0 / m_bbox.width + 0.00001;
	float scaley = nNewHeight*1.0 / m_bbox.height + 0.00001;
	m_bbox.x *= scalex;
	m_bbox.y *= scaley;
	m_bbox.width *= scalex;
	m_bbox.height *= scaley;
}

int CSVGElementText::GetTextPos(float fx, float fy)
{
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	int nIndex = -1;
	if (m_ChildNodes.size() == 1 && m_ChildNodes[0]->getNodeType() == SVG_TEXTCONTENT)
	{
		CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)m_ChildNodes[0];
		std::wstring strVal = pTextContent->getNodeValue();
		std::wstring strtemp;
		CSVGRectF lstbox(0, 0, 0, 0);
		int nFindIndex = 0;
		for (int i = 0; i < (int)strVal.length(); ++i)
		{
			strtemp += strVal[i];
			CSVGRectF box = pTextContent->GetTextBox(strtemp, x, y);
			if (box.PtInRect(pt))
			{
				float fhalfwidth = (box.width - lstbox.width) / 2;
				CSVGRectF leftHalfBox = box;
				leftHalfBox.width -= fhalfwidth;
				if (leftHalfBox.PtInRect(pt))
				{
					nIndex = nFindIndex;
					break;
				}
				nFindIndex++;
				nIndex = nFindIndex;
				break;
			}
			lstbox = box;
			nFindIndex++;
		}
	}
	if (nIndex < 0)
	{
		CSVGRectF bbox = getBBox();
		if (bbox.PtInRect(pt))
		{
			nIndex = 0;
		}
	}
	return nIndex;
}

CSVGRectF  CSVGElementText::GetLeftTextBox(int nIndex)
{
	CSVGRectF box(0, 0, 0, 0);
	if (m_ChildNodes.size() == 1 && m_ChildNodes[0]->getNodeType() == SVG_TEXTCONTENT)
	{
		CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)m_ChildNodes[0];
		std::wstring strVal = pTextContent->getNodeValue();
		std::wstring strtemp;
		for (int i = 0; i < (int)strVal.length(); ++i)
		{
			if (i == nIndex)
			{
				break;
			}
			strtemp += strVal[i];
		}
		box = pTextContent->GetTextBox(strtemp, x, y);
		if (box.IsEmpty())
		{
			box = pTextContent->GetTextBox(L"|", x, y);
			box.x = x;
			box.width = 1;
		}
	}
	return box;
}

bool CSVGElementText::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
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

std::vector<CSVGElementTextContent*> CSVGElementText::GetTextContentArray()
{
	std::vector<CSVGElementTextContent*> ContentArr;
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		CSVGElement *pElement = m_ChildNodes[i];
		if (pElement->getNodeType() == SVG_TSPAN)
		{
			for (size_t j = 0; j < pElement->getChildCount(); ++j)
			{
				CSVGElement *pSpanChild = pElement->getChild(j);
				if (pSpanChild->getNodeType() == SVG_TEXTCONTENT)
				{
					ContentArr.push_back((CSVGElementTextContent*)pSpanChild);
				}
			}
		}
		if (pElement->getNodeType() == SVG_TEXTCONTENT)
		{
			CSVGElementTextContent *pTextContent = (CSVGElementTextContent*)pElement;
			ContentArr.push_back((CSVGElementTextContent*)pTextContent);
		}
	}
	return ContentArr;
}

std::wstring  CSVGElementText::GetText()
{
	std::vector<CSVGElementTextContent*> vcTextContent = GetTextContentArray();
	std::wstring val;
	for (size_t i = 0; i < vcTextContent.size(); ++i)
	{
		val += vcTextContent[i]->getNodeValue();
	}
	return val;
}

bool CSVGElementText::HasChildTSpan()
{
	std::vector<CSVGElementTextContent*> ContentArr;
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		CSVGElement *pElement = m_ChildNodes[i];
		if (pElement->getNodeType() == SVG_TSPAN)
		{
			return true;
		}
	}
	return false;
}
