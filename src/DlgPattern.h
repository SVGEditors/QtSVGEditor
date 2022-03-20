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
#include "qdialog.h"
#include "ui_UIPattern.h"
#include "MenuColor.h"

class CDlgPattern : public QDialog
{
	Q_OBJECT
public:
	CDlgPattern();
	~CDlgPattern();
	std::wstring GetXML();
	public slots:
	void OnMenuForeColor(QColor clr);
	void OnMenuBackColor(QColor clr);
	void OnForeColor();
	void OnBackColor();
	void OnOK();
	void OnCancel();
	void Button1();
	void Button2();
	void Button3();
	void Button4();
	void Button5();
	void Button6();
	void Button7();
	void Button8();
	void Button9();
	void Button10();
	void Button11();
	void Button12();
private:
	void InitButtonIcon();
	void ReDrawPreview();
	Ui_UIPattern m_ui;
	CMenuColor *m_pColorMenuFore;
	CMenuColor *m_pColorMenuBack;
	int m_nIndex;
	QColor m_foreColor;
	QColor m_backColor;
};
