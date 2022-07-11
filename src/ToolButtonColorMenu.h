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
#include "qtoolbutton.h"
#include <QMouseEvent>
#include <QMenu>
#include "MenuColor.h"
class CToolButtonColorMenu : public QToolButton
{
	Q_OBJECT
public:
	CToolButtonColorMenu(int nIconIndex, QColor defaultclr, QWidget *parent = Q_NULLPTR);
	~CToolButtonColorMenu();
	virtual void mousePressEvent(QMouseEvent *event);
Q_SIGNALS:
	void ColorSelected(QColor clr);
	void colorSelectedRadient(const std::wstring &);
	void colorSelectedPattern(const std::wstring &);
	void colorSelectedPicture(const std::wstring &);
public Q_SLOTS:
	void OnColorSelect(QColor clr);
	void OncolorSelectedRadient(const std::wstring &str);
	void OncolorSelectedPattern(const std::wstring &str);
	void OncolorSelectedPicture(const std::wstring &str);
private:
	QIcon GetUnderLineColorIcon(const QColor &clr);
	CMenuColor *m_pMenu;
	int   m_nIconIndex;
	QColor  m_currentColor;
};
