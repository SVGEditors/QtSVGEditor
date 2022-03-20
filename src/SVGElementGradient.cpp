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
#include "SVGElementGradient.h"
#include "SVGElementStop.h"
#include "SVGDocument.h"
#include "SVGGlobal.h"
#include "SVGDef.h"

CSVGElementGradient::CSVGElementGradient(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype)
	:CSVGElement(ownerDoc, tagname, nodetype)
{
	color = NULL;
	offset = NULL;
	nlen = 0;
}


CSVGElementGradient::~CSVGElementGradient()
{
}

void CSVGElementGradient::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	addorsetAttribute(attrname, value, bSetModfiyFlag);
	CSVGElement::setAttribute(attrname, value, bSetModfiyFlag);

}

bool CSVGElementGradient::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	unsigned short nIndex = global.getIndexByAttrName(attrname);
	switch (nIndex)
	{
	case ATTR_GRADIENTUNITS:
		gradientUnits = value;
		break;
	case ATTR_XLINK_HREF:
		xlink_href = value;
		break;
	case ATTR_GRADIENTTRANSFORM:
		gradientmat = value;
		break;
	default:
		return false;
	}
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
	return true;
}

void   CSVGElementGradient::BuildFromStopElement()
{
	if (color)
	{
		delete color;
		color = NULL;
	}
	if (offset)
	{
		delete offset;
		offset = NULL;
	}
	int nstopcount = 0;
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		CSVGElement *pElement = (CSVGElement*)m_ChildNodes[i];
		if (pElement->getNodeType() == SVG_STOP)
		{
			nstopcount++;
		}
	}
	if (xlink_href.length() > 0)
	{
		std::wstring id = global.UrlStringGetId(xlink_href.c_str());
		CSVGElement *pLinkElement = m_pownDocument->getElementById(id.c_str());
		if (pLinkElement)
		{
			if (gradientmat.length() == 0)
			{
				gradientmat = pLinkElement->getAttribute(L"gradientTransform");
			}
			if (gradientUnits.length() == 0)
			{
				gradientUnits = pLinkElement->getAttribute(L"gradientUnits");
			}
			for (int i = 0; i < (int)pLinkElement->getChildCount(); ++i)
			{
				CSVGElement *pElement = (CSVGElement*)pLinkElement->getChild(i);
				if (pElement->getNodeType() == SVG_STOP)
				{
					nstopcount++;
				}
			}
		}
	}

	nlen = nstopcount + 2;
	color = new SVGCOLORREF[nlen];
	offset = new float[nlen];
	memset(color, 0, sizeof(SVGCOLORREF)*nlen);
	for (int i = 0; i < nlen; ++i)
	{
		color[i] = SVGRGBA(255, 255, 255, 255);
	}
	memset(offset, 0, sizeof(float)*nlen);

	int index = 1;
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		CSVGElement *pElement = (CSVGElement*)m_ChildNodes[i];
		if (pElement->getNodeType() == SVG_STOP)
		{
			CSVGElementStop *pStop = (CSVGElementStop*)pElement;
			float fstop = pStop->getOffset();
			std::wstring strcolor = pStop->getStopColor();
			SVGCOLORREF clr = global.StrToColor(strcolor);
			std::wstring stropacity = pStop->getOpacity();
			if (stropacity.length() == 0)
				stropacity = L"1";
			SVGCOLORREF argb = SVGRGBA(SVGGetRValue(clr), SVGGetGValue(clr), SVGGetBValue(clr), global.atof_3(stropacity.c_str()) * 255);
			color[index] = argb;
			offset[index] = fstop;
			index++;
		}
	}

	if (xlink_href.length() > 0)
	{
		std::wstring id = global.UrlStringGetId(xlink_href);
		CSVGElement *pLinkElement = m_pownDocument->getElementById(id.c_str());
		if (pLinkElement)
		{
			for (int i = 0; i < (int)pLinkElement->getChildCount(); ++i)
			{
				CSVGElement *pElement = (CSVGElement*)pLinkElement->getChild(i);
				if (pElement->getNodeType() == SVG_STOP)
				{
					CSVGElementStop *pStop = (CSVGElementStop*)pElement;
					float fstop = pStop->getOffset();
					std::wstring strcolor = pStop->getStopColor();
					SVGCOLORREF clr = global.StrToColor(strcolor);
					std::wstring stropacity = pStop->getOpacity();
					if (stropacity.length() == 0)
						stropacity = L"1";
					SVGCOLORREF argb = SVGRGBA(SVGGetRValue(clr), SVGGetGValue(clr), SVGGetBValue(clr), global.atof_3(stropacity.c_str()) * 255);
					color[index] = argb;
					offset[index] = fstop;
					index++;
				}
			}
		}
	}

	Sort();
}

void CSVGElementGradient::Sort()
{
	int nSortLen = nlen - 2;
	for (int i = 1; i < nSortLen + 1; ++i)
	{
		for (int j = 1; j < nSortLen + 1 - i; ++j)
		{
			if (offset[j] > offset[j + 1])
			{
				float ftemp = offset[j];
				offset[j] = offset[j + 1];
				offset[j + 1] = ftemp;

				SVGCOLORREF ctemp = color[j];
				color[j] = color[j + 1];
				color[j + 1] = ctemp;
			}
		}
	}
	if (nlen > 2)
	{
		offset[0] = 0;
		color[0] = color[1];
		offset[nlen - 1] = 1;
		color[nlen - 1] = color[nlen - 2];
	}
}

bool  CSVGElementGradient::Toxml(std::wstring &strxml, int nLevel)
{
	std::wstring strid = getAttribute(L"id");
	if (m_pownDocument->IsUrlIdUsedByShape(strid))
	{
		return CSVGElement::Toxml(strxml, nLevel);
	}
	return false;
}

CSVGMatrix CSVGElementGradient::GetGradientMatrix()
{
	CSVGMatrix mat;
	if (gradientmat.length() > 0)
	{
		mat.setText(gradientmat);
	}
	return mat;
}
