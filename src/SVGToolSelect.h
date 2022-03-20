#pragma once
#include "ExportMarco.h"
#include "SVGToolShape.h"
class SVGCONTROL_API CSVGToolSelect : public CSVGToolShape
{
public:
	CSVGToolSelect(CSVGDocument *pDocument);
	~CSVGToolSelect();
	virtual unsigned short GetToolType() { return TOOLTYPE_SELECT; };
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnLButtonDblClk(const POINT& point);
	void SetmouseType(unsigned short type);//0矩阵变化,1绝对坐标
private:
	void  ShowHandleCursor();
	mousetype    m_mousetype;
	SelectMode   m_selectMode;
	bool         m_bCanMove;
	POINT        m_ptDown;
	int          m_nHandle;//保存鼠标按下时选中的哪个角点
	CSVGElementShape *m_pCurrentShape;
};


