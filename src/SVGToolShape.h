#pragma once
#include "ExportMarco.h"
#include "SVGDocument.h"
#include "SVGDOMDef.h"

class SVGCONTROL_API CSVGToolShape
{
public:
	CSVGToolShape(CSVGDocument *pDocument);
	~CSVGToolShape();
	virtual unsigned short GetToolType() = 0;
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnRButtonDown(const POINT &point);
	virtual void OnRButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnLButtonDblClk(const POINT& point);
	virtual void OnContextMenu(const POINT &point);
	POINT           m_downPoint;
	POINT           m_lastPoint;
	bool		    m_bCapture;
	bool		    m_bLButtonDown;
	bool            m_bFinish;
	CSVGDocument*	m_pDocument;
};

