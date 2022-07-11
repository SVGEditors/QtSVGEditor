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
#include "DlgRotateSlider.h"
#include <QLineEdit>


CDlgRotateSlider::CDlgRotateSlider(QWidget *parent)
	:QDialog(parent)
{
	m_nValue = 0;
	m_ui.setupUi(this);
	QSize sz = size();
	setFixedSize(sz);
	m_ui.spinBox->setRange(-360, 360);
	m_ui.spinBox->setValue(0);
	m_ui.horizontalSlider->setRange(0, 360);
	connect(m_ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnvalueChanged(int)));
	connect(m_ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(OnSpinvalueChanged(int)));
	m_ui.spinBox->setFocus();
}

CDlgRotateSlider::~CDlgRotateSlider()
{

}

void CDlgRotateSlider::OnvalueChanged(int value)
{
	if (value != m_nValue)
	{
		m_nValue = value;
		m_ui.spinBox->setValue(value);
		emit RotatevalueChanged(value);
	}
}

void CDlgRotateSlider::OnSpinvalueChanged(int value)
{
	if (value != m_nValue)
	{
		m_nValue = value;
		m_ui.horizontalSlider->setValue(value);
		emit RotatevalueChanged(value);
	}
}
