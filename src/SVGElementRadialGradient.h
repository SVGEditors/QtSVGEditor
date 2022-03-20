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
#include "SVGElementGradient.h"
class CSVGElementRadialGradient : public CSVGElementGradient
{
public:
	CSVGElementRadialGradient(CSVGDocument* ownerDoc);
	~CSVGElementRadialGradient();
	virtual void      setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = false);
	QBrush            getBrush(const CSVGRectF &bbox, float fopacity);
	SVGCOLORREF       getOutColor();
private:
	void              reBuildBrush();
	bool              addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	std::wstring       m_cx;
	std::wstring       m_cy;
	std::wstring       m_r;
	std::wstring       m_fx;
	std::wstring       m_fy;
};
