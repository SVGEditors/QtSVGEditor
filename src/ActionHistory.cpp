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
// ActionHistory.cpp : implementation file

#include "ActionHistory.h"
#include <set>
#include <QDir>
#include "SVGGlobal.h"
#include "main.h"
#include "SVGDocument.h"
#include "SVGElementShape.h"


CActionXMLNode::CActionXMLNode()
{
	m_ActionType = ACTION_XMLNODE;
}

CActionXMLNode::~CActionXMLNode()
{
}


void CActionXMLNode::AddAction(QVector<CXMLMakerNode> &undoDatas, QVector<CXMLMakerNode>&redoDatas)
{
	m_undodatas.clear();
	m_redodatas.clear();
	m_undodatas = undoDatas;
	m_redodatas = redoDatas;
}

void CActionXMLNode::GetAttrValueFromParam(std::wstring &strParam, std::vector<std::wstring>& AttrNames, std::vector<std::wstring> &AttrVals)
{
	std::vector<std::wstring> vcArr = global.tokenize(strParam, L";");
	for (size_t i = 0; i < vcArr.size(); ++i)
	{
		std::vector<std::wstring> vcPair = global.tokenize(vcArr[i], L"|");
		if (vcPair.size() >= 1)
		{
			AttrNames.push_back(vcPair[0]);
			if (vcPair.size() >= 2)
			{
				AttrVals.push_back(vcPair[1]);
			}
			else
			{
				AttrVals.push_back(L"");
			}
		}
	}
}


void CActionXMLNode::ChangeRedoData(QVector<CXMLMakerNode>&redoDatas)
{
	m_redodatas = redoDatas;

	std::vector<std::wstring> vcRedoName;
	std::vector<std::wstring> vcRedoValue;
	std::set<std::wstring> redoset;
	for (int i = 0; i < m_redodatas.size(); ++i)
	{
		std::wstring strproperty = m_redodatas[i].GetAttribute(L"property");
		GetAttrValueFromParam(strproperty, vcRedoName, vcRedoValue);
	}
	for (size_t i = 0; i < vcRedoName.size(); ++i)
	{
		redoset.insert(vcRedoName[i]);
	}
	for (int i = 0; i < m_undodatas.size(); ++i)
	{
		std::vector<std::wstring> vcUndoName;
		std::vector<std::wstring> vcUndoValue;
		std::set<std::wstring> undoset;
		std::wstring strproperty = m_undodatas[i].GetAttribute(L"property");
		GetAttrValueFromParam(strproperty, vcUndoName, vcUndoValue);
		for (size_t i = 0; i < vcUndoName.size(); ++i)
		{
			undoset.insert(vcUndoName[i]);
		}
		std::wstring strNewProperty;
		std::set<std::wstring>::iterator iter = redoset.begin();
		for (; iter != redoset.end(); ++iter)
		{
			if (undoset.find(*iter) == undoset.end())
			{
				strNewProperty += L";";
				strNewProperty += *iter;
				strNewProperty += L"|";
			}
		}
		strproperty += strNewProperty;
		if (strproperty.length() > 0)
		{
			m_undodatas[i].SetAttribute(L"property", strproperty);
		}
	}
}

QVector<CXMLMakerNode> CActionXMLNode::GetRedoData()
{
	return m_redodatas;
}

std::wstring CActionXMLNode::Undo()
{
	std::wstring strxml;
	CXMLMakerNode rootnode(L"svgsvg");
	rootnode.SetAttribute(L"selectelementids", L"");
	rootnode.SetAttribute(L"editingfocusid", L"");
	rootnode.m_vcNode = m_undodatas;
	strxml = rootnode.GetXML();
	return strxml;
}

std::wstring CActionXMLNode::Redo()
{
	std::wstring strxml;
	CXMLMakerNode rootnode(L"svgsvg");
	rootnode.SetAttribute(L"selectelementids", L"");
	rootnode.SetAttribute(L"editingfocusid", L"");
	rootnode.m_vcNode = m_redodatas;
	strxml = rootnode.GetXML();
	return strxml;
}
//////////////////////////////////////////////////////////////////////////
CActionHistory::CActionHistory()
	: m_nPos(0)
{
}

CActionHistory::~CActionHistory()
{
	ClearHistory();
}

void CActionHistory::ClearAll()
{
	ClearHistory();
}

void CActionHistory::AddAction(CActionBase* NewAction)
{
	if (m_nPos < 0)
		m_nPos = (int)m_arActions.size() - 1;
	if (m_nPos > m_arActions.size())
		m_nPos = m_arActions.size();
	ClearEnd();
	m_arActions.push_back(NewAction);
	m_nPos = m_arActions.size();
}

CActionXMLNode *CActionHistory::GetLastComposite()
{
	if (m_arActions.size() > 0)
	{
		CActionBase *pAction = m_arActions[m_arActions.size() - 1];
		if (pAction->m_ActionType == ACTION_XMLNODE)
		{
			return (CActionXMLNode *)pAction;
		}
	}
	return NULL;
}

void CActionHistory::ClearHistory()
{
	for (std::vector<CActionBase*>::iterator itAction = m_arActions.begin();
		itAction < m_arActions.end(); itAction++)
		delete *itAction;
	m_arActions.clear();

	m_nPos = 0;
}

std::wstring CActionHistory::Undo()
{
	std::wstring strxml;
	if (m_nPos <= 0 || m_nPos - 1 > m_arActions.size())
		return strxml;

	m_nPos--;

	strxml = m_arActions[m_nPos]->Undo();
	return strxml;
}

void CActionHistory::popback()
{
	if (m_nPos <= 0 || m_nPos - 1 > m_arActions.size())
		return;
	m_nPos--;
	ClearEnd();
}

CActionBase *CActionHistory::back()
{
	if (m_arActions.size() > 0)
	{
		CActionBase *pAction = m_arActions[m_arActions.size() - 1];
		if (pAction->m_ActionType == 1)
		{
			return pAction;
		}
	}
	return NULL;
}

std::wstring CActionHistory::Redo()
{
	std::wstring strxml;
	if (m_nPos < 0 || m_nPos >= m_arActions.size())
		return strxml;

	strxml = m_arActions[m_nPos]->Redo();
	m_nPos++;
	return strxml;
}

void CActionHistory::ClearEnd()
{
	std::vector<CActionBase*>::iterator itStartAction = m_arActions.begin() + m_nPos;
	std::vector<CActionBase*>::iterator itEndAction = m_arActions.end();
	for (std::vector<CActionBase*>::iterator itAction = itStartAction;
		itAction < itEndAction; itAction++)
		delete *itAction;
	m_arActions.erase(itStartAction, itEndAction);
}


bool CActionHistory::CanUndo()
{
	if (m_nPos <= 0 || m_nPos - 1 > m_arActions.size())
		return false;
	return true;
}

bool CActionHistory::CanRedo()
{
	if (m_nPos < 0 || m_nPos >= m_arActions.size())
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CActionComposite::CActionComposite(CSVGDocument *pIDoc)
{
	m_ActionType = ACTION_COMPOSITE;
	m_pDocument = pIDoc;
}

CActionComposite::~CActionComposite()
{
	for (size_t i = 0; i < m_Actions.size(); ++i)
	{
		delete m_Actions[i];
	}
	m_Actions.clear();
}

std::wstring CActionComposite::Undo()
{
	for (int i = m_Actions.size() - 1; i >= 0; --i)
	{
		m_Actions[i]->Undo();
	}
	return L"";
}

std::wstring CActionComposite::Redo()
{
	for (size_t i = 0; i < m_Actions.size(); ++i)
	{
		m_Actions[i]->Redo();
	}
	return L"";
}

void CActionComposite::PushBack(CActionBase *pAction)
{
	m_Actions.push_back(pAction);
}

std::vector<CActionBase*>& CActionComposite::GetActions()
{
	return m_Actions;
}


CActionAdd::CActionAdd(CSVGDocument *pIDoc, CSVGElement *pParentElement, CSVGElement *pShapeElement)
	:m_pRemoveElement(NULL)
{
	m_ActionType = ACTION_ADD;
	m_pDocument = pIDoc;
	m_pParentElement = pParentElement;
	m_pShapeElement = pShapeElement;
}

CActionAdd::~CActionAdd()
{
	if (m_pRemoveElement)
	{
		delete m_pRemoveElement;
	}
}

std::wstring CActionAdd::Undo()
{
	if (m_pShapeElement)
	{
		m_pDocument->RemoveFromSelect(m_pShapeElement);
		m_pDocument->SetContentHasChange();
		m_pShapeElement->remove();
		m_pRemoveElement = m_pShapeElement;
	}
	return L"";
}

std::wstring CActionAdd::Redo()
{
	if (m_pShapeElement)
	{
		m_pParentElement->addChild(m_pShapeElement, true);
		m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
		m_pDocument->SetContentHasChange();
		m_pRemoveElement = NULL;
	}
	return L"";
}

//////////////////////////////////////////////////////////////////////////
CActionAttr::CActionAttr(CSVGDocument *pIDoc, CSVGElement *pShapeElement, std::wstring attrname)
{
	m_ActionType = ACTION_ATTR;
	m_pDocument = pIDoc;
	m_pShapeElement = pShapeElement;
	m_attrname = attrname;
	m_preval = m_pShapeElement->getAttribute(m_attrname);
	m_nowval = m_preval;
}

std::wstring CActionAttr::Undo()
{
	m_pShapeElement->setAttribute(m_attrname, m_preval);
	m_pShapeElement->resetUseMatrix();
	m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
	m_pDocument->ResetAllFocus();
	m_pDocument->SetContentHasChange();
	return L"";
}

std::wstring CActionAttr::Redo()
{
	m_pShapeElement->setAttribute(m_attrname.c_str(), m_nowval.c_str());
	m_pShapeElement->resetUseMatrix();
	m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
	m_pDocument->ResetAllFocus();
	m_pDocument->SetContentHasChange();
	return L"";
}

void CActionAttr::EndValue()
{
	m_nowval = m_pShapeElement->getAttribute(m_attrname);
}

//////////////////////////////////////////////////////////////////////////
CActionValue::CActionValue(CSVGDocument *pIDoc, CSVGElement *pShapeElement)
{
	m_ActionType = ACTION_VALUE;
	m_pDocument = pIDoc;
	m_pShapeElement = pShapeElement;
	m_preval = m_pShapeElement->getNodeValue();
	m_nowval = m_preval;
}

std::wstring CActionValue::Undo()
{
	m_pShapeElement->setNodeValue(m_preval);
	m_pShapeElement->resetUseMatrix();
	m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
	m_pDocument->ResetAllFocus();
	m_pDocument->SetContentHasChange();
	return L"";
}

std::wstring CActionValue::Redo()
{
	m_pShapeElement->setNodeValue(m_nowval);
	m_pShapeElement->resetUseMatrix();
	m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
	m_pDocument->ResetAllFocus();
	m_pDocument->SetContentHasChange();
	return L"";
}

void CActionValue::EndValue()
{
	m_nowval = m_pShapeElement->getNodeValue();
}

//////////////////////////////////////////////////////////////////////////
CActionDel::CActionDel(CSVGDocument *pIDoc, CSVGElement *pShapeElement)
{
	m_pDocument = pIDoc;
	m_pParentElement = pShapeElement->getParentNode();
	m_pBeforeElement = pShapeElement->getPreviousSibling();
	m_pShapeElement = pShapeElement;
	m_pRemoveElement = pShapeElement;
}

CActionDel::~CActionDel()
{
	if (m_pRemoveElement)
	{
		delete m_pRemoveElement;
	}
}

std::wstring CActionDel::Undo()
{
	m_pParentElement->insertBefore(m_pShapeElement, m_pBeforeElement);
	m_pDocument->ReSetAllElementIndex();
	m_pDocument->AddToSelect(m_pShapeElement, Qt::green);
	m_pDocument->ResetAllFocus();
	m_pDocument->SetContentHasChange();
	m_pRemoveElement = NULL;
	return L"";
}

std::wstring CActionDel::Redo()
{
	m_pDocument->RemoveFromSelect(m_pShapeElement);
	m_pShapeElement->remove();
	m_pDocument->SetContentHasChange();
	m_pDocument->ResetAllFocus();
	m_pRemoveElement = m_pShapeElement;
	return L"";
}

//////////////////////////////////////////////////////////////////////////
CActionLayer::CActionLayer(CSVGDocument *pDoc, CSVGElement *pShapeElement)
{
	m_pBeforePreNode = NULL;
	m_pAfterPreNode = NULL;
	m_pDocument = pDoc;
	m_pShapeElement = pShapeElement;
	m_pParent = m_pShapeElement->getParentNode();
	for (int i = 0; i < (int)m_pParent->getChildCount(); ++i)
	{
		if (m_pParent->getChild(i) == m_pShapeElement)
		{
			if (i != 0)
			{
				m_pBeforePreNode = m_pParent->getChild(i - 1);
				m_pAfterPreNode = m_pBeforePreNode;
				break;
			}
		}
	}
}

CActionLayer::~CActionLayer()
{

}

std::wstring CActionLayer::Undo()
{
	if (m_pBeforePreNode)
	{
		m_pParent->insertAfter(m_pShapeElement, m_pBeforePreNode);
	}
	else
	{
		m_pParent->insertBefore(m_pShapeElement, NULL);
	}
	m_pDocument->ReSetAllElementIndex();
	m_pDocument->SetContentHasChange();
	return L"";
}

std::wstring CActionLayer::Redo()
{
	if (m_pAfterPreNode)
	{
		m_pParent->insertAfter(m_pShapeElement, m_pAfterPreNode);
	}
	else
	{
		m_pParent->insertBefore(m_pShapeElement, NULL);
	}
	m_pDocument->ReSetAllElementIndex();
	m_pDocument->SetContentHasChange();
	return L"";
}

void CActionLayer::EndValue()
{
	m_pAfterPreNode = NULL;
	for (int i = 0; i < (int)m_pParent->getChildCount(); ++i)
	{
		if (m_pParent->getChild(i) == m_pShapeElement)
		{
			if (i != 0)
			{
				m_pAfterPreNode = m_pParent->getChild(i - 1);
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
CActionGroup::CActionGroup(CSVGDocument *pIDoc, CSVGElement *pGElement)
{
	m_pDocument = pIDoc;
	m_pGElement = pGElement;
	m_pParentElement = m_pGElement->getParentNode();
	m_pRemoveElement = NULL;
}

CActionGroup::~CActionGroup()
{
	delete m_pRemoveElement;
}

std::wstring CActionGroup::Undo()
{
	m_pDocument->RemoveFromSelect(m_pGElement);
	m_pDocument->RemoveInnerSelect();
	for (int i = m_pGElement->getChildCount() - 1; i >= 0; --i)
	{
		CSVGElement *pElement = m_pGElement->getChild(i);
		m_childnodes.push_back(pElement);
		m_pGElement->removeChildOnly(pElement);
		m_pParentElement->appendChild(pElement, false);
		m_pDocument->AddToSelect(pElement, Qt::green);
		m_pDocument->ResetAllFocus();
	}
	m_pRemoveElement = m_pGElement;
	m_pGElement->remove();
	return L"";
}

std::wstring CActionGroup::Redo()
{
	for (size_t i = 0; i < m_childnodes.size(); ++i)
	{
		m_pDocument->RemoveFromSelect(m_childnodes[i]);
		m_childnodes[i]->remove();
		m_pGElement->appendChild(m_childnodes[i], false);
	}
	m_pParentElement->appendChild(m_pGElement, true);
	m_childnodes.clear();
	m_pDocument->AddToSelect(m_pGElement, Qt::green);

	m_pRemoveElement = NULL;
	return L"";
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CActionUnGroup::CActionUnGroup(CSVGDocument *pIDoc, CSVGElement *pGElement)
{
	m_pDocument = pIDoc;
	m_pParentElement = pGElement->getParentNode();
	m_pNextElement = pGElement->getNextSibling();
	m_pGElement = pGElement;
	m_pRemoveElement = pGElement;
	for (size_t i = 0; i < pGElement->getChildCount(); ++i)
	{
		m_childnodes.push_back(pGElement->getChild(i));
	}
}

CActionUnGroup::~CActionUnGroup()
{
	delete m_pRemoveElement;
}

std::wstring CActionUnGroup::Undo()
{
	m_pParentElement->insertBefore(m_pGElement, m_pNextElement, false);
	for (size_t i = 0; i < m_childnodes.size(); ++i)
	{
		m_pParentElement->removeChildOnly(m_childnodes[i]);
		m_pGElement->appendChild(m_childnodes[i], false);

		if (m_childnodes[i]->IsShapeElement())
		{
			CSVGElementShape *pChild = (CSVGElementShape*)m_childnodes[i];
			CSVGMatrix gMat = m_pGElement->getMatrix();
			CSVGMatrix  rmat = gMat;
			rmat.Inverse();
			CSVGMatrix ownerMat = pChild->getMatrix();
			CSVGMatrix newMat = rmat;
			newMat.Add(ownerMat);
			pChild->setAttribute(L"transform", newMat.getText());
			pChild->resetUseMatrix();
		}
	}

	m_pDocument->AddToSelect(m_pGElement, Qt::green);

	m_pRemoveElement = NULL;
	return L"";
}

std::wstring CActionUnGroup::Redo()
{
	for (size_t i = 0; i < m_pParentElement->getChildCount(); ++i)
	{
		CSVGElement *pGElement = m_pParentElement->getChild(i);
		if (pGElement == m_pGElement)
		{
			CSVGElement *pBeforeElement = m_pGElement;
			for (int j = (int)m_childnodes.size() - 1; j >= 0; j--)
			{
				CSVGElement *pMovechild = m_childnodes.at(j);
				if (pMovechild->IsShapeElement() && m_pParentElement->IsShapeElement())
				{
					CSVGElementShape *pShapeMoveChild = (CSVGElementShape*)pMovechild;
					pShapeMoveChild->remove();

					CSVGMatrix gMat = m_pGElement->getMatrix();
					CSVGMatrix ownerMat = pShapeMoveChild->getMatrix();
					CSVGMatrix newMat = gMat;
					newMat.Add(ownerMat);
					pShapeMoveChild->setAttribute(L"transform", newMat.getText());
					pShapeMoveChild->resetUseMatrix();

					m_pParentElement->insertBefore(pShapeMoveChild, pBeforeElement, false);
					pBeforeElement = pShapeMoveChild;
					m_pDocument->AddToSelect(pShapeMoveChild, Qt::green);
				}
			}
			m_pGElement->remove();
			break;
		}
	}
	return L"";
}
