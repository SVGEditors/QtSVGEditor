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
#include "SVGElementGradient.h"
#include <QBrush>
//<linearGradient id = "linearGradient">
//<stop offset = "0%" stop - color = "rgb(154,120,0)" stop - opacity = "1" / >
//<stop offset = "49%" stop - color = "rgb(255,249,227)" stop - opacity = "1" / >
//<stop offset = "50%" stop - color = "rgb(255,255,255)" stop - opacity = "1" / >
//<stop offset = "51%" stop - color = "rgb(255,255,255)" stop - opacity = "1" / >
//<stop offset = "100%" stop - color = "rgb(0,0,128)" stop - opacity = "1" / >
//< / linearGradient>

class CSVGElementLinearGradient : public CSVGElementGradient
{
public:
	CSVGElementLinearGradient(CSVGDocument* ownerDoc);
	~CSVGElementLinearGradient();
	virtual void      setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	QBrush            getBrush(const CSVGRectF &bbox, float fopacity);
private:
	void              reBuildBrush();
	bool              addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	float m_x1;
	float m_y1;
	float m_x2;
	float m_y2;
};
