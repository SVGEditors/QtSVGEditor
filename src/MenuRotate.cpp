
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
#include "MenuRotate.h"
#include <QGridLayout>
#include <QToolButton>

CMenuRotate::CMenuRotate(CSVGView *pView)
{
	m_nLastValue = 0;
	m_nValue = 0;
	m_pDlg = new CDlgRotateSlider(this);
	connect(m_pDlg, SIGNAL(RotatevalueChanged(int)), this, SLOT(OnValueChange(int)));
	connect(this, SIGNAL(valueChanged(int, bool)), pView, SLOT(OnRotate(int, bool)));
	Init();
}

CMenuRotate::~CMenuRotate()
{
	emit valueChanged(m_nValue - m_nLastValue, true);
}

void CMenuRotate::Init()
{
	QWidgetAction* pAction = new QWidgetAction(this);
	pAction->setDefaultWidget(m_pDlg);
	addAction(pAction);
}

void CMenuRotate::OnValueChange(int nValue)
{
	m_nValue = nValue;
	emit valueChanged(m_nValue - m_nLastValue, false);
	m_nLastValue = nValue;
}
