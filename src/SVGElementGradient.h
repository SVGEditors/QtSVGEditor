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
#pragma once
#include "SVGElement.h"

class CSVGElementGradient : public CSVGElement
{
public:
	CSVGElementGradient(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype);
	~CSVGElementGradient();
	virtual void      setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	bool              addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	virtual bool      Toxml(std::wstring &strxml, int nLevel);
	CSVGMatrix        GetGradientMatrix();
protected:
	virtual void      BuildFromStopElement();
	void   Sort();
public:
	SVGCOLORREF* color;
	float *      offset;
	int             nlen;
	std::wstring     gradientUnits;
	std::wstring     xlink_href;
	std::wstring     gradientmat;
};
