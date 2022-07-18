/**
*
* @license MIT
*
* @copyright: 2022 LJ
*
* @technical support: http://www.svgsvg.cn/support/tech/
*
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
// ActionHistory.h

#ifndef ACTIONHISTORY_H
#define ACTIONHISTORY_H

#include <vector>			
#include "ElementMaker.h"

enum ACTIONTYPE
{
	ACTION_BASE,
	ACTION_COMPOSITE,
	ACTION_ADD,
	ACTION_XMLNODE,
	ACTION_ATTR,
	ACTION_VALUE,
	ACTION_DEL
};

class CSVGElementShape;
class CSVGDocument;
class CActionBase
{
public:
	CActionBase() { m_ActionType = ACTION_BASE; m_pDocument = NULL; };
	virtual ~CActionBase() {};
public:
	virtual std::wstring Undo() { return L""; };
	virtual std::wstring Redo() { return L""; };
	virtual void EndValue() {};
	ACTIONTYPE m_ActionType;
	CSVGDocument *m_pDocument;
};

class CActionComposite : public CActionBase
{
public:
	CActionComposite(CSVGDocument *pDoc);
	void PushBack(CActionBase *pAction);
	std::vector<CActionBase*>&GetActions();
	virtual ~CActionComposite();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
private:
	std::vector<CActionBase*>m_Actions;
};

class CActionAdd : public CActionBase
{
public:
	CActionAdd(CSVGDocument *pIDoc, CSVGElement *pParentElement, CSVGElement *pShapeElement);
	virtual ~CActionAdd();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
public:
	CSVGElement *m_pShapeElement;
	CSVGElement *m_pParentElement;
	CSVGElement *m_pRemoveElement;
};


class CActionDel : public CActionBase
{
public:
	CActionDel(CSVGDocument *pIDoc, CSVGElement *pShapeElement);
	virtual ~CActionDel();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
public:
	CSVGElement *m_pBeforeElement;
	CSVGElement *m_pShapeElement;
	CSVGElement *m_pParentElement;
	CSVGElement *m_pRemoveElement;
};

class CActionAttr : public CActionBase
{
public:
	CActionAttr(CSVGDocument *pIDoc, CSVGElement *pShapeElement, std::wstring attrname);
	virtual std::wstring Undo();
	virtual std::wstring Redo();
	virtual void EndValue();
private:
	CSVGElement *m_pShapeElement;
	std::wstring m_attrname;
	std::wstring m_preval;
	std::wstring m_nowval;
};

class CActionValue : public CActionBase
{
public:
	CActionValue(CSVGDocument *pIDoc, CSVGElement *pShapeElement);
	virtual std::wstring Undo();
	virtual std::wstring Redo();
	virtual void EndValue();
private:
	CSVGElement *m_pShapeElement;
	std::wstring m_preval;
	std::wstring m_nowval;
};

class CActionLayer : public CActionBase
{
public:
	CActionLayer(CSVGDocument *pDoc, CSVGElement *pShapeElement);
	virtual ~CActionLayer();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
	void   EndValue();
public:
	CSVGElement  *m_pShapeElement;
	CSVGElement  *m_pParent;
	CSVGElement  *m_pBeforePreNode;
	CSVGElement  *m_pAfterPreNode;
};

class CActionXMLNode : public CActionBase
{
public:
	CActionXMLNode();
	virtual ~CActionXMLNode();
	void AddAction(QVector<CXMLMakerNode> &undoDatas, QVector<CXMLMakerNode>&redoDatas);
	void ChangeRedoData(QVector<CXMLMakerNode>&redoDatas);
	QVector<CXMLMakerNode> GetRedoData();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
private:
	void GetAttrValueFromParam(std::wstring &strParam, std::vector<std::wstring>& AttrNames, std::vector<std::wstring> &AttrVals);
	QVector<CXMLMakerNode>m_redodatas;
	QVector<CXMLMakerNode>m_undodatas;
};


class CActionGroup : public CActionBase
{
public:
	CActionGroup(CSVGDocument *pDoc, CSVGElement *pGElement);
	virtual ~CActionGroup();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
public:
	CSVGElement *m_pGElement;
	CSVGElement *m_pParentElement;
	CSVGElement *m_pRemoveElement;
	std::vector<CSVGElement*> m_childnodes;
};

class CActionUnGroup : public CActionBase
{
public:
	CActionUnGroup(CSVGDocument *pDoc, CSVGElement *pGElement);
	virtual ~CActionUnGroup();
	virtual std::wstring Undo();
	virtual std::wstring Redo();
public:
	CSVGElement *m_pGElement;
	CSVGElement      *m_pParentElement;
	CSVGElement      *m_pNextElement;
	CSVGElement *m_pRemoveElement;
	std::vector<CSVGElement*> m_childnodes;
};

class CActionHistory
{
public:
	CActionHistory();
	~CActionHistory();
public:
	void    popback();
	CActionBase *back();
	std::wstring Undo();
	std::wstring Redo();
	bool         CanUndo();
	bool         CanRedo();
	void         AddAction(CActionBase* NewAction);
	CActionXMLNode *GetLastComposite();
	void     ClearAll();
private:
	void ClearEnd();
	void ClearHistory();
	int GetPos() const { return (int)m_nPos; }
	int GetTotal() const { return (int)m_arActions.size(); }
	std::vector<CActionBase*> m_arActions;
	size_t m_nPos;
};
#endif	// ACTIONHISTORY_H
