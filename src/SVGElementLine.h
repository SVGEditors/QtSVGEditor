﻿/**
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
class CSVGElementLine : public CSVGElementShape
{
public:
	CSVGElementLine(CSVGDocument* ownerDoc);
	~CSVGElementLine();
public:
	virtual void  setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  resetpatternPath(int nNewWidth, int nNewHeight);
	virtual void  calBBox();
	virtual void  resetPath();
	virtual bool  IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid);
	virtual bool  IsShapeInRect(CSVGRectF rectf);
	virtual int   HitTest(int nx, int ny, const CSVGMatrix *screenmat);
	virtual std::set<std::wstring> getAttrNames();
private:
	bool  addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	float x1;
	float y1;
	float x2;
	float y2;
};
