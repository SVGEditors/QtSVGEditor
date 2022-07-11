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
#include "DlgAbout.h"
#include <QIcon>
#include <QLabel>
#include "QDesktopServices"
#include <QUrl>

CDlgAbout::CDlgAbout()
{
	m_ui.setupUi(this);
	m_ui.ButtonIcon->setIconSize(m_ui.ButtonIcon->size());
	m_ui.ButtonIcon->setIcon(QIcon(":/images/windows32.png"));
	QLabel *pLabel1 = new QLabel(tr("QtSVGEditor"), this);
	pLabel1->setGeometry(QRect(100, 20, 450, 30));
	QLabel *pLabel2 = new QLabel(QString::fromStdWString(L"技术支持:"), this);
	pLabel2->setGeometry(QRect(100, 50, 450, 50));

	QLabel* labelLink = new QLabel(this);
	labelLink->setText("<a style=\"font-size:10pt\" href=\"http://www.svgsvg.cn/support/tech/\">http://www.svgsvg.cn/support/tech/</a>"));
	labelLink->setGeometry(QRect(100, 80, 300, 50));

	connect(m_ui.ButtonOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(labelLink, SIGNAL(linkActivated(QString)), this, SLOT(openUrl(QString)));
}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::OnOK()
{
	QDialog::accept();
}

void CDlgAbout::openUrl(QString url)
{
	QDesktopServices::openUrl(QUrl(url));
}
