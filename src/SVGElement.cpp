
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
#include "SVGElement.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "SVGElementStyle.h"

CSVGElement::CSVGElement()
{
	m_nChildCntBuffSize = 0;
	m_parentNode = NULL;
	m_ElementIndex = 0;
	m_pownDocument = NULL;
	m_bShow = true;
	m_bAttrChange = true; m_bLock = false;
	m_bPathHasChanged = true;
}

CSVGElement::CSVGElement(CSVGDocument* ownerDoc, const std::wstring & tagname, unsigned short nodetype)
	:m_pownDocument(ownerDoc)
	, m_nodeType(nodetype)
{
	m_nChildCntBuffSize = 0;
	m_parentNode = NULL;
	m_ElementIndex = 0;
	m_bShow = true;
	m_tagName = tagname;
	m_bAttrChange = true;
	m_bLock = false;
	m_bPathHasChanged = true;
}

CSVGElement::~CSVGElement()
{
	m_pownDocument->RemoveFromSelect(this);
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		delete m_ChildNodes[i];
	}
	m_ChildNodes.clear();
	m_mapAttrValue.clear();
}

unsigned short CSVGElement::getNodeType()
{
	return m_nodeType;
}

std::wstring  CSVGElement::getTagName()
{
	return m_tagName;
}

CSVGElement* CSVGElement::getPreviousSibling()
{
	if (m_parentNode)
	{
		for (int i = 0; i < (int)m_parentNode->m_ChildNodes.size(); ++i)
		{
			if (i != 0 && m_parentNode->m_ChildNodes[i] == this)
			{
				return (CSVGElement*)m_parentNode->m_ChildNodes[i - 1];
			}
		}
	}
	return NULL;
}

CSVGElement* CSVGElement::getNextSibling()
{
	if (m_parentNode)
	{
		for (int i = 0; i < (int)m_parentNode->m_ChildNodes.size(); ++i)
		{
			if (i != (m_parentNode->m_ChildNodes.size() - 1) && m_parentNode->m_ChildNodes[i] == this)
			{
				return (CSVGElement*)m_parentNode->m_ChildNodes[i + 1];
			}
		}
	}
	return NULL;
}

CSVGElement* CSVGElement::getParentNode()
{
	return m_parentNode;
}

std::wstring CSVGElement::getParentId()
{
	if (m_parentNode)
	{
		return m_parentNode->getAttributeId();
	}
	return L"";
}

std::wstring CSVGElement::getPreviousSiblingId()
{
	CSVGElement *pPreviousSibling = getPreviousSibling();
	if (pPreviousSibling)
	{
		return pPreviousSibling->getAttributeId();
	}
	return L"";
}

std::wstring CSVGElement::getNextSiblingId()
{
	CSVGElement* pNextSibling = getNextSibling();
	if (pNextSibling)
	{
		return pNextSibling->getAttributeId();
	}
	return L"";
}

unsigned int CSVGElement::getChildCount()
{
	return m_ChildNodes.size();
}

CSVGElement* CSVGElement::getChild(unsigned int index)
{
	if (index >= 0 && index < m_ChildNodes.size())
	{
		return (CSVGElement*)m_ChildNodes[index];
	}
	return NULL;
}

CSVGElement  * CSVGElement::cloneNodeWithId(bool deep)
{
	CSVGElement *pnewElement = m_pownDocument->createElement(m_tagName);
	std::map<int, std::wstring>::iterator iter = m_mapAttrValue.begin();
	for (; iter != m_mapAttrValue.end(); ++iter)
	{
		std::wstring strname = global.getAttrNameByIndex(iter->first);
		pnewElement->setAttribute(strname, iter->second, false);
	}
	pnewElement->setNodeValue(m_nodeValue, false);
	if (deep)
	{
		for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
		{
			CSVGElement*pNewChild = m_ChildNodes[i]->cloneNodeWithId(deep);
			pnewElement->addChild(pNewChild, true, false);
		}
	}
	return pnewElement;
}

CSVGElement* CSVGElement::cloneNode(bool deep)
{
	CSVGElement *pnewElement = m_pownDocument->createElement(m_tagName);
	pnewElement->m_mapAttrValue = m_mapAttrValue;
	pnewElement->setAttribute(L"id", L"");
	pnewElement->setNodeValue(m_nodeValue);
	if (deep)
	{
		for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
		{
			CSVGElement*pNewChild = m_ChildNodes[i]->cloneNode(deep);
			pnewElement->addChild(pNewChild, true);
		}
	}
	return pnewElement;
}

void  CSVGElement::resetElement()
{
	std::wstring strid = getAttributeId();
	if (strid.length() > 0)
	{
		m_pownDocument->buildIdElement(this, strid);
	}
	if (m_nodeType == SVG_STYLE)
	{
		m_pownDocument->AddStyleElement((CSVGElementStyle*)this);
	}
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->m_pownDocument = m_pownDocument;
		m_ChildNodes[i]->resetElement();
	}
}

bool  CSVGElement::addChild(CSVGElement* newChild, bool bDeep, bool bSetModfiyFlag)
{
	if (newChild)
	{
		if (!m_bShow)
		{
			newChild->m_bShow = false;
		}
		m_ChildNodes.push_back(newChild);
		newChild->m_parentNode = this;
		newChild->m_pownDocument = m_pownDocument;
		newChild->setAttrChange(true);
		if (bDeep)
		{
			this->resetElement();
		}
		if (bSetModfiyFlag)
		{
			SetCheckModify();
		}
		return true;
	}
	return false;
}

bool CSVGElement::pushfront(CSVGElement* newChild, bool bDeep, bool bSetModifyFlag)
{
	if (newChild)
	{
		if (!m_bShow)
		{
			newChild->m_bShow = false;
		}
		m_ChildNodes.insert(m_ChildNodes.begin(), newChild);
		newChild->m_parentNode = this;
		newChild->m_pownDocument = m_pownDocument;
		newChild->setAttrChange(true);
		if (bDeep)
		{
			this->resetElement();
		}
		if (bSetModifyFlag)
		{
			SetCheckModify();
		}
		return true;
	}
	return false;
}

bool CSVGElement::removeChild(CSVGElement* child, bool bSetModfiyFlag)
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i] == child)
		{
			m_pownDocument->removeFromId(child);
			if (child->IsShapeElement())
			{
				m_pownDocument->ReMoveElementFromArea((CSVGElementShape*)child);
			}
			child->m_parentNode = NULL;
			m_ChildNodes.erase(m_ChildNodes.begin() + i);
			setAttrChange(true);
			if (bSetModfiyFlag)
			{
				SetCheckModify();
			}
			return true;
		}
	}
	return false;
}

bool  CSVGElement::removeChildOnly(CSVGElement* child)
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i] == child)
		{
			child->m_parentNode = NULL;
			m_ChildNodes.erase(m_ChildNodes.begin() + i);
			setAttrChange(true);
			SetCheckModify();
			return true;
		}
	}
	return false;
}

bool CSVGElement::deleteChild(CSVGElement* child)
{
	if (child)
	{
		removeChild(child);
		delete child;
		return true;
	}
	return false;
}

void  CSVGElement::deleteAllChild()
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		m_pownDocument->removeFromId(m_ChildNodes[i]);
		delete m_ChildNodes[i];
	}
	m_ChildNodes.clear();
}

void CSVGElement::remove(bool bSetModfiyFlag)
{
	if (m_parentNode)
	{
		if (bSetModfiyFlag)
		{
			SetCheckModify();
		}
		m_parentNode->removeChild(this, bSetModfiyFlag);
	}
}

bool CSVGElement::appendChild(CSVGElement* newChild, bool bSetModfiyFlag)
{
	if (newChild == this)
	{
		return  false;
	}
	if (newChild)
	{
		newChild->remove(bSetModfiyFlag);
	}
	return addChild(newChild, true, bSetModfiyFlag);
}

bool CSVGElement::insertBefore(CSVGElement* newChild, CSVGElement *refChild, bool bDeep, bool bSetModifyFlag)
{
	if (!newChild)
		return false;
	newChild->remove();
	newChild->setAttrChange(true);
	if (!refChild)
	{
		if (m_ChildNodes.size() == 0)
		{
			return addChild(newChild, bDeep);
		}
		else
		{
			m_ChildNodes.insert(m_ChildNodes.begin(), newChild);
			newChild->m_parentNode = this;
		}
	}
	else
	{
		std::vector<CSVGElement*>::iterator iter = m_ChildNodes.begin();
		for (; iter != m_ChildNodes.end(); ++iter)
		{
			if (refChild == *iter)
			{
				m_ChildNodes.insert(iter, newChild);
				newChild->m_parentNode = this;
				break;
			}
		}
	}
	if (bDeep)
	{
		this->resetElement();
	}
	if (bSetModifyFlag)
	{
		SetCheckModify();
	}
	return true;
}

bool CSVGElement::insertAfter(CSVGElement* newChild, CSVGElement *refChild, bool bDeep)
{
	if (!newChild)
		return false;
	newChild->remove();
	newChild->setAttrChange(true);
	if (!refChild)
	{
		addChild(newChild, bDeep);
	}
	else
	{
		std::vector<CSVGElement*>::iterator iter = m_ChildNodes.begin();
		for (; iter != m_ChildNodes.end(); ++iter)
		{
			if (refChild == *iter)
			{
				std::vector<CSVGElement*>::iterator nextIter = iter;
				++nextIter;
				if (nextIter != m_ChildNodes.end())
				{
					m_ChildNodes.insert(nextIter, newChild);
					newChild->m_parentNode = this;
					newChild->m_pownDocument = m_pownDocument;
					SetCheckModify();
				}
				else
				{
					addChild(newChild, bDeep);
				}
				break;
			}
		}
	}
	if (bDeep)
	{
		this->resetElement();
	}
	return true;
}

void CSVGElement::setFloatAttribute(const std::wstring & attrname, float fval, bool bLoad)
{
	if (attrname.length() > 0)
	{
		std::wstring wval = global.ftostring(fval);
		setAttribute(attrname, wval, bLoad);
	}
}

float CSVGElement::getFloatAttribute(const std::wstring & attrname)
{
	std::wstring AttrVal = getAttribute(attrname);
	if (AttrVal.length() > 0)
	{
		return global.atof_3(AttrVal);
	}
	return 0;
}

int CSVGElement::getIntAttribute(const std::wstring & attrname)
{
	std::wstring AttrVal = getAttribute(attrname);
	if (AttrVal.length() > 0)
	{
		return global.strtoi(AttrVal);
	}
	return -1;
}

bool CSVGElement::IsStyleAttr(const std::wstring &attrname)
{
	unsigned short nIndex = global.getIndexByAttrName(attrname);
	return 	global.IsStyleAttr(nIndex);
}

void CSVGElement::setAttribute(const std::wstring &attrname, const std::wstring & value, bool bSetModfiyFlag)
{
	int nIndex = global.getIndexByAttrName(attrname);
	std::map<int, std::wstring>::iterator iter = m_mapAttrValue.find(nIndex);
	if (iter == m_mapAttrValue.end())
	{
		m_mapAttrValue[nIndex] = value;
	}
	else
	{
		iter->second = value;
	}
	if (attrname.length() == 2 && attrname == L"id")
	{
		m_id = value;
		m_pownDocument->buildIdElement(this, value);
	}
	m_strXml.clear();
	if (bSetModfiyFlag)
	{
		SetCheckModify();
		setAttrChange(true);
	}
}

std::wstring CSVGElement::getAttributeId()
{
	return getAttribute(L"id");
}

std::wstring CSVGElement::getAttribute(const std::wstring & attrname)
{
	int nIndex = global.getIndexByAttrName(attrname);
	std::map<int, std::wstring>::iterator iter = m_mapAttrValue.find(nIndex);
	if (iter != m_mapAttrValue.end())
	{
		return iter->second;
	}
	return L"";
}

bool CSVGElement::delAttribute(const std::wstring &attrname)
{
	int nIndex = global.getIndexByAttrName(attrname);
	m_mapAttrValue.erase(nIndex);
	m_strXml = L"";
	return true;
}

bool CSVGElement::delAllAttribute()
{
	m_mapAttrValue.clear();
	m_strXml = L"";
	return true;
}

std::wstring  CSVGElement::getNodeValue()
{
	return m_nodeValue;
}


void  CSVGElement::setNodeValue(const std::wstring &value, bool bSetModfiyFlag)
{
	m_nodeValue = value;
	setAttrChange(true);
	m_strXml.clear();
	if (bSetModfiyFlag)
	{
		SetCheckModify();
	}
}

void CSVGElement::SetCheckModify()
{
	m_pownDocument->SetModify(true);
}

void CSVGElement::setParentAttrChange()
{
	if (m_parentNode)
	{
		m_parentNode->m_bAttrChange = true;
		m_parentNode->setParentAttrChange();
	}
}

void CSVGElement::setChildAttrChange()
{
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->m_bAttrChange = true;
		m_ChildNodes[i]->setChildAttrChange();
	}
}

void CSVGElement::setAttrChange(bool bChange)
{
	m_bAttrChange = bChange;
	if (m_bAttrChange)
	{
		setChildAttrChange();
	}
}

bool CSVGElement::IsAttrChange()
{
	return m_bAttrChange;
}

CSVGDocument* CSVGElement::getOwnerDocument()
{
	return m_pownDocument;
}

CSVGElement* CSVGElement::getRootSVGElement()
{
	CSVGElement *pParent = this;
	while (pParent)
	{
		if (pParent->getNodeType() == SVG_SVG)
		{
			return pParent;
		}
		pParent = pParent->getParentNode();
	}
	return NULL;
}

CSVGElementStyle * CSVGElement::GetOrNewStyle()
{
	CSVGElementStyle *pStyle = NULL;
	CSVGElement *pRoot = getRootSVGElement();
	if (pRoot)
	{
		for (int i = 0; i < (int)pRoot->getChildCount(); ++i)
		{
			CSVGElement* pChild = pRoot->getChild(i);
			if (pChild->getNodeType() == SVG_STYLE)
			{
				pStyle = (CSVGElementStyle *)pChild;
				break;
			}
		}
		pStyle = (CSVGElementStyle*)m_pownDocument->createElement(L"style");
		pRoot->insertBefore(pStyle, NULL, false);
	}
	return pStyle;
}

std::wstring   CSVGElement::getAttrxml()
{
	if (m_strXml.length() == 0)
	{
		m_strXml = L"<";
		m_strXml += m_tagName;
		std::wstring xmlattr;
		std::map<int, std::wstring>::iterator iter = m_mapAttrValue.begin();
		for (; iter != m_mapAttrValue.end(); ++iter)
		{
			std::wstring strName = global.getAttrNameByIndex(iter->first);
			std::wstring strValue = iter->second;
			if (strName.length() > 0 && strValue.length() > 0)
			{
				xmlattr += L" ";
				xmlattr += strName;
				xmlattr += L"=\"";
				xmlattr += strValue;
				xmlattr += L"\"";
			}
		}
		if (xmlattr.length() > 0)
		{
			m_strXml += xmlattr;
		}
	}
	return m_strXml;
}

void  CSVGElement::AddLevelSpace(std::wstring &strText, int nlevel)
{
	if (nlevel == 1)
	{
		strText += L" ";
	}
	else if (nlevel == 2)
	{
		strText += L"  ";
	}
	else if (nlevel == 3)
	{
		strText += L"   ";
	}
	else if (nlevel == 4)
	{
		strText += L"    ";
	}
	else if (nlevel == 5)
	{
		strText += L"     ";
	}
	else if (nlevel == 6)
	{
		strText += L"      ";
	}
	else
	{
		for (int i = 0; i < nlevel; ++i)
		{
			strText += L" ";
		}
	}
}

bool CSVGElement::Toxml(std::wstring &retstrxml, int nLevel)
{
	if (m_strXml.length() == 0)
	{
		getAttrxml();
	}
	if (m_nodeType != SVG_TSPAN && m_nodeType != SVG_TEXTCONTENT)
	{
		AddLevelSpace(retstrxml, nLevel);
	}
	retstrxml += m_strXml;
	if (m_ChildNodes.size() == 0 && m_nodeValue.length() == 0)
	{
		retstrxml += L"/>\n";
	}
	else
	{
		retstrxml += L">";
		if (m_nodeValue.length() > 0)
		{
			retstrxml += m_nodeValue;
		}
		for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
		{
			CSVGElement *pChild = m_ChildNodes[i];
			pChild->Toxml(retstrxml, nLevel + 1);
		}
		if (m_nodeType != SVG_TEXT
			&& m_nodeType != SVG_TSPAN)
		{
			AddLevelSpace(retstrxml, nLevel);
		}
		retstrxml += L"</";
		retstrxml += m_tagName;
		retstrxml += L">\n";
	}
	return true;
}

std::set<std::wstring> CSVGElement::getAttrNames()
{
	std::set<std::wstring> vcAttrName;
	std::map<int, std::wstring>::iterator iter = m_mapAttrValue.begin();
	for (; iter != m_mapAttrValue.end(); ++iter)
	{
		std::wstring strName = global.getAttrNameByIndex(iter->first);
		if (strName.length() > 0)
		{
			vcAttrName.insert(strName);
		}
	}
	return vcAttrName;
}

CSVGElement* CSVGElement::getprenode()
{
	CSVGElement* preSibling = getPreviousSibling();
	CSVGElement *preNode = preSibling;
	while (preNode && (preNode->m_nodeType == SVG_G || preNode->m_nodeType == SVG_SVG))
	{
		std::vector<CSVGElement*>childnodes = preNode->m_ChildNodes;
		if (childnodes.size() > 0)
		{
			preNode = childnodes[childnodes.size() - 1];
		}
		else
		{
			break;
		}
	}
	if (!preNode)
		preNode = m_parentNode;
	return preNode;
}

CSVGElement* CSVGElement::getnextnode()
{
	CSVGElement *pnextnode = NULL;
	if (m_nodeType == SVG_G)
	{
		if (m_ChildNodes.size() > 0)
		{
			pnextnode = m_ChildNodes[0];
		}
	}
	if (!pnextnode)
	{
		pnextnode = getNextSibling();
	}
	CSVGElement *pParent = m_parentNode;
	while (!pnextnode && pParent)
	{
		pnextnode = pParent->getNextSibling();
		pParent = pParent->m_parentNode;
	}
	return pnextnode;
}

bool CSVGElement::IsShow()
{
	return m_bShow;
}

void CSVGElement::SetShow(bool bShow)
{
	m_bShow = bShow;
}

void CSVGElement::ClearXML()
{
	m_strXml.clear();
}

void  CSVGElement::ChangeFillIds(std::map<std::wstring, std::wstring>&idmap)
{
	if (IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)this;
		std::wstring strfill = pShape->getUseStyle(L"fill");
		std::wstring fillid = global.UrlStringGetId(strfill);
		if (idmap.size() > 0)
		{
			std::map<std::wstring, std::wstring>::iterator iter = idmap.find(fillid);
			if (iter != idmap.end())
			{
				std::wstring stridtemp = L"url(#" + iter->second + L")";
				setAttribute(L"fill", stridtemp.c_str());
			}
		}
	}
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->ChangeFillIds(idmap);
	}
}

void CSVGElement::pathHasChange()
{
	m_bPathHasChanged = true;
	CSVGElement* pParent = getParentNode();
	while (pParent)
	{
		pParent->pathHasChange();
		pParent = pParent->getParentNode();
	}
}

void CSVGElement::resetUseMatrix(CSVGElement *pParent)
{
	m_usedmatrix = CSVGMatrix();
	if (pParent)
	{
		m_usedmatrix = pParent->m_usedmatrix;
	}
	else if (m_parentNode)
	{
		m_usedmatrix = m_parentNode->m_usedmatrix;
	}
	std::wstring strmat = getAttribute(L"transform");
	CSVGMatrix selfmat;
	selfmat.setText(strmat);
	m_usedmatrix.Add(selfmat);
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->resetUseMatrix();
	}
}

CSVGMatrix CSVGElement::getMatrix()
{
	std::wstring strmat = getAttribute(L"transform");
	CSVGMatrix selfmat;
	selfmat.setText(strmat);
	return selfmat;
}

CSVGMatrix CSVGElement::DoMatrixTranslate(CSVGMatrix Newmat)
{
	CSVGMatrix mat;
	mat.Add(Newmat);
	mat.Add(getMatrix());
	return mat;
}

CSVGMatrix  CSVGElement::getParentMatrix()
{
	if (m_parentNode && m_parentNode)
	{
		return (m_parentNode->m_usedmatrix);
	}
	return CSVGMatrix::getNullMatrix();
}

CSVGMatrix CSVGElement::getUsedMatrix()
{
	return m_usedmatrix;
}

void CSVGElement::GetGroupElement(CSVGElement *&pGroup)
{
	CSVGElement* pParentNode = getParentNode();
	if (pParentNode && pParentNode->IsShapeElement())
	{
		if (pParentNode->IsShapeElement()
			&& pParentNode->getNodeType() == SVG_G)
		{
			if (pParentNode->getNodeType() != SVG_SVG)
			{
				pGroup = pParentNode;
				((CSVGElementShape*)pParentNode)->GetGroupElement(pGroup);
			}
		}
	}
}

CSVGElement * CSVGElement::GetTopGroupElement()
{
	CSVGElement *pShape = NULL;
	GetGroupElement(pShape);
	return pShape;
}

void CSVGElement::AddToLabelName(std::set<std::wstring>&vcname)
{
	vcname.insert(m_tagName);
	std::map<int, std::wstring>::iterator iter = m_mapAttrValue.begin();
	for (; iter != m_mapAttrValue.end(); ++iter)
	{
		std::wstring strName = global.getAttrNameByIndex(iter->first);
		vcname.insert(strName);
	}
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->AddToLabelName(vcname);
	}
}

void CSVGElement::GetAllElementFlat(std::vector<CSVGElement*>&vcElement, bool bIncludeself)
{
	if (bIncludeself)
	{
		vcElement.push_back(this);
	}
	for (int i = 0; i < (int)m_ChildNodes.size(); ++i)
	{
		m_ChildNodes[i]->GetAllElementFlat(vcElement);
	}
}

void CSVGElement::SetLock(bool bLock)
{
	m_bLock = bLock;
}

bool CSVGElement::IsLock()
{
	return m_bLock;
}

void CSVGElement::RemoveChildByTagNames(std::vector<std::wstring>&tagNames)
{
	for (int i = m_ChildNodes.size() - 1; i >= 0; --i)
	{
		std::wstring strTagName = m_ChildNodes[i]->getTagName();
		for (int j = 0; j < (int)tagNames.size(); ++j)
		{
			if (strTagName == tagNames[j])
			{
				delete m_ChildNodes[i];
				m_ChildNodes.erase(m_ChildNodes.begin() + i);
			}
		}
	}
	for (int i = m_ChildNodes.size() - 1; i >= 0; --i)
	{
		m_ChildNodes[i]->RemoveChildByTagNames(tagNames);
	}
}

void CSVGElement::RemoveAttrByName(std::vector<std::wstring>&attrNames)
{
	for (int i = 0; i < (int)attrNames.size(); ++i)
	{
		std::wstring stroldval = getAttribute(attrNames[i]);
		if (stroldval.length() > 0)
		{
			setAttribute(attrNames[i], L"");
		}
	}
	for (int i = m_ChildNodes.size() - 1; i >= 0; --i)
	{
		m_ChildNodes[i]->RemoveAttrByName(attrNames);
	}
}

void CSVGElement::RemoveAttrByKeyName(std::vector<std::wstring>&attrKeyNames)
{
	std::set<std::wstring>vcAttr = getAttrNames();
	std::set<std::wstring>::iterator iter = vcAttr.begin();
	for (; iter != vcAttr.end(); ++iter)
	{
		std::wstring strName = *iter;
		for (int i = 0; i < (int)attrKeyNames.size(); ++i)
		{
			if (strName.find(attrKeyNames[i]) != std::wstring::npos)
			{
				setAttribute(strName, L"");
			}
		}
	}
	for (int i = m_ChildNodes.size() - 1; i >= 0; --i)
	{
		m_ChildNodes[i]->RemoveAttrByKeyName(attrKeyNames);
	}
}

void CSVGElement::DeclineMoreClass()
{
	std::vector<CSVGElement*>vcElement;
	GetAllElementFlat(vcElement);
	std::vector<CSVGElementStyle*>vcStyles;
	for (int i = 0; i < (int)vcElement.size(); ++i)
	{
		if (vcElement[i]->getNodeType() == SVG_STYLE)
		{
			vcStyles.push_back((CSVGElementStyle*)vcElement[i]);
		}
	}
	if (vcStyles.size() >= 2)
	{
		CSVGElementStyle *pFirstStyle = vcStyles[0];
		for (int i = 1; i < (int)vcStyles.size(); ++i)
		{
			std::vector<std::wstring> vcClassName = vcStyles[i]->GetClassNames();
			for (int j = 0; j < (int)vcClassName.size(); ++j)
			{
				if (!pFirstStyle->getClassByName(vcClassName[j]))
				{
					CSVGElementStyleClass * pExistClass = vcStyles[i]->getClassByName(vcClassName[j]);
					CSVGElementStyleClass *pNewClass = pExistClass->Clone();
					pFirstStyle->addClass(pNewClass);
				}
			}
			vcStyles[i]->remove(false);
			delete vcStyles[i];
		}
	}
}

void CSVGElement::SetTwoPrecision()
{
	std::wstring strmat = getAttribute(L"transform");
	if (strmat.length() > 0)
	{
		CSVGMatrix mat;
		mat.setText(strmat);
		strmat = mat.getText();
		setAttribute(L"transform", strmat);
	}
	for (int i = m_ChildNodes.size() - 1; i >= 0; --i)
	{
		m_ChildNodes[i]->SetTwoPrecision();
	}
}
