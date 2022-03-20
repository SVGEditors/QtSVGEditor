#pragma once
#include "SVGEditBase.h"
class CSVGPathSeg;
class CSVGPathSegLineto;
class CActionAttr;
class CSVGEditPath : public CSVGEditBase
{
public:
	CSVGEditPath(CSVGDocument *pDocument, int nType, CSVGElementShape* pShape=NULL);
	~CSVGEditPath();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnMouseMove(const POINT& point);
	virtual void OnRButtonUp(const POINT &point);
private:
	int                 m_nType;//TOOLTYPE_PATH,TOOLTYPE_PATHARC,TOOLTYPE_PATHFAN
	CSVGPathSeg        *m_pPathSeg;
	CSVGPathSegLineto  *m_pPathLineto;
	CSVGPoint	        m_lstCtrl;//上一次的控制点,当鼠标点击确定下来之后的控制点
	CSVGPoint	        m_lstCtrlMove;//移动过程中产生的临时控制点
	bool                m_bContinueEdit; 
	CActionAttr        *m_pActionAttr;
};

