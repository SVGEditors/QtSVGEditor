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
#include "ElementMaker.h"
#include <QtXml>
#include "SVGGlobal.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"

CXMLMakerNode::CXMLMakerNode()
{

}

CXMLMakerNode::CXMLMakerNode(std::wstring strTagName)
{
	m_strTagName = strTagName;
}

void CXMLMakerNode::SetTagName(std::wstring strTagName)
{
	m_strTagName = strTagName;
}

std::wstring CXMLMakerNode::GetTagName()
{
	return m_strTagName;
}

std::wstring CXMLMakerNode::GetXML()
{
	std::wstring strXml = L"<" + m_strTagName;
	for (int i = 0; i < (int)m_vcAttrName.size(); ++i)
	{
		if (m_vcAttrName[i] == L"boxx"
			|| m_vcAttrName[i] == L"boxy"
			|| m_vcAttrName[i] == L"boxwidth"
			|| m_vcAttrName[i] == L"boxheight")
		{
			continue;
		}
		strXml += L" ";
		strXml += m_vcAttrName[i];
		strXml += L"=\"";
		strXml += m_vcAttrVal[i];
		strXml += L"\"";
	}
	if (m_strNodeVal.length() > 0)
	{
		strXml += L">";
		strXml += m_strNodeVal;
	}
	else
	{
		strXml += L">\n";
	}
	for (int i = 0; i < m_vcNode.size(); ++i)
	{
		strXml += m_vcNode[i].GetXML();
	}
	strXml += L"</" + m_strTagName + L">\n";
	return strXml;

	return strXml;
}

std::wstring CXMLMakerNode::GetJSON(bool bRoot)
{
	std::wstring str;
	if (bRoot)
	{
		str += L"{\n";
	}
	else
	{
		str += L"\"" + m_strTagName + L"\":{\n";
	}
	for (size_t i = 0; i < m_vcAttrName.size(); ++i)
	{
		if (i != 0)
		{
			str += L",\n";
		}
		str += L"\"@" + m_vcAttrName[i] + L"\":\"" + m_vcAttrVal[i] + L"\"";
	}
	if (m_strNodeVal.length() > 0)
	{
		str += L",\n";
		str += L"\"#text\":";
	}

	for (int i = 0; i < m_vcNode.size(); ++i)
	{
		str += m_vcNode[i].GetJSON(false);
	}
	str += L"}\n";
	return str;
}

void CXMLMakerNode::CreateRectFocus(bool bChild, std::wstring fx, std::wstring fy, std::wstring fill, std::wstring ownerid
	, std::wstring strid, std::wstring index)
{
	if (bChild)
	{
		SetAttribute(L"x", global.ftostring(global.strtoi(fx) - 3));
		SetAttribute(L"y", global.ftostring(global.strtoi(fy) - 3));
		SetAttribute(L"width", global.ftostring(6));
		SetAttribute(L"height", global.ftostring(6));
	}
	else
	{
		SetAttribute(L"x", global.ftostring(global.strtoi(fx) - 5));
		SetAttribute(L"y", global.ftostring(global.strtoi(fy) - 5));
		SetAttribute(L"width", global.ftostring(10));
		SetAttribute(L"height", global.ftostring(10));
	}

	SetAttribute(L"stroke", L"#000000");
	SetAttribute(L"fill", fill);
	SetAttribute(L"stroke-opacity", L"1");
	SetAttribute(L"stroke-width", L"1");
	SetAttribute(L"fill-opacity", L"1");
	SetAttribute(L"id", strid);
	SetAttribute(L"ownerid", ownerid);
	SetAttribute(L"index", index);
	SetAttribute(L"isfocus", L"true");
	SetAttribute(L"nocheckmodify", L"true");
}

void CXMLMakerNode::SetAttribute(std::wstring strAtt, std::wstring strVal)
{
	for (int i = 0; i < (int)m_vcAttrName.size(); ++i)
	{
		if (m_vcAttrName[i] == strAtt)
		{
			m_vcAttrName.erase(m_vcAttrName.begin() + i);
			m_vcAttrVal.erase(m_vcAttrVal.begin() + i);
			break;
		}
	}
	m_vcAttrName.push_back(strAtt);
	m_vcAttrVal.push_back(strVal);
}

std::wstring CXMLMakerNode::GetAttribute(std::wstring strAtt)
{
	if (m_vcAttrName.size() == m_vcAttrVal.size())
	{
		for (int i = 0; i < (int)m_vcAttrName.size(); ++i)
		{
			if (m_vcAttrName[i] == strAtt)
			{
				return m_vcAttrVal[i];
			}
		}
	}
	return L"";
}

float CXMLMakerNode::getFloatAttribute(std::wstring strName)
{
	std::wstring strValue = GetAttribute(strName);
	return global.atof_3(strValue);
}

void CXMLMakerNode::GetAttribute(std::vector<std::wstring>&vcAttr, std::vector<std::wstring>&vcVal)
{
	if (m_vcAttrName.size() == m_vcAttrVal.size())
	{
		vcAttr = m_vcAttrName;
		vcVal = m_vcAttrVal;
	}
}

void CXMLMakerNode::SetNodeValueAsCDATA(std::wstring strValue)
{
	QString str = QString::fromStdWString(strValue);
	if (str.indexOf("<![CDATA[") >= 0)
	{
		str = str.replace("<![CDATA[", "TUYEDATABEGIN");
		str = str.replace("]]>", "TUYEDATAEND");
		strValue = str.toStdWString();
	}
	m_strNodeVal = L"<![CDATA[" + strValue + L"]]>";
}

void CXMLMakerNode::SetNodeValue(std::wstring strNodeValue)
{
	m_strNodeVal = strNodeValue;
}

std::wstring CXMLMakerNode::GetNodeValue()
{
	QString strValue = QString::fromStdWString(m_strNodeVal);
	strValue = strValue.replace("TUYEDATABEGIN", "<![CDATA[");
	strValue = strValue.replace("TUYEDATAEND", "]]>");
	if (strValue.indexOf("<![CDATA[") == 0)
	{
		strValue = strValue.right(strValue.length() - strlen("<![CDATA["));
		strValue = strValue.left(strValue.length() - strlen("]]>"));
	}
	return strValue.toStdWString();
}

CSVGElement *CXMLMakerNode::CreateElement(CSVGDocument *pOwnerDoc)
{
	CSVGElement *pElement = pOwnerDoc->createElement(m_strTagName);
	if (pElement)
	{
		for (int i = 0; i < (int)m_vcAttrName.size(); ++i)
		{
			pElement->setAttribute(m_vcAttrName.at(i), m_vcAttrVal.at(i), true);
		}
		if (m_strNodeVal.length() > 0)
		{
			pElement->setNodeValue(m_strNodeVal, true);
		}
		for (int i = 0; i < m_vcNode.size(); ++i)
		{
			CSVGElement *pChildElement = m_vcNode[i].CreateElement(pOwnerDoc);
			pElement->addChild(pChildElement, true, true);
		}
	}
	return pElement;
}

void CXMLMakerNode::LoadFromXml(std::wstring strXml)
{
	QDomDocument doc;
	if (doc.setContent(QString::fromStdWString(strXml)))
	{
		QDomNode root = doc.documentElement();
		QDomElement node = root.toElement();
		ParserElement(node);
	}
}

void CXMLMakerNode::LoadFromJSON(std::wstring strjson)
{

}

void CXMLMakerNode::ParserElement(QDomElement &node)
{
	m_strTagName = node.nodeName().toStdWString();
	QDomNamedNodeMap attrmap = node.attributes();
	for (int i = 0; i < attrmap.length(); ++i)
	{
		std::wstring strNodeName = attrmap.item(i).nodeName().toStdWString();
		std::wstring strNodeVal = attrmap.item(i).nodeValue().toStdWString();
		m_vcAttrName.push_back(strNodeName);
		m_vcAttrVal.push_back(strNodeVal);
	}
	QDomNode child = node.firstChild();
	QDomNode::NodeType nodetype = child.nodeType();
	if (nodetype == QDomNode::CDATASectionNode)
	{
		std::wstring strtemp = child.nodeValue().toStdWString();
		m_strNodeVal = L"<![CDATA[" + strtemp + L"]]>";
	}
	else if (nodetype == QDomNode::TextNode)
	{
		m_strNodeVal = node.text().toStdWString();
	}
	QDomNodeList childlist = node.childNodes();
	for (int i = 0; i < childlist.size(); ++i)
	{
		CXMLMakerNode makenode;
		QDomElement element = childlist.at(i).toElement();
		if (element.nodeName().length() > 0)
		{
			QDomElement node = childlist.at(i).toElement();
			makenode.ParserElement(node);
			m_vcNode.push_back(makenode);
		}
	}
}

void CXMLMakerNode::FromSVGElement(CSVGElement *pElement)
{
	if (pElement)
	{
		std::wstring strxml;
		pElement->Toxml(strxml, 0);
		LoadFromXml(strxml);
	}
}

void CXMLMakerNode::FromSVGElementWithBBox(CSVGElement *pElement)
{
	if (!pElement)
		return;
	FromSVGElement(pElement);
	if (pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF bbox = pShape->getBBox();
		SetAttribute(L"boxx", global.ftostring(bbox.x));
		SetAttribute(L"boxy", global.ftostring(bbox.y));
		SetAttribute(L"boxwidth", global.ftostring(bbox.width));
		SetAttribute(L"boxheight", global.ftostring(bbox.height));
	}
}

CXMLMakerNode *CXMLMakerNode::GetElemnetById(std::wstring strid)
{
	for (int i = 0; i < (int)m_vcAttrName.size(); ++i)
	{
		if (m_vcAttrName[i] == L"id" &&m_vcAttrVal[i] == strid)
		{
			return this;
		}
	}
	CXMLMakerNode * pNode = NULL;
	for (int i = 0; i < m_vcNode.size(); ++i)
	{
		pNode = m_vcNode[i].GetElemnetById(strid);
		if (pNode)
		{
			break;
		}
	}
	return pNode;
}

void CXMLMakerNode::AddChild(CXMLMakerNode &node)
{
	m_vcNode.push_back(node);
}

void CXMLMakerNode::FindParent(CXMLMakerNode *pChild, bool &bFind, QVector<CXMLMakerNode>&vcParent)
{
	if (this == pChild)
	{
		bFind = true;
		vcParent.push_back(*this);
		return;
	}
	for (int i = 0; i < m_vcNode.size(); ++i)
	{
		m_vcNode[i].FindParent(pChild, bFind, vcParent);
		if (bFind)
		{
			break;
		}
	}
	if (bFind)
	{
		vcParent.push_front(*this);
	}
}

std::wstring CXMLMakerNode::GetChildMatrix(CXMLMakerNode *pChild)
{
	bool bFind = false;
	QVector<CXMLMakerNode> vcParent;
	FindParent(pChild, bFind, vcParent);
	CSVGMatrix retmat;
	for (int i = 0; i < vcParent.size(); ++i)
	{
		std::wstring strmat = vcParent[i].GetAttribute(L"transform");
		CSVGMatrix mat;
		mat.setText(strmat);
		retmat.Add(mat);
	}
	return retmat.getText();
}
