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
#include "MenuCombox.h"
#include <QWidgetAction>
#include <QGridLayout>
#include <QToolButton>
#include <QPushButton>
#include "QLabel"

CMenuCombox::CMenuCombox()
{
}


CMenuCombox::~CMenuCombox()
{
}

void CMenuCombox::initImage(int nType)
{
	m_Action = new QWidgetAction(this);
	m_Widget = new QWidget(this);
	QGridLayout *gLayout = new QGridLayout(m_Widget);
	gLayout->setMargin(0);
	gLayout->setSpacing(5);
	if (nType == 0)
	{
		QImage image(":/images/linewidth.png");
		for (int i = 0; i < image.height() / 20; i++)
		{
			QImage cpyimage = image.copy(0, i * 20, 80, 20);
			QPixmap pix = QPixmap::fromImage(cpyimage);
			QToolButton *btn = new QToolButton(m_Widget);
			btn->setFixedSize(80, 20);
			btn->setIconSize(QSize(btn->width(), btn->height()));
			btn->setIcon(QIcon(pix));
			btn->setAutoRaise(true);
			btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
			QAction *act = new QAction(QIcon(pix), QString(), btn);
			act->setData(QVariant(i));
			connect(act, &QAction::triggered, this, &CMenuCombox::OnSelected);
			btn->setDefaultAction(act);
			gLayout->addWidget(btn, i, 0);
		}
	}
	else if (nType == 1)
	{
		QImage image(":/images/linetype.png");
		for (int i = 0; i < image.height() / 20; i++)
		{
			QImage cpyimage = image.copy(0, i * 20, 80, 20);
			QPixmap pix = QPixmap::fromImage(cpyimage);
			QToolButton *btn = new QToolButton(m_Widget);
			btn->setFixedSize(80, 20);
			btn->setIconSize(QSize(btn->width(), btn->height()));
			btn->setIcon(QIcon(pix));
			btn->setAutoRaise(true);
			btn->setToolButtonStyle(Qt::ToolButtonIconOnly);
			QAction *act = new QAction(QIcon(pix), QString(), btn);
			act->setData(QVariant(i));
			connect(act, &QAction::triggered, this, &CMenuCombox::OnSelected);
			btn->setDefaultAction(act);
			gLayout->addWidget(btn, i, 0);
		}

	}
	m_Action->setDefaultWidget(m_Widget);
	addAction(m_Action);
}

void CMenuCombox::OnSelected()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act)
	{
		int nIndex = qvariant_cast<int>(act->data());
		emit OnSelect(nIndex);
	}
	this->setVisible(false);
}
