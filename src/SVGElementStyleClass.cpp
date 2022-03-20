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
#include "SVGElementStyleClass.h"
#include "SVGGlobal.h"


CSVGElementStyleClass::CSVGElementStyleClass()
{
	m_mapAttr[L"noused"] = L"";
}

CSVGElementStyleClass::~CSVGElementStyleClass()
{
}

CSVGElementStyleClass *CSVGElementStyleClass::Clone()
{
	CSVGElementStyleClass *pnewClass = new CSVGElementStyleClass();
	pnewClass->m_mapAttr = m_mapAttr;
	pnewClass->m_name = m_name;
	pnewClass->m_strtext = m_strtext;
	return pnewClass;
}

std::wstring CSVGElementStyleClass::getAttr(const std::wstring &attrname)
{
	if (attrname.length() > 0)
	{
		std::map<std::wstring, std::wstring>::iterator iter = m_mapAttr.find(attrname);
		if (iter != m_mapAttr.end())
		{
			return iter->second;
		}
	}
	return L"";
}

void CSVGElementStyleClass::setAttr(const std::wstring &attrname, const std::wstring &attrval)
{
	if (attrname.length() > 0 && attrval.length() > 0)
	{
		m_mapAttr[attrname] = attrval;
	}
}

void CSVGElementStyleClass::parseClass(wchar_t *value)
{//stroke:#ffffff;stroke-linecap:round;stroke-linejoin:round;stroke-width:0.36
	if (!value)
		return;
	m_strtext = value;
	m_mapAttr.clear();

	while (*value == L' ' || *value == L'\t' || *value == L'\r' || *value == L'\n')
		value++;
	std::wstring name;
	std::wstring val;
	while (*value)
	{
		wchar_t *pTemptext = value;
		while (*value && *value != L':')
			value++;
		if (*value == L':')
		{
			char c = *value;
			*value = 0;
			name = pTemptext;
			*value = c;
			value++;
		}
		pTemptext = value;
		while (*value && *value != L';' && *value != L'\t'&& *value != L'\r'&& *value != L'\n')
			value++;

		wchar_t c = *value;
		*value = 0;
		val = pTemptext;
		*value = c;

		QString strName = QString::fromStdWString(name);
		QString strVal = QString::fromStdWString(val);
		strName = strName.trimmed();
		strVal = strVal.trimmed();

		m_mapAttr[strName.toStdWString()] = strVal.toStdWString();
		while (*value == L' ' || *value == L';' || *value == L'\t' || *value == L'\r' || *value == L'\n')
			value++;
	}
}

void CSVGElementStyleClass::setName(const std::wstring & name)
{
	m_name = name;
}

std::wstring CSVGElementStyleClass::getName()
{
	return m_name;
}

std::wstring CSVGElementStyleClass::getText()
{
	return m_strtext;
}

std::vector<std::wstring> CSVGElementStyleClass::GetUrlIDs()
{
	std::vector<std::wstring>vcUrl;
	std::wstring strfill = getAttr(L"fill");
	if (strfill.find(L"url(") != std::wstring::npos)
	{
		std::wstring strid = global.UrlStringGetId(strfill);
		vcUrl.push_back(strid);
	}
	return vcUrl;
}

void CSVGElementStyleClass::ChangeUrlID(const std::wstring& stroldid, const std::wstring& strnewid)
{
	std::wstring strtemp = global.g_replace_all_distinct(m_strtext, L"url(#" + stroldid, L"url(#" + strnewid);
	wchar_t *pBuff = new wchar_t[strtemp.length() + 1];
	wmemset(pBuff, 0, strtemp.length() + 1);
	wmemcpy(pBuff, strtemp.c_str(), strtemp.length());
	parseClass(pBuff);
}

void CSVGElementStyleClass::ChangeClassName(std::map<std::wstring, std::wstring>&oldtonewnamemap)
{
	std::wstring strnewclassname = L"st" + global.MakeId();
	oldtonewnamemap[m_name] = strnewclassname;
	m_name = strnewclassname;
}
