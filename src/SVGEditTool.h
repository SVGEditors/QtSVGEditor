#pragma once
#include "ExportMarco.h"
class CSVGEditBase;
class CSVGDocument;
class SVGCONTROL_API CSVGEditTool
{
public:
	CSVGEditTool(CSVGDocument *pDoc,int nType);
	~CSVGEditTool();
	void         setToolType(int ntype);//TOOLTYPE_CIRCLE
	int          getToolType();
	bool         IsFinish();
	void         OnEscPushDown();
	void         OnLButtonDown(const POINT &point);
	void         OnLButtonUp(const POINT &point);
	void         OnMButtonDown(const POINT &point);
	void         OnMouseMove(const POINT &point);
	void         OnRButtonUp(const POINT &point);
	void         OnLButtonDblClk(const POINT& point);
	bool         IsInputing();
	void         OnVKeyDown(int virtKey);
	void         InputChar(const char *str);
	void         SetTextInputMode();
	void         EndInputMode();
private:
	void Clear();
	CSVGEditBase  *m_pTool;
	CSVGDocument  *m_pDoc;
	int            m_nType;
};

