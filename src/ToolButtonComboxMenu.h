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
#include "qtoolbutton.h"
#include "MenuCombox.h"
#include <QMouseEvent>
class CToolButtonComboxMenu : public QToolButton
{
	Q_OBJECT
public:
	CToolButtonComboxMenu(int nType, int nIndex, QWidget *parent);
	~CToolButtonComboxMenu();
	void ResetIcon();
	virtual void mousePressEvent(QMouseEvent *event);
Q_SIGNALS:
	void OnSelected(int nIndex);
	public Q_SLOTS:
	void Select(int nIndex);
private:
	CMenuCombox *pComboMenu;
	int m_nIndex;
	int m_ntype;
};
