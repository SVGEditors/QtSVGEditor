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
#include <map>
#include <QPen>
#include <QBrush>

class  CSVGElementStyleable : public CSVGElement
{
public:
	CSVGElementStyleable(CSVGDocument* ownerDoc, const std::wstring& tagname, unsigned short nodetype);
	virtual ~CSVGElementStyleable();
	virtual CSVGElement* cloneNode(bool deep);
	virtual void         setAttribute(const std::wstring &attrname, const std::wstring & value, bool bSetModfiyFlag = true);
	virtual std::wstring getAttribute(const std::wstring &attrname);
	virtual bool         IsStyleableElement() { return true; }
	virtual std::set<std::wstring> getAttrNames();
	void                 StyleOrClasstoAttr();
	void                 SetStyleChange(bool bChange);
private:
	bool                 addorsetAttribute(const std::wstring &attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	void                 ParseStyle(const std::wstring & style);
	void                 ParseStyle(std::map<int, std::wstring>&stylemaps, const std::wstring& style);
	void                 ParseClass(const std::wstring & strclass);
	std::wstring         MakeStyleString(std::map<int, std::wstring> &mapStyle);
	void                 buildbrush();
	void                 buildpen();
	void                 decodeStrokeDasharray(const std::wstring & data, float fwidth, QVector<qreal> & dasharay);
protected:
	std::map<int, std::wstring> &GetUseStyle();
	std::map<int, std::wstring> &GetSelfStyle();
	bool                 HasSingleBrush();
	QBrush               &GetSingleBrush();
	QPen                 &GetPen();
public:
	void                 ResetParentIfStyleHasChange();
	bool                 IsStyleChange();
	void                 ResetStyleUsed(CSVGElementShape *pParent = NULL);
private:
	std::map<int, std::wstring> m_usedStyle;
	std::map<int, std::wstring> m_selfStyle;
	bool                  m_bstylechange;
	QBrush                m_SingleBrush;
	bool                  m_bSingleBrush;
	QPen                  m_Pen;
};
