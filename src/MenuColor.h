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

#include <QMenu>
#include <QWidgetAction>

class CMenuColor : public QMenu
{
	Q_OBJECT
public:
	CMenuColor(int nType = 0, QWidget *parent = Q_NULLPTR);
	void init(int nType);
Q_SIGNALS:
	void colorSelected(QColor);
	void colorSelectedRadient(const std::wstring &);
	void colorSelectedPattern(const std::wstring &);
	void colorSelectedPicture(const std::wstring &);
	public Q_SLOTS:
	void OnColorSelected();
	void OnTransprent(QAction *pAction);
	void OnMoreColor();
	void OnRadient();
	void OnPattern();
private:
	QWidgetAction	*m_colorAction;
	QWidget			*m_colorWidget;
};
