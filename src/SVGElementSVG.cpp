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
#include "SVGElementSVG.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"

CSVGElementSVG::CSVGElementSVG(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"svg", SVG_SVG)
{
	x = 0;
	y = 0;
	width = 600;
	height = 600;
	viewBoxx = 0;
	viewBoxy = 0;
	viewBoxw = width;
	viewBoxh = height;

	xmlns = L"http://www.w3.org/2000/svg";
	xmlns_xlink = L"http://www.w3.org/1999/xlink";
	setFloatAttribute(L"width", width, false);
	setFloatAttribute(L"height", height, false);
}

CSVGElementSVG::~CSVGElementSVG()
{
}

void CSVGElementSVG::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	addAttribute(attrname, value, bSetModfiyFlag);
	m_strXml.clear();
}

void CSVGElementSVG::addAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementSVG::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	Q_UNUSED(bSetModfiyFlag);
	if (attrname == L"width")
	{
		width = global.atof_3(value);
	}
	else if (attrname == L"height")
	{
		height = global.atof_3(value);
	}
	else if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else if (attrname == L"xmlns")
	{
		xmlns = value;
	}
	else if (attrname == L"xmlns:xlink")
	{
		xmlns_xlink = value;
	}
	else if (attrname == L"viewBox")
	{
		viewBox = value;
		std::vector<std::wstring>vcAttr = global.tokenize(viewBox, L" ");
		if (vcAttr.size() == 4)
		{
			viewBoxx = global.atof_3(vcAttr[0].c_str());
			viewBoxy = global.atof_3(vcAttr[1].c_str());
			viewBoxw = global.atof_3(vcAttr[2].c_str());
			viewBoxh = global.atof_3(vcAttr[3].c_str());
		}
	}
	else
	{
		return false;
	}
	return true;
}

void CSVGElementSVG::resetPath()
{
	CSVGElementShape::resetPath();
}

std::set<std::wstring> CSVGElementSVG::getNamespaceAttrNames()
{
	std::set<std::wstring> vcNameSpace;

	std::set<std::wstring> vcName = CSVGElement::getAttrNames();
	std::set<std::wstring>::iterator iter = vcName.begin();
	for (; iter != vcName.end(); ++iter)
	{
		if (iter->find(L"xmlns:") != std::wstring::npos)
		{
			vcNameSpace.insert(*iter);
		}
	}
	return vcNameSpace;
}

CSVGMatrix  *CSVGElementSVG::getOwnerMatrix()
{
	return &m_tempMatrix;
}
