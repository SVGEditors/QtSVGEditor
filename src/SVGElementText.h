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
#include "SVGElementShape.h"
class CSVGElementTextContent;
class CSVGElementText : public CSVGElementShape
{
public:
	CSVGElementText(CSVGDocument* ownerDoc);
	~CSVGElementText();
	virtual void  setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  setNodeValue(const std::wstring& value, bool bSetModfiyFlag = true);
	virtual std::wstring getNodeValue();
	virtual void  resetpatternPath(int nNewWidth, int nNewHeight);
	virtual bool  IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid);
	virtual void  resetPath();
	bool          isTextBottom();
	std::wstring  GetText();
	int           GetTextPos(float fx, float fy);
	CSVGRectF     GetLeftTextBox(int nIndex);
	bool          HasChildTSpan();
private:
	bool  addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	virtual void  calBBox();
	std::vector<CSVGElementTextContent*> GetTextContentArray();
private:
	float	   x;
	float      y;
	int        m_ncursorpos;
	int        m_nAllCharSize;
};
