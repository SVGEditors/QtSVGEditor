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
#include "SVGElementStyle.h"
#include "SVGDocument.h"
#include "main.h"

CSVGElementStyle::CSVGElementStyle(CSVGDocument* ownerDoc)
	:CSVGElement(ownerDoc, L"style", SVG_STYLE)
{
	m_pownDocument->AddStyleElement(this);
}

CSVGElementStyle::~CSVGElementStyle()
{
	m_pownDocument->RemoveStyleElement(this);
	Clear();
}

void CSVGElementStyle::Clear()
{
	std::vector<CSVGElementStyleClass*>::iterator iter = m_vcclass.begin();
	for (; iter != m_vcclass.end(); ++iter)
	{
		delete *iter;
	}
}

std::vector<std::wstring> CSVGElementStyle::GetClassNames()
{
	std::vector<std::wstring> vcNames;
	std::vector<CSVGElementStyleClass*>::iterator iter = m_vcclass.begin();
	for (; iter != m_vcclass.end(); ++iter)
	{
		vcNames.push_back((*iter)->getName());
	}
	return vcNames;
}

CSVGElementStyleClass *CSVGElementStyle::getClassByName(const std::wstring &name)
{
	for (int i = m_vcclass.size() - 1; i >= 0; --i)
	{
		if (m_vcclass[i]->getName() == name)
			return m_vcclass[i];
	}
	return NULL;
}

std::vector<CSVGElementStyleClass*> CSVGElementStyle::GetClass()
{
	return m_vcclass;
}

void   CSVGElementStyle::parseReady()
{
	std::wstring strVal = getNodeValue();
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		CSVGElement *pChild = (CSVGElement*)m_ChildNodes[i];
		if (pChild->getNodeType() == XML_DATA)
		{
			std::wstring val = pChild->getNodeValue();
			val = StrReplace(val, L"<![CDATA[", L"");
			val = StrReplace(val, L"]]>", L"");
			strVal += val;
		}
	}
	parserValue(strVal);
}

void CSVGElementStyle::parserValue(const std::wstring & strValue)
{
	int nLen = strValue.length();
	wchar_t * pBeforeBuff = new wchar_t[nLen + 10];
	wchar_t *pBuff = pBeforeBuff;
	std::wmemset(pBuff, 0, nLen + 10);
	std::wmemcpy(pBuff, strValue.c_str(), nLen);

	while (*pBuff)
	{
		while (*pBuff == L' ' || *pBuff == L'\t' || *pBuff == L'\r' || *pBuff == L'\n')
			pBuff++;
		if (*pBuff == L'.')
		{//.st2 {fill:url(#grad30-7)}
			std::wstring classname;
			wchar_t *pTempText = pBuff + 1;
			while (*pBuff && *pBuff != L' ' && *pBuff != L'\t' && *pBuff != L'\r' && *pBuff != L'\n' && *pBuff != L'{')
				pBuff++;
			wchar_t c = *pBuff;
			*pBuff = 0;
			classname = pTempText;
			*pBuff = c;
			while (*pBuff && *pBuff != L'{')
				pBuff++;
			if (*pBuff == L'{')
			{
				pTempText = pBuff + 1;
				while (*pBuff && *pBuff != L'}')
					pBuff++;
				if (*pBuff == L'}')
				{
					*pBuff = 0;

					std::vector<std::wstring>vcClassName = global.tokenize(classname, L",");
					for (int k = 0; k < (int)vcClassName.size(); ++k)
					{
						std::wstring strclassname = vcClassName[k];
						if (strclassname.length() > 0 && strclassname.at(0) == L'.')
						{
							strclassname = strclassname.substr(1, strclassname.length() - 1);
						}
						CSVGElementStyleClass *pClass = m_pownDocument->getClassByName(strclassname);
						if (!pClass)
						{
							pClass = new CSVGElementStyleClass;
							pClass->setName(strclassname);
							m_vcclass.push_back(pClass);
						}
						pClass->parseClass(pTempText);
					}

					*pBuff = L'}';
				}
			}
		}
		pBuff++;
	}
	delete pBeforeBuff;
}

CSVGElement  * CSVGElementStyle::cloneNodeWithId(bool deep)
{
	CSVGElementStyle * pNewElement = (CSVGElementStyle*)CSVGElement::cloneNodeWithId(deep);
	for (int i = 0; i < (int)m_vcclass.size(); ++i)
	{
		pNewElement->m_vcclass.push_back(m_vcclass[i]->Clone());
	}
	return pNewElement;
}

bool CSVGElementStyle::Toxml(std::wstring &retstrxml, int nLevel)
{
	if (m_ChildNodes.size() == 0)
	{
		CSVGElement *pElement = getOwnerDocument()->createElement(L"[CDATA[");
		addChild(pElement, true, true);
	}
	if (m_ChildNodes.size() == 1 && m_ChildNodes[0]->getNodeType() == XML_DATA)
	{
		std::wstring strtext;
		strtext += L"<![CDATA[\r\n";
		std::vector<CSVGElementStyleClass*>::iterator iter = m_vcclass.begin();
		for (; iter != m_vcclass.end(); ++iter)
		{
			CSVGElementStyleClass *pClass = *iter;
			if (pClass)
			{
				strtext += L".";
				strtext += pClass->getName();
				strtext += L" {";
				std::map<std::wstring, std::wstring>::iterator mapiter = pClass->m_mapAttr.begin();
				for (; mapiter != pClass->m_mapAttr.end(); ++mapiter)
				{
					if (mapiter->second.length() > 0)
					{
						strtext += mapiter->first;
						strtext += L":";
						strtext += mapiter->second;
						strtext += L";";
					}
				}
				strtext += L"}\r\n";
			}
		}
		strtext += L"]]>\r\n";
		m_ChildNodes[0]->setNodeValue(strtext.c_str());
		m_nodeValue = L"";
	}
	return CSVGElement::Toxml(retstrxml, nLevel);
}

bool CSVGElementStyle::addClass(CSVGElementStyleClass *pClass)
{
	std::wstring strText = pClass->getText();
	for (int i = 0; i < (int)m_vcclass.size(); ++i)
	{
		std::wstring strExistText = m_vcclass[i]->getText();
		if (strText == strExistText)
		{
			return false;
		}
	}
	m_vcclass.push_back(pClass);
	return true;
}
