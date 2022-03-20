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
#include "ToolButtonComboxMenu.h"


CToolButtonComboxMenu::CToolButtonComboxMenu(int nType, int nIndex, QWidget *parent)
	:QToolButton(parent)
{
	m_ntype = nType;
	m_nIndex = nIndex;
	pComboMenu = new CMenuCombox();
	pComboMenu->initImage(nType);
	connect(pComboMenu, SIGNAL(OnSelect(int)), this, SLOT(Select(int)));
	ResetIcon();
}

CToolButtonComboxMenu::~CToolButtonComboxMenu()
{
}

void CToolButtonComboxMenu::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button())
	{
		QRect popupButtonRect = rect();
		if (popupButtonRect.isValid() && popupButtonRect.contains(event->pos()))
		{
			QPoint pt(0, popupButtonRect.height());
			pt = mapToGlobal(pt);
			pComboMenu->exec(pt);
		}
	}
}

void CToolButtonComboxMenu::Select(int nIndex)
{
	m_nIndex = nIndex;
	ResetIcon();
	emit OnSelected(nIndex);
}

void CToolButtonComboxMenu::ResetIcon()
{
	QImage image;
	QIcon icon;
	if (m_ntype == 0)
	{
		image = QImage(":/images/linewidth.png");
	}
	else
	{
		image = QImage(":/images/linetype.png");
	}
	for (int i = 0; i < image.height() / 20; i++)
	{
		if (i == m_nIndex)
		{
			QImage cpyimage = image.copy(0, i * 20, 100, 20);
			QPixmap pix = QPixmap::fromImage(cpyimage);
			icon = QIcon(pix);
			break;
		}
	}
	setIcon(icon);
}
