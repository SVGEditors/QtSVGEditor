#pragma once
#include "SVGEditBase.h"
class CActionComposite;
class CSVGElementText;
class  CSVGEditSelect : public CSVGEditBase
{
public:
	CSVGEditSelect(CSVGDocument *pDocument);
	~CSVGEditSelect();
	virtual unsigned short GetToolType() { return TOOLTYPE_SELECT; };
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnLButtonDblClk(const POINT& point);
	void         SetmouseType(unsigned short type);//0矩阵变化,1绝对坐标
	void         ShowInput();
	bool         IsInputing();
	void         OnVKeyDown(int virtKey);
	void         InputChar(const char *str);
	void         SetTextInputMode();
	void         SetTextInputMode(float fx,float fy);
	void         EndInputMode();
private:
	void         ShowHandleCursor(int nhandle);
	void         StartTimer();
	void         KillFocusTimer();
	mousetype    m_mousetype;
	SelectMode   m_selectMode;
	bool         m_bCanMove;
	bool         m_bCanCopy;
	POINT        m_ptDown;
	int          m_nHandle;//保存鼠标按下时选中的哪个角点
	CActionComposite *m_pTransformAction;
	CActionComposite *m_pSizetoAction;
	bool              m_bOnTimer;
	CSVGElementText  *m_pEditText;
	UINT              m_TimerID;
};


