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
#include <QVector>
#include <vector>
#include <QDomElement>

class CSVGElement;
class CSVGDocument;
class CXMLMakerNode
{
public:
	CXMLMakerNode();
	CXMLMakerNode(std::wstring strTagName);
	void SetTagName(std::wstring strTagName);
	std::wstring GetTagName();
	void LoadFromXml(std::wstring strXml);
	void LoadFromJSON(std::wstring strjson);
	void FromSVGElement(CSVGElement *pElement);
	void FromSVGElementWithBBox(CSVGElement *pElement);
	std::wstring GetXML();
	std::wstring GetJSON(bool bRoot = true);
	void SetAttribute(std::wstring strAtt, std::wstring strVal);
	std::wstring GetAttribute(std::wstring strAtt);
	void GetAttribute(std::vector<std::wstring>&vcAttr, std::vector<std::wstring>&vcVal);
	void CreateRectFocus(bool bChild, std::wstring fx, std::wstring fy, std::wstring fill, std::wstring ownerid, std::wstring strid, std::wstring index);
	void SetNodeValueAsCDATA(std::wstring strValue);
	std::wstring GetNodeValue();
	void SetNodeValue(std::wstring strNodeValue);
	float getFloatAttribute(std::wstring strName);
	CSVGElement *CreateElement(CSVGDocument *pOwnerDoc);
	CXMLMakerNode *GetElemnetById(std::wstring strid);
	void  AddChild(CXMLMakerNode &node);
	std::wstring GetChildMatrix(CXMLMakerNode *pChild);
private:
	void FindParent(CXMLMakerNode *pChild, bool &bFind, QVector<CXMLMakerNode>&vcParent);
public:
	QVector<CXMLMakerNode>m_vcNode;
private:
	void ParserElement(QDomElement &node);
	std::wstring m_strTagName;
	std::wstring m_strNodeVal;
	std::vector<std::wstring> m_vcAttrName;
	std::vector<std::wstring> m_vcAttrVal;
};
