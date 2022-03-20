#pragma once
#include "SVGEditBase.h"
class CSVGEditText : public CSVGEditBase
{
public:
	CSVGEditText(CSVGDocument *pDocument,int nType);
	~CSVGEditText();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnMouseMove(const POINT& point);
private:
	int   m_nType;
};

