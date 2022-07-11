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
#include "SVGElementShape.h"
class CSVGElementTspan : public CSVGElementShape
{
	friend class CSVGElementText;
public:
	CSVGElementTspan(CSVGDocument* ownerDoc);
	~CSVGElementTspan();
	virtual void         setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  setNodeValue(const std::wstring& value, bool bSetModfiyFlag = true);
	void resetTextPath(float nx, float ny);
	virtual void  calBBox();
private:
	bool  addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	float       fx;
	float       fy;
	float       fdx;
	float       fdy;
	std::wstring x;
	std::wstring y;
	std::wstring dx;
	std::wstring dy;
};
