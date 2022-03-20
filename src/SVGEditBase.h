#pragma once
#include "SVGDOMDef.h"
#include "TooltipText.h"
class CSVGElementShape;
class CSVGDocument;
class  CSVGEditBase
{
public:
	CSVGEditBase(CSVGDocument *pDocument);
	virtual ~CSVGEditBase();
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnRButtonDown(const POINT &point);
	virtual void OnMButtonDown(const POINT &point);
	virtual void OnRButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnLButtonDblClk(const POINT& point);
	virtual void OnContextMenu(const POINT &point);
	virtual void OnEscPushDown() {};
	bool         IsFinish();
	void         BeginAction();
	void		 EndAction();
protected:
	POINT            m_downPoint;
	POINT            m_lastPoint;
	bool		     m_bCapture;
	bool		     m_bLButtonDown;
	CSVGDocument*	 m_pDocument;
	CSVGElementShape *m_pShape;
	bool             m_bFinish;
	CToolTipText    *m_pToolTip;
};

