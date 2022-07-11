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
class CSVGElementSVG : public CSVGElementShape
{
public:
	CSVGElementSVG(CSVGDocument* ownerDoc);
	virtual ~CSVGElementSVG();
	virtual void  addAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  resetPath();
	std::set<std::wstring> getNamespaceAttrNames();
	virtual CSVGMatrix  *getOwnerMatrix();
private:
	bool  addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	float x;
	float y;
	float width;
	float height;
	std::wstring xmlns;
	std::wstring xmlns_xlink;
	std::wstring viewBox;
	float viewBoxx;
	float viewBoxy;
	float viewBoxw;
	float viewBoxh;
	CSVGMatrix m_matViewBox;
	CSVGMatrix m_tempMatrix;
};
