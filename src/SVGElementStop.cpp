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
#include "SVGElementStop.h"
#include "SVGDocument.h"

CSVGElementStop::CSVGElementStop(CSVGDocument* ownerDoc)
	:CSVGElementStyleable(ownerDoc, L"stop", SVG_STOP)
{
}

CSVGElementStop::~CSVGElementStop()
{

}

void CSVGElementStop::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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


bool CSVGElementStop::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"offset")
	{
		offset = value;
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

std::wstring CSVGElementStop::getAttribute(const std::wstring &attrname)
{
	unsigned short attrtype = global.getIndexByAttrName(attrname);
	std::wstring str;
	switch (attrtype)
	{
	case ATTR_OFFSET:
	{
		str = offset.c_str();
		break;
	}
	default:
	{
		return CSVGElementStyleable::getAttribute(attrname);
	}
	}
	return str;
}


float CSVGElementStop::getOffset()
{
	if (offset.find(L"%") != std::wstring::npos)
	{
		return global.atof_3(offset.c_str()) / 100;
	}
	return global.atof_3(offset.c_str());
}

std::wstring CSVGElementStop::getStopColor()
{
	return GetUseStyle()[ATTR_STOP_COLOR].c_str();
}

std::wstring CSVGElementStop::getOpacity()
{
	return GetUseStyle()[ATTR_STOP_OPACITY].c_str();
}
