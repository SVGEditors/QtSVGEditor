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
#include "DlgInputName.h"

CDlgInputName::CDlgInputName(const QString & strTitle, const QString & strDefname, const QString & strType)
{
	m_ui.setupUi(this);
	m_strType = strType;
	this->setWindowTitle(strTitle);
	m_ui.lineEditName->setText(strDefname);
	m_ui.lineEditName->selectAll();
	m_ui.lineEditName->setFocus(Qt::NoFocusReason);
	connect(m_ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(m_ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CDlgInputName::~CDlgInputName()
{
}

QString CDlgInputName::GetName()
{
	QString strText = m_ui.lineEditName->text();
	strText = strText.replace(m_strType, "");
	strText += m_strType;
	return strText;
}

void CDlgInputName::OnOK()
{
	QDialog::accept();
}

void CDlgInputName::OnCancel()
{
	QDialog::close();
}
