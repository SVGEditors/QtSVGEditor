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
#include <map>
#include <string>
#include <vector>
class CSVGElementStyleClass
{
	friend class CSVGElementStyle;
	friend class CSVGElementStyleable;
public:
	CSVGElementStyleClass();
	~CSVGElementStyleClass();
public:
	CSVGElementStyleClass *Clone();
	std::wstring getAttr(const std::wstring &attrname);
	void         setAttr(const std::wstring &attrname, const std::wstring & attrval);
	void         setName(const std::wstring &name);
	std::wstring getName();
	std::wstring getText();
	std::vector<std::wstring> GetUrlIDs();
	void         ChangeUrlID(const std::wstring& stroldid, const std::wstring& strnewid);
	void         ChangeClassName(std::map<std::wstring, std::wstring>&oldtonewnamemap);
private:
	void parseClass(wchar_t *value);
	std::map<std::wstring, std::wstring>m_mapAttr;
	std::wstring m_name;
	std::wstring m_strtext;
};
