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
#include "SVGPointF.h"
class CSVGElementTextContent : public CSVGElementShape
{
	friend class CSVGElementText;
	friend class CSVGElementTspan;
public:
	CSVGElementTextContent(CSVGDocument* ownerDoc);
	~CSVGElementTextContent();
	void          resetTextPath(float nx, float ny);
	virtual bool  IsShapeInRect(const CSVGRectF &rectf);
	virtual void  setNodeValue(const std::wstring &value, bool bSetModfiyFlag = true);
	int           GetTextLength();
	virtual bool  Toxml(std::wstring &strxml, int nLevel);
	CSVGRectF     GetTextBox(const std::wstring &, float fx, float fy);
private:
	void          FormatSpaceString();
	void          ResetTextFont(const std::wstring &Text, float nx, float ny);
	std::wstring  m_fontName;
	bool          m_bUnderLine;
	bool          m_bBold;
	bool          m_bItalic;
	int           m_nfontPixelSize;
	bool          m_btopbottom;
};
