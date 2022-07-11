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
#include <QLineEdit>
class CLineEditEX : public QLineEdit
{
	Q_OBJECT
public:
	CLineEditEX(QWidget *parent = Q_NULLPTR);
	~CLineEditEX();
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
Q_SIGNALS:
	void DBClick();
	void focusIn();
};
