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
#include "MenuColor.h"
#include <QToolButton>
#include <QGridLayout>
#include <QPainter>
#include <QLabel>
#include <QAction>
#include <QColorDialog>
#include "DlgGradient.h"
#include "DlgPattern.h"

const QColor colors[6][8] =
{
	{ QColor(0, 0, 0, 255), QColor(170, 0, 0, 255), QColor(0, 85, 0, 255), QColor(170, 85, 0, 255),
	QColor(0, 170, 0, 255), QColor(170, 170, 0, 255), QColor(0, 255, 0, 255), QColor(170, 250, 0, 255) },

	{ QColor(0, 0, 127, 255), QColor(170, 0, 127, 255), QColor(0, 85, 127, 255), QColor(170, 85, 127, 255),
	QColor(0, 170, 127, 255), QColor(170, 170, 127, 255), QColor(0, 255, 127, 255), QColor(170, 255, 127, 255) },

	{ QColor(0, 0, 255, 255), QColor(170, 0, 255, 255), QColor(0, 85, 255, 255), QColor(170, 85, 255, 255),
	QColor(0, 170, 255, 255), QColor(170, 170, 255, 255), QColor(0, 255, 255, 255), QColor(170, 255, 255, 255) },

	{ QColor(85, 0, 0, 255), QColor(255, 0, 0, 255), QColor(85, 85, 0, 255), QColor(255, 85, 0, 255),
	QColor(85, 170, 0, 255), QColor(255, 170, 0, 255), QColor(85, 255, 0, 255), QColor(255, 255, 0, 255) },

	{ QColor(85, 0, 127, 255), QColor(255, 0, 127, 255), QColor(85, 85, 127, 255), QColor(255, 85, 127, 255),
	QColor(85, 170, 127, 255), QColor(255, 170, 127, 255), QColor(85, 255, 127, 255), QColor(255, 255, 127, 255) },

	{ QColor(85, 0, 255, 255), QColor(255, 0, 255, 255), QColor(85, 85, 255, 255), QColor(255, 85, 255, 255),
	QColor(85, 170, 255, 255), QColor(255, 170, 255, 255), QColor(85, 255, 255, 255), QColor(255, 255, 255, 255) }
};

CMenuColor::CMenuColor(int nType, QWidget *parent)
	: QMenu(parent)
{
	init(nType);
}

void CMenuColor::init(int nType)
{
	QWidgetAction* pAction = new QWidgetAction(this);
	pAction->setData(QColor(Qt::transparent));
	pAction->setIconText(tr("transparent"));
	connect(pAction, &QAction::triggered, this, &CMenuColor::OnColorSelected);
	addAction(pAction);

	addSeparator();

	m_colorAction = new QWidgetAction(this);
	m_colorWidget = new QWidget(this);
	QGridLayout *gLayout = new QGridLayout(m_colorWidget);
	gLayout->setMargin(0);
	gLayout->setSpacing(2);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			QPixmap pix(20, 20);
			pix.fill(colors[i][j]);

			QToolButton *btn = new QToolButton(m_colorWidget);
			btn->setFixedSize(20, 20);
			QAction *act = new QAction(QIcon(pix), QString(), btn);
			act->setData(QVariant(colors[i][j]));
			connect(act, &QAction::triggered, this, &CMenuColor::OnColorSelected);
			btn->setDefaultAction(act);
			gLayout->addWidget(btn, i, j);
		}
	}
	m_colorAction->setDefaultWidget(m_colorWidget);
	addAction(m_colorAction);

	addSeparator();
	QWidgetAction* pMoreColorAction = new QWidgetAction(this);
	pMoreColorAction->setData(QColor(Qt::transparent));
	connect(pMoreColorAction, &QAction::triggered, this, &CMenuColor::OnMoreColor);
	pMoreColorAction->setText(QString::fromStdWString(L"更多颜色"));
	addAction(pMoreColorAction);

	if (nType == 1)
	{
		addSeparator();
		QWidgetAction* pRadientColorAction = new QWidgetAction(this);
		pRadientColorAction->setData(QColor(Qt::transparent));
		connect(pRadientColorAction, &QAction::triggered, this, &CMenuColor::OnRadient);
		pRadientColorAction->setText(QString::fromStdWString(L"渐变色填充"));
		addAction(pRadientColorAction);

		addSeparator();
		QWidgetAction* pPatternAction = new QWidgetAction(this);
		pPatternAction->setData(QColor(Qt::transparent));
		connect(pPatternAction, &QAction::triggered, this, &CMenuColor::OnPattern);
		pPatternAction->setText(QString::fromStdWString(L"网纹填充"));
		addAction(pPatternAction);
	}
}

void CMenuColor::OnColorSelected()
{
	QAction *act = qobject_cast<QAction *>(sender());
	if (act)
	{
		QColor color = qvariant_cast<QColor>(act->data());
		emit colorSelected(color);
	}
	this->setVisible(false);
}

void CMenuColor::OnTransprent(QAction *pAction)
{
	Q_UNUSED(pAction);
	this->setVisible(false);
}

void CMenuColor::OnMoreColor()
{
	this->setVisible(false);
	QColorDialog dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		QColor color = dlg.currentColor();
		emit colorSelected(color);
	}
}

void CMenuColor::OnRadient()
{
	this->setVisible(false);
	CDlgGradient dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		std::wstring str = dlg.GetXML();
		emit colorSelectedRadient(str);
	}
}

void CMenuColor::OnPattern()
{
	this->setVisible(false);
	CDlgPattern dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		std::wstring str = dlg.GetXML();
		emit colorSelectedPattern(str);
	}
}
