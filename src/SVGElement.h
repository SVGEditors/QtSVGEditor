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
#pragma once
#include "SVGRectF.h"
#include "SVGMatrix.h"
#include "SVGGlobal.h"
#include <QImage>
#include <set>

class CSVGElementShape;
class CSVGDocument;
class CSVGElementStyle;

#define AREA_GRID 150
struct AreaManager
{
	bool                         bneedRepaint;
	long long                    nAreaId;
	CSVGRectF                    rect;
	std::list<CSVGElementShape*> areaElement;
	QImage                      *m_pbitmap;
	AreaManager()
	{
		bneedRepaint = false;
		m_pbitmap = NULL;
		nAreaId = 0;
	}
	~AreaManager()
	{
		areaElement.clear();
		if (m_pbitmap)
		{
			delete m_pbitmap;
		}
	}
};

class  CSVGElement
{
	friend class CSVGDocument;
public:
	CSVGElement(CSVGDocument* ownerDoc, const std::wstring & tagname, unsigned short nodetype);
	virtual ~CSVGElement();
public:
	unsigned short	     getNodeType();
	std::wstring         getTagName();
	//////////////////////////////////////////////////////////////////////////
	CSVGElement*		 getPreviousSibling();
	CSVGElement*		 getNextSibling();
	CSVGElement*         getParentNode();
	std::wstring         getParentId();
	std::wstring         getPreviousSiblingId();
	std::wstring         getNextSiblingId();
	unsigned int         getChildCount();
	CSVGElement*         getChild(unsigned int index);
	virtual CSVGElement* cloneNode(bool deep);
	virtual CSVGElement  *cloneNodeWithId(bool deep);
	bool                 removeChild(CSVGElement* child, bool bSetModfiyFlag = true);
	bool                 removeChildOnly(CSVGElement* child);
	bool                 deleteChild(CSVGElement* child);
	void                 remove(bool bSetModfiyFlag = true);
	void                 deleteAllChild();
	bool                 addChild(CSVGElement* newChild, bool bDeep, bool bSetModifyFlag = true);
	bool                 appendChild(CSVGElement* newChild, bool bSetModifyFlag = true);
	bool                 insertBefore(CSVGElement* newChild, CSVGElement *refChild, bool bDeep = true, bool bSetModifyFlag = true);
	bool                 insertAfter(CSVGElement* newChild, CSVGElement *refChild, bool bDeep = true);
	bool                 pushfront(CSVGElement* newChild, bool bDeep, bool bSetModifyFlag = true);
	//////////////////////////////////////////////////////////////////////////
	virtual void         setAttribute(const std::wstring &attrname, const std::wstring& value, bool bSetModifyFlag = true);
	void                 setFloatAttribute(const std::wstring & attrname, float fval, bool bSetModifyFlag = true);
	float                getFloatAttribute(const std::wstring & attrname);
	int                  getIntAttribute(const std::wstring & attrname);
	std::wstring         getAttributeId();
	virtual std::wstring getAttribute(const std::wstring & attrname);
	virtual bool         delAttribute(const std::wstring & attrname);
	virtual bool         delAllAttribute();
	virtual std::wstring getNodeValue();
	virtual void         setNodeValue(const std::wstring & value, bool bSetModifyFlag = true);
	void                 setAttrChange(bool bChange);
	bool                 IsAttrChange();
	void                 AddToLabelName(std::set<std::wstring>&vcname);
	bool                 IsStyleAttr(const std::wstring &attrname);
	//////////////////////////////////////////////////////////////////////////
	CSVGDocument*        getOwnerDocument();
	CSVGElement*         getRootSVGElement();
	CSVGElementStyle *   GetOrNewStyle();
	virtual bool         Toxml(std::wstring &retstrxml, int nLevel);
	std::wstring         getAttrxml();
	virtual bool         IsShapeElement() { return false; }
	virtual bool         IsGroupElement() { return false; }
	virtual bool         IsStyleableElement() { return false; }
	virtual std::set<std::wstring> getAttrNames();
	int                  GetElementIndex() { return m_ElementIndex; }
	bool                 IsShow();
	void                 SetShow(bool bShow);
	void                 ClearXML();
	void                 ChangeFillIds(std::map<std::wstring, std::wstring>&idmap);
	virtual void         resetUseMatrix(CSVGElement *pParent = NULL);
	CSVGMatrix           getParentMatrix();
	CSVGMatrix           getUsedMatrix();
	CSVGMatrix           getMatrix();
	CSVGMatrix           DoMatrixTranslate(CSVGMatrix Newmat);
	CSVGElement         *GetTopGroupElement();
	void                 GetAllElementFlat(std::vector<CSVGElement*>&vcElement, bool bIncludeself = true);
	void                 SetLock(bool bLock);
	bool                 IsLock();
	void                 RemoveChildByTagNames(std::vector<std::wstring>&tagNames);
	void                 RemoveAttrByName(std::vector<std::wstring>&attrNames);
	void                 RemoveAttrByKeyName(std::vector<std::wstring>&attrKeyNames);
	void                 DeclineMoreClass();
	virtual void         SetTwoPrecision();
public:
	void                 pathHasChange();
protected:
	void                 SetCheckModify();
	virtual void         parseReady() {};
	inline void          AddLevelSpace(std::wstring &strText, int nlevel);
	void                 GetGroupElement(CSVGElement *&pGroup);
private:
	CSVGElement*		 getprenode();
	CSVGElement*		 getnextnode();
	void                 resetElement();
	void                 setParentAttrChange();
	void                 setChildAttrChange();
protected:
	unsigned short				m_nodeType;
	std::wstring    			m_tagName;
	bool                        m_bShow;
	CSVGDocument*				m_pownDocument;
	CSVGElement*				m_parentNode;
	std::vector<CSVGElement*>	m_ChildNodes;
	int							m_nChildCntBuffSize;
	std::wstring				m_nodeValue;
	std::map<int, std::wstring>	m_mapAttrValue;
	int							m_ElementIndex;
	std::wstring                m_strXml;
	CSVGMatrix			        m_usedmatrix;
	bool                        m_bAttrChange;
	bool                        m_bPathHasChanged;
	std::wstring                m_id;
	bool                        m_bLock;
protected:
	CSVGElement();
};
