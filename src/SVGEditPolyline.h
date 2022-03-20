#pragma once
#include "SVGEditBase.h"
class CActionAttr;
class CSVGEditPolyline : public CSVGEditBase
{
public:
	CSVGEditPolyline(CSVGDocument *pDocument, int nType, CSVGElementShape *pShape=NULL);
	~CSVGEditPolyline();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnLButtonDblClk(const POINT& point);
	virtual void OnRButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT& point);
	virtual void OnEscPushDown();
private:
	void   Finish();
	void   EndTo(const POINT& point);
	int    m_nType;//TOOLTYPE_POLYLINE,TOOLTYPE_POLYGON
	bool   m_bContinueEdit;
	CActionAttr *m_pActionAttr;
};

