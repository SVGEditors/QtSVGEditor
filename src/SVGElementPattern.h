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
#pragma once
#include "SVGElementStyleable.h"

//<pattern id = "111" x = "0" y = "0" width = "1" height = "1" patternContentUnits = "objectBoundingBox">
//<path d = "M 0 1 L 0 0 L 1 0 z" fill = "#FF0000" / >
//<path d = "M 0 1 L 1 1 L 1 0 z" fill = "#00FF00" / >
//< / pattern>
class CSVGElementPattern : public CSVGElementStyleable
{
public:
	CSVGElementPattern(CSVGDocument* ownerDoc);
	~CSVGElementPattern();
	virtual void      setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	QBrush            getBrush(const CSVGRectF &bbox);
	virtual bool      Toxml(std::wstring &retstrxml, int nLevel);

private:
	bool              addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	float             x;
	float             y;
	float             width;
	float             height;
	std::wstring      m_patternContentUnits;//objectBoundingBox,
};
