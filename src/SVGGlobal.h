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
#include <string>
#include <map>
#include <vector>
#include <list>
#include <vector>
#include "SVGDef.h"
#include "StringHash.h"
#include "SVGPointF.h"
#include <QColor>

class CSVGGlobal
{
public:
	CSVGGlobal();
	~CSVGGlobal();
public:
	std::wstring ftostring(float fvalue);
	std::wstring ftostring4(float fvalue);
	std::wstring itostring(int nval);
	std::wstring   g_replace_all_distinct(const std::wstring &str, const std::wstring &old_value, const std::wstring& new_value);
	float  atof_3(const std::wstring &str);
	int    strtoi(const std::wstring &str);
	bool   ISSpace(int nVal);
	bool   ISDigit(int nVal);
	std::wstring ColorToStr(const SVGCOLORREF & color);
	SVGCOLORREF  StrToColor(const std::wstring &str);
	QColor       SVGColorToQ(const SVGCOLORREF & color);
	QColor       StrToQColor(const QString &strcolor);
	QColor       StdToQColor(const std::wstring &strcolor);
	COLOR_TYPE   GetColorType(const std::wstring &str);
	std::wstring   GetColorByWord(const std::wstring& word);
	std::wstring  QColorToStr(const QColor &clr);
	QString GetCorrectUnicode(const QByteArray &ba);
	std::wstring AddAttrTextValue(const std::wstring &attrname, float val, const std::wstring &strtext);
	std::wstring AddAttrTextValue(const std::wstring &attrname, const std::wstring &attrval, const std::wstring &strtext);
	std::vector<std::wstring> tokenize(const std::wstring &str, const std::wstring &delimiters);
	std::wstring UrlStringGetId(const std::wstring & str);
	void GetparallelLine(const CSVGPointF &ptStart, const CSVGPointF & ptEnd, CSVGPointF &GetPtStart, CSVGPointF &GetPtEnd, double dDistance);
	bool PtInPolygonEx(const std::vector<CSVGPointF>& polygon, CSVGPointF& pt);
	bool PtInPolygonEx(const std::list<CSVGPointF>& polygon, CSVGPointF& pt);
	int base64_decode(const  char *src, int srclen, char *out, int ndeslen);
	std::wstring MakeRandStr();
	std::wstring MakeId();
public:
	unsigned short getIndexByTagName(const std::wstring &tagName);
	std::wstring   getTagNameByIndex(int nIndex);
	unsigned short getIndexByAttrName(const std::wstring& attrName);
	std::wstring   getAttrNameByIndex(int nIndex);
	bool           IsStyleAttr(int nIndex);
private:
	CSVGPointF     GetPosInLine(const CSVGPointF & ptStart, const CSVGPointF & ptEnd, double dDistance);
	CSVGPointF     GetParaPostion(const CSVGPointF & ptStart, const CSVGPointF & ptEnd, double dDistance, bool bStart);
	int            GetQuadrant(const CSVGPointF & ptStart, const CSVGPointF & ptEnd);
	void           InitColor();
	void           InitTagNameMap();
	void           InitAttrNameMap();
	void           InitStyleAttr();
	std::map<std::wstring, std::wstring>           m_colormap;
public:
	wchar_t         gspace[256];
	wchar_t         m_styleAttr[256];
	CStringHash  m_tagNameHash;
	CStringHash  m_attrNameHash;
	std::vector<std::wstring> m_vcTagName;
	std::vector<std::wstring> m_vcAttrName;
	int            nIndex;
public:
	float  m_fmulmm2pixel;
	float  m_fmulpt2mm;
	float  m_fpt2pixel;
	float  m_fpixel2pt;
	float  m_fscreenscale;
};

extern CSVGGlobal global;
