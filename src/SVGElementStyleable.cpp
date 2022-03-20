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
#include "SVGElementStyleable.h"
#include "SVGElementStyleClass.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "SVGGlobal.h"
#include "main.h"

CSVGElementStyleable::CSVGElementStyleable(CSVGDocument* ownerDoc, const std::wstring& tagname, unsigned short nodetype)
	:CSVGElement(ownerDoc, tagname, nodetype)
{
	m_bstylechange = true;
	m_bSingleBrush = false;
}

CSVGElementStyleable::~CSVGElementStyleable()
{
}

CSVGElement* CSVGElementStyleable::cloneNode(bool deep)
{
	CSVGElement* pNewElement = CSVGElement::cloneNode(deep);
	if (pNewElement)
	{
		CSVGElementStyleable *pStyle = (CSVGElementStyleable*)pNewElement;
		pStyle->m_usedStyle = m_usedStyle;
		pStyle->m_selfStyle = m_selfStyle;
	}
	return pNewElement;
}

void  CSVGElementStyleable::ParseStyle(std::map<int, std::wstring>&stylemaps, const std::wstring& style)
{
	int nLen = style.length();
	wchar_t *pHeadChar = new wchar_t[nLen + 1];
	std::wmemset(pHeadChar, 0, nLen + 1);
	wmemcpy(pHeadChar, style.c_str(), nLen);
	wchar_t *pChar = pHeadChar;
	wchar_t *name_ = NULL;
	wchar_t *value_ = NULL;
	wchar_t ctemp = 0;
	wchar_t name[32];
	while (*pChar)
	{
		while (*pChar && global.ISSpace(*pChar))
			pChar++;
		name_ = pChar;
		while (*pChar&&*pChar != L':')
			pChar++;
		ctemp = *pChar;
		*pChar = 0;
		std::wmemset(name, 0, 32);
		std::wcsncpy(name, name_, 32);
		*pChar = ctemp;
		while (*pChar && (*pChar == L':') || global.ISSpace(*pChar))
			pChar++;
		value_ = pChar;
		while (*pChar && *pChar != L';')
			pChar++;
		ctemp = *pChar;
		*pChar = 0;
		if (name &&value_)
		{
			unsigned short attrindex = global.getIndexByAttrName(name);
			stylemaps[attrindex] = value_;
		}
		*pChar = ctemp;
		while (*pChar == L';')
			pChar++;
	}
	delete[]pHeadChar;
}

void CSVGElementStyleable::ParseStyle(const std::wstring& style)
{
	ParseStyle(m_selfStyle, style);
}

void  CSVGElementStyleable::StyleOrClasstoAttr()
{
	if (m_selfStyle[ATTR_STYLE].length() > 0)
	{
		std::map<int, std::wstring> mapStyle;
		ParseStyle(mapStyle, m_selfStyle[ATTR_STYLE].c_str());
		std::map<int, std::wstring>::iterator iter = mapStyle.begin();
		for (; iter != mapStyle.end(); ++iter)
		{
			m_selfStyle[iter->first] = iter->second;
			m_usedStyle[iter->first] = iter->second;
		}
		m_selfStyle[global.getIndexByAttrName(L"style")] = L"";
	}
	if (m_selfStyle[ATTR_CLASS].length() > 0)
	{
		CSVGElementStyleClass * pClass = m_pownDocument->getClassByName(m_selfStyle[ATTR_CLASS].c_str());
		if (pClass)
		{
			std::map<std::wstring, std::wstring>::iterator iter = pClass->m_mapAttr.begin();
			for (; iter != pClass->m_mapAttr.end(); ++iter)
			{
				unsigned short attrindex = global.getIndexByAttrName(iter->first.c_str());
				if (m_selfStyle[attrindex].length() == 0)
				{
					m_selfStyle[attrindex] = iter->second.c_str();
					m_usedStyle[attrindex] = iter->second.c_str();
				}
			}
		}
		m_selfStyle[ATTR_CLASS] = L"";
	}
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			((CSVGElementShape*)m_ChildNodes[i])->StyleOrClasstoAttr();
		}
	}
}

void  CSVGElementStyleable::ParseClass(const std::wstring &strclass)
{
	std::wstring strValue = strclass;
	strValue = StrReplace(strValue, L" ", L",");
	strValue = StrReplace(strValue, L";", L",");
	std::vector<std::wstring> vcValue = global.tokenize(strValue, L",");

	CSVGDocument* ownerDoc = getOwnerDocument();
	for (size_t i = 0; i < vcValue.size(); ++i)
	{
		CSVGElementStyleClass *pClass = ownerDoc->getClassByName(vcValue[i]);
		if (pClass)
		{
			std::map<std::wstring, std::wstring>::iterator iter = pClass->m_mapAttr.begin();
			for (; iter != pClass->m_mapAttr.end(); ++iter)
			{
				unsigned short attrindex = global.getIndexByAttrName(iter->first.c_str());
				m_selfStyle[attrindex] = iter->second.c_str();
			}
		}
	}
}

std::wstring CSVGElementStyleable::MakeStyleString(std::map<int, std::wstring> &mapStyle)
{
	std::wstring str;
	std::map<int, std::wstring>::iterator iter = mapStyle.begin();
	for (; iter != mapStyle.end(); ++iter)
	{
		str += global.getAttrNameByIndex(iter->first);
		str += L":";
		str += iter->second.c_str();
		str += L";";
	}
	return str;
}

void CSVGElementStyleable::setAttribute(const std::wstring &attrname, const std::wstring &value, bool bSetModfiyFlag)
{
	std::wstring attrnewname = attrname;
	std::wstring attrnewval = value;
	if (addorsetAttribute(attrname, value, bSetModfiyFlag))
	{
		if (m_selfStyle[ATTR_STYLE].length() > 0 && bSetModfiyFlag)
		{
			std::map<int, std::wstring>mapstyle;
			ParseStyle(mapstyle, m_selfStyle[ATTR_STYLE].c_str());
			int index = global.getIndexByAttrName(attrname);
			if (mapstyle[index].length() > 0)
			{//style="fill:url(#grad28-9)"
				m_selfStyle[index] = L"";
				mapstyle[index] = value;
				std::wstring strstyle = MakeStyleString(mapstyle);
				m_selfStyle[ATTR_STYLE] = strstyle.c_str();
				attrnewname = L"style";
				attrnewval = strstyle;
			}
		}
	}
	CSVGElement::setAttribute(attrnewname, attrnewval, bSetModfiyFlag);
}

bool CSVGElementStyleable::addorsetAttribute(const std::wstring &attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (IsStyleAttr(attrname))
	{
		unsigned short nIndex = global.getIndexByAttrName(attrname);
		m_selfStyle[nIndex] = value;
		if (bSetModfiyFlag)
		{
			SetStyleChange(true);
		}
		return true;
	}
	return false;
}

std::set<std::wstring> CSVGElementStyleable::getAttrNames()
{
	std::set<std::wstring>vcName = CSVGElement::getAttrNames();
	std::map<int, std::wstring>::iterator iter = m_selfStyle.begin();
	for (; iter != m_selfStyle.end(); ++iter)
	{
		if (iter->first != ATTR_STROKE
			&& iter->first != ATTR_STROKE_OPACITY
			&& iter->first != ATTR_FILL
			&& iter->first != ATTR_FILL_OPACITY)
		{
			vcName.insert(vcName.begin(), global.getAttrNameByIndex(iter->first));
		}
	}
	vcName.insert(vcName.begin(), L"fill-opacity");
	vcName.insert(vcName.begin(), L"fill");
	vcName.insert(vcName.begin(), L"stroke-opacity");
	vcName.insert(vcName.begin(), L"stroke");
	return vcName;
}

std::wstring CSVGElementStyleable::getAttribute(const std::wstring &attrname)
{
	unsigned short attrtype = global.getIndexByAttrName(attrname);
	if (global.IsStyleAttr(attrtype))
	{
		return m_selfStyle[attrtype].c_str();
	}

	return CSVGElement::getAttribute(attrname);
}

void CSVGElementStyleable::ResetStyleUsed(CSVGElementShape *pParent)
{
	m_bstylechange = false;
	ResetParentIfStyleHasChange();


	if (m_selfStyle[ATTR_STYLE].length() > 0)
	{
		ParseStyle(m_selfStyle[ATTR_STYLE].c_str());
	}
	if (m_selfStyle[ATTR_CLASS].length() > 0)
	{
		ParseClass(m_selfStyle[ATTR_CLASS].c_str());
	}
	m_usedStyle = m_selfStyle;

	CSVGElementStyleable *pShapeParent = NULL;
	if (pParent)
	{
		pShapeParent = (CSVGElementStyleable*)pParent;
	}
	else if (m_parentNode &&m_parentNode->IsStyleableElement())
	{
		pShapeParent = (CSVGElementStyleable*)m_parentNode;
	}
	if (pShapeParent)
	{
		std::map<int, std::wstring>::iterator iter = pShapeParent->m_usedStyle.begin();
		for (; iter != pShapeParent->m_usedStyle.end(); ++iter)
		{
			if (iter->first == ATTR_DISPLAY &&m_usedStyle[iter->first] != L"none")
			{
				m_usedStyle[iter->first] = L"";
			}
			else if (iter->first == ATTR_VISIBILITY && m_usedStyle[iter->first] != L"hidden")
			{
				m_usedStyle[iter->first] = L"";
			}
			if (m_usedStyle[iter->first].length() == 0 && pShapeParent->m_usedStyle[iter->first].length() > 0)
			{
				m_usedStyle[iter->first] = pShapeParent->m_usedStyle[iter->first];
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_usedStyle[ATTR_FILL].length() == 0 && m_usedStyle[ATTR_FILL_OPACITY].length() == 0)
	{
		m_usedStyle[ATTR_FILL] = L"#000000";
		m_usedStyle[ATTR_FILL_OPACITY] = L"1";
	}
	if (m_usedStyle[ATTR_FILL] == L"none")
	{
		if (m_ChildNodes.size() == 0)
		{
			m_usedStyle[ATTR_FILL_OPACITY] = L"0";
		}
	}
	else if (m_usedStyle[ATTR_FILL].length() > 0 && m_usedStyle[ATTR_FILL_OPACITY].length() == 0)
	{
		m_usedStyle[ATTR_FILL_OPACITY] = L"1";
	}
	if (m_usedStyle[ATTR_STROKE].length() > 0 && m_usedStyle[ATTR_STROKE] != L"none" && m_usedStyle[ATTR_STROKE_OPACITY].length() == 0)
	{
		m_usedStyle[ATTR_STROKE_OPACITY] = L"1";
	}
	if (m_usedStyle[ATTR_OPACITY].length() > 0)
	{
		if (global.atof_3(m_usedStyle[ATTR_OPACITY].c_str()) < global.atof_3(m_usedStyle[ATTR_FILL_OPACITY].c_str()))
		{
			m_usedStyle[ATTR_FILL_OPACITY] = m_usedStyle[ATTR_OPACITY];
		}
		if (global.atof_3(m_usedStyle[ATTR_OPACITY].c_str()) < global.atof_3(m_usedStyle[ATTR_STROKE_OPACITY].c_str()))
		{
			m_usedStyle[ATTR_STROKE_OPACITY] = m_usedStyle[ATTR_OPACITY];
		}
	}
	if (m_usedStyle[ATTR_STROKE_WIDTH].length() > 0)
	{
		float fwidth = global.atof_3(m_usedStyle[ATTR_STROKE_WIDTH].c_str());
		if (fwidth > 0.001 && fwidth < 1)
		{
			float fopacity = global.atof_3(m_usedStyle[ATTR_STROKE_OPACITY].c_str());
			fopacity = fopacity * fwidth;
			std::wstring wval = global.ftostring(fopacity);
			m_usedStyle[ATTR_STROKE_OPACITY] = wval;
		}
	}
	buildbrush();
	buildpen();
}


std::map<int, std::wstring> &CSVGElementStyleable::GetUseStyle()
{
	if (m_bstylechange)
	{
		ResetStyleUsed();
	}
	return m_usedStyle;
}

std::map<int, std::wstring> &CSVGElementStyleable::GetSelfStyle()
{
	if (m_bstylechange)
	{
		ResetStyleUsed();
	}
	return m_selfStyle;
}

void CSVGElementStyleable::buildbrush()
{
	m_bSingleBrush = false;
	std::wstring strfill = GetUseStyle()[ATTR_FILL].c_str();
	if (strfill.find(L"url(#", 0) == 0)
	{
		return;
	}
	SVGCOLORREF color = global.StrToColor(strfill);
	std::wstring opacity = GetUseStyle()[ATTR_FILL_OPACITY];
	float fopacity = global.atof_3(opacity);
	if (fopacity <= 0.01)
	{
		m_SingleBrush = QBrush(Qt::NoBrush);
	}
	else
	{
		int aopacity = SVGGetAValue(color);
		QColor clr(SVGGetRValue(color), SVGGetGValue(color), SVGGetBValue(color), global.atof_3(opacity) * aopacity);
		m_SingleBrush = QBrush(clr);
	}
	m_bSingleBrush = true;
}

void CSVGElementStyleable::decodeStrokeDasharray(const std::wstring & data, float fwidth, QVector<qreal> & dasharay)
{
	if (data == L"")
	{
		dasharay.clear();
		return;
	}
	std::wstring datastr_ = data;
	std::wstring delstr_ = L", ";
	std::vector<std::wstring> retData;
	retData = global.tokenize(datastr_, delstr_);
	int nSize_ = retData.size();
	int i = 0;
	if (fwidth <= 0.01)
	{
		fwidth = 1;
	}
	for (; i < nSize_; ++i)
	{
		dasharay.push_back(global.atof_3(retData[i].c_str()) / fwidth);
	}
	return;
}

void  CSVGElementStyleable::buildpen()
{
	std::wstring stroke = GetUseStyle()[ATTR_STROKE].c_str();
	std::wstring opacity = GetUseStyle()[ATTR_STROKE_OPACITY].c_str();
	std::wstring width = GetUseStyle()[ATTR_STROKE_WIDTH].c_str();
	std::wstring strdasharray = GetUseStyle()[ATTR_STROKE_DASHARRAY].c_str();
	if (strdasharray == L"none")
	{
		strdasharray = L"";
	}
	SVGCOLORREF color = global.StrToColor(stroke);
	QColor clr(SVGGetRValue(color), SVGGetGValue(color), SVGGetBValue(color), (int)(global.atof_3(opacity) * 255));
	float fwidth = global.atof_3(width);
	if (width.length() > 0 && fwidth < 0.01)
	{
		m_Pen = Qt::NoPen;
	}
	else
	{
		m_Pen = QPen(clr);
		m_Pen.setWidthF(fwidth);
		if (strdasharray.length() > 0)
		{
			QVector<qreal> dasharray;
			decodeStrokeDasharray(strdasharray, global.atof_3(width), dasharray);
			int nSize = (int)dasharray.size();
			if (nSize > 0)
			{
				m_Pen.setDashPattern(dasharray);
			}
			std::wstring dashoffset = GetUseStyle()[ATTR_STROKE_DASHOFFSET];
			if (dashoffset.length() > 0)
			{
				m_Pen.setDashOffset(global.atof_3(dashoffset));
			}
		}
	}
}

bool CSVGElementStyleable::HasSingleBrush()
{
	return m_bSingleBrush;
}

QBrush &CSVGElementStyleable::GetSingleBrush()
{
	if (m_bstylechange)
	{
		ResetStyleUsed();
	}
	return m_SingleBrush;
}

QPen  &CSVGElementStyleable::GetPen()
{
	if (m_bstylechange)
	{
		ResetStyleUsed();
	}
	return m_Pen;
}

void CSVGElementStyleable::SetStyleChange(bool bChange)
{
	m_bstylechange = bChange;
	if (m_bstylechange)
	{
		for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
		{
			if (m_ChildNodes[i]->IsShapeElement())
			{
				CSVGElementShape *pShapeChild = (CSVGElementShape*)m_ChildNodes[i];
				if (!pShapeChild->IsStyleChange())
				{
					pShapeChild->SetStyleChange(bChange);
				}
			}
		}
	}
}

bool CSVGElementStyleable::IsStyleChange()
{
	return m_bstylechange;
}

void CSVGElementStyleable::ResetParentIfStyleHasChange()
{
	if (m_parentNode && m_parentNode->IsShapeElement())
	{
		CSVGElementShape *pParentShape = (CSVGElementShape*)m_parentNode;
		if (pParentShape->IsStyleChange())
		{
			pParentShape->ResetStyleUsed();
		}
	}
}
