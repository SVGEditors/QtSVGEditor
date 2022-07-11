/**
*
* @license MIT
*
* @copyright: 2022 LJ
*
* @technical support: http://www.svgsvg.cn/support/tech/
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
#pragma once
#include "qdialog.h"
#include "ui_UIGradient.h"
#include "MenuColor.h"

enum GRADIENTDIRECT
{
	GRADIENT_LEFTTORIGHT,
	GRADIENT_TOPTOBOTTOM,
	GRADIENT_LEFTTOTORIGHTBOTTOM,
	GRADIENT_LEFTBOTTOMTORIGHTTOP,
	GRADIENT_CENTER
};

struct COLORPOS
{
	QColor clr;
	float  fpos;
	bool   bSelect;
	COLORPOS()
	{
		fpos = 0;
		bSelect = false;
	}
	COLORPOS(QColor color, float pos)
	{
		clr = color;
		fpos = pos;
		bSelect = false;
	}
};

class CDlgGradient : public QDialog
{
	Q_OBJECT
public:
	CDlgGradient();
	~CDlgGradient();
	bool eventFilter(QObject *o, QEvent *e);
	std::wstring GetXML();
public slots:
	void OnColorMenu();
	void OnColor(QColor clr);
	void OnSlidervalueChanged(int value);
	void OncurrentHistoryIndexChanged(int index);
	void OnHeadToTail();
	void OnButton1();
	void OnButton2();
	void OnButton3();
	void OnButton4();
	void OnButton5();
	void OnOk();
	void OnCancel();
private:
	void  SetColor(QColor clr);
	void  InitHistory();
	QIcon GetGradientImage(QVector<COLORPOS>&clrArr, int nWidth, int nHeight, GRADIENTDIRECT Direct);
	void  ReDrawMultiColorBar();
	void  PressPosColor(int nXPos, int nYPos, bool bShowMenu);
	void  DeletePosColor(int nXPos);
	bool  IsSelectColor(int nXPos);
	void  MoveColor(int nXPos);
	void  ReDrawPreView();
private:
	Ui_Gradient      m_ui;
	CMenuColor*      m_pColorMenu;
	QVector<COLORPOS>m_vcColor;
	GRADIENTDIRECT   m_direct;
	QPoint           m_ptCenter;
	bool             m_bSelectCenter;
	QVector< QVector<COLORPOS> >m_templateClr;
};
