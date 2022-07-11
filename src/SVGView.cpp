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
#include "SVGView.h"
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <QApplication>
#include <QDomDocument>
#include "ElementMaker.h"
#include <string>
#include <QMenu>
#include "QFileDialog"
#include <QMimeData>
#include <QColorDialog>
#include <QBuffer>
#include "SVGElementShape.h"
#include "MainFrame.h"
#include <QClipboard>
#include "SVGElementText.h"
#include "DlgInputName.h"
#include "main.h"
#include "SVGElementG.h"
#include "SVGElementImage.h"

#define ANIMATE_USABLE 0

CSVGView::CSVGView(CMainFrame *pSVGEditor)
{
	m_lstsvgtype = -1;
	m_nZoomTimerId = -1;
	m_timerid = -1;
	m_nZoomTimerEllapse = 0;
	m_nMouseWheelDelta = 0;
	m_buttondownpt = QPoint(0, 0);
	m_pMainFrame = pSVGEditor;
	setAcceptDrops(true);
	m_strcommand = L"select";
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	setFocus(Qt::NoFocusReason);
	setAttribute(Qt::WA_InputMethodEnabled);
	installEventFilter(this);
}

CSVGView::~CSVGView()
{
}

void CSVGView::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void CSVGView::dropEvent(QDropEvent * event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasUrls())
	{
		QList<QUrl> urls = mimeData->urls();
		QList<QUrl>::iterator iter = urls.begin();
		for (; iter != urls.end(); ++iter)
		{
			QString strPath = iter->toLocalFile();
			if (strPath.indexOf(".svg", 0, Qt::CaseInsensitive) > 0)
			{
				m_pMainFrame->OpenFile(strPath);
			}
		}
	}
	else
	{
		QPoint  pt = event->pos();
		QByteArray data = event->mimeData()->data("text");
		if (data.length() > 0)
		{
			QString strutf8xml = QString::fromUtf8(data);
			std::wstring strxml = strutf8xml.toStdWString();
			CSVGDocument doc;
			doc.LoadFromBuff(strxml);
			doc.MakeGroup();
			std::wstring strtemp;
			strtemp = doc.ToFileXml();
			m_SVGDocument.Paste(strtemp, pt);
		}
		QStringList typelst;
		typelst.push_back("jpg");
		typelst.push_back("bmp");
		typelst.push_back("png");
		typelst.push_back("jpeg");
		typelst.push_back("gif");
		typelst.push_back("tiff");
		typelst.push_back("jfif");
		typelst.push_back("tif");
		typelst.push_back("pcx");
		typelst.push_back("tga");
		typelst.push_back("exif");
		typelst.push_back("fpx");
		typelst.push_back("avif");
		for (int i = 0; i < typelst.size(); ++i)
		{
			QString strTextType = "text" + typelst[i];
			QByteArray imagedata64 = event->mimeData()->data(strTextType);
			if (imagedata64.length() > 0)
			{
				QString srtType = typelst[i];
				QImage image;
				QByteArray dataimage = QByteArray::fromBase64(imagedata64);
				image.loadFromData(dataimage);
				std::wstring strText = QString(imagedata64).toStdWString();
				int nWidth = image.width();
				int nHeight = image.height();
				pt = m_SVGDocument.GetLogicPoint(pt);
				std::wstring strxmlImage = L"<image x=\"" + QString::number(pt.x() - nWidth / 2).toStdWString() + L"\"";
				strxmlImage += L" y=\"" + QString::number(pt.y() - nHeight / 2).toStdWString() + L"\"";
				strxmlImage += L" width=\"" + QString::number(nWidth).toStdWString() + L"\"";
				strxmlImage += L" height=\"" + QString::number(nHeight).toStdWString() + L"\"";
				strxmlImage += L" xlink:href=\"data:image/" + srtType.toStdWString();
				strxmlImage += L"; base64, " + strText + L"\"/>";
				m_SVGDocument.Paste(strxmlImage);
			}
		}

		m_pMainFrame->ResetToolBarEnable();
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
		m_timermenuid = startTimer(100);
	}
}

void CSVGView::open(const std::wstring &filepath)
{
	m_SVGDocument.LoadFromFile(filepath);
	QFileInfo fileinfo(QString::fromStdWString(filepath));
	QString strFolder = fileinfo.absolutePath() + "/";
	m_SVGDocument.setLocalFolder(strFolder.toStdWString());
	repaint();
	ResetProperty();
}

void CSVGView::InsertImage(const std::wstring &filepath)
{
	QFile file(QString::fromStdWString(filepath));
	if (file.exists())
	{
		if (file.open(QFile::ReadOnly))
		{
			QByteArray  bytedata = file.readAll();
			QByteArray base64 = bytedata.toBase64();
			file.close();
			QString str = base64;
			std::wstring strdata64 = str.toStdWString();
			m_SVGDocument.InsertImage(strdata64);
			if (m_SVGDocument.IsNeedRepaint())
			{
				repaint();
			}
			m_pMainFrame->ResetToolBarEnable();
		}
	}
}

void CSVGView::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	QRectF rc = this->rect();
	m_SVGDocument.Resize(rc.width(), rc.height());
	repaint();
}

void CSVGView::timerEvent(QTimerEvent *event)
{
	QWidget::timerEvent(event);
	QDateTime dttime = QDateTime::currentDateTime();
	if (dttime.toMSecsSinceEpoch() - m_lstDateTime.toMSecsSinceEpoch() > 500)
	{
		m_lstDateTime = dttime;
		if (m_SVGDocument.HasTextFlash())
		{
			m_SVGDocument.SetShowFalsh(!m_SVGDocument.GetShowFlash());
			repaint();
		}
	}
}

bool CSVGView::eventFilter(QObject *o, QEvent *e)
{
	Q_UNUSED(o);
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent *pKeyEvent = (QKeyEvent *)e;
		OnKeyEvent(pKeyEvent);
	}
	else if (e->type() == QEvent::InputMethod)
	{
		QInputMethodEvent* ievent = dynamic_cast<QInputMethodEvent*>(e);
		if (!ievent->preeditString().isEmpty())
		{
			return true;
		}
		if (!ievent->commitString().isEmpty())
		{
			std::wstring strInput = ievent->commitString().toStdWString();
			m_SVGDocument.InputText(strInput);
			m_lstDateTime = QDateTime::currentDateTime();
			if (m_SVGDocument.IsNeedRepaint())
			{
				repaint();
			}
			return true;
		}
	}
	else if (e->type() == QEvent::Wheel)
	{

	}
	else if (e->type() == QEvent::MouseMove)
	{
		OnMouseMove((QMouseEvent *)e);
	}
	else if (e->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *pMouseEvent = (QMouseEvent *)e;
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			OnLButtonDown(pMouseEvent);
		}
		else if (pMouseEvent->button() == Qt::RightButton)
		{
		}
		else if (pMouseEvent->button() == Qt::MiddleButton)
		{
			OnMiddleButtonDown(pMouseEvent);
		}
	}
	else if (e->type() == QEvent::MouseButtonDblClick)
	{
		QMouseEvent *pMouseEvent = (QMouseEvent *)e;
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			OnLButtonDBClick(pMouseEvent);
		}
	}
	else if (e->type() == QEvent::MouseButtonRelease)
	{
		QMouseEvent *pMouseEvent = (QMouseEvent *)e;
		if (pMouseEvent->button() == Qt::LeftButton)
		{
			OnLButtonUp(pMouseEvent);
		}
		else if (pMouseEvent->button() == Qt::RightButton)
		{
			OnRButtonUp(pMouseEvent);
		}
		else if (pMouseEvent->button() == Qt::MiddleButton)
		{
			OnMiddleButtonUp(pMouseEvent);
		}
	}
	return QWidget::eventFilter(o, e);
}

void CSVGView::OnKeyEvent(QKeyEvent *pKeyEvent)
{
	if (pKeyEvent->matches(QKeySequence::Undo))
	{
		OnUndo();
	}
	else if (pKeyEvent->matches(QKeySequence::SelectAll))
	{
		m_SVGDocument.SelectAll();
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else if (pKeyEvent->matches(QKeySequence::Redo))
	{
		OnRedo();
	}
	else if (pKeyEvent->matches(QKeySequence::Copy))
	{
		OnCopy();
	}
	else if (pKeyEvent->matches(QKeySequence::Paste))
	{
		OnPaste();
	}
	else if (pKeyEvent->key() == Qt::Key_F5)
	{
		ReloadFile();
	}
	else if (pKeyEvent->key() == Qt::Key_Backspace)
	{
		if (m_SVGDocument.InputBackSpace())
		{
			m_lstDateTime = QDateTime::currentDateTime();
			if (m_SVGDocument.IsNeedRepaint())
			{
				repaint();
			}
			ResetProperty();
		}
	}
	else if (pKeyEvent->key() == Qt::Key_Delete)
	{
		OnDel();
	}
	else if (pKeyEvent->key() == Qt::Key_Down)
	{
		if (m_SVGDocument.KeyDown())
		{
			m_lstDateTime = QDateTime::currentDateTime();
		}
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else if (pKeyEvent->key() == Qt::Key_Up)
	{
		if (m_SVGDocument.KeyUp())
		{
			m_lstDateTime = QDateTime::currentDateTime();
		}
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else if (pKeyEvent->key() == Qt::Key_Left)
	{
		if (m_SVGDocument.KeyLeft())
		{
			m_lstDateTime = QDateTime::currentDateTime();
		}
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else if (pKeyEvent->key() == Qt::Key_Right)
	{
		if (m_SVGDocument.KeyRight())
		{
			m_lstDateTime = QDateTime::currentDateTime();
		}
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else if (pKeyEvent->key() == Qt::Key_Escape)
	{
		SetDrawTool(DRAWTOOL_SELECT);
	}
	else
	{
		std::wstring strInput = pKeyEvent->text().toStdWString();
		if (strInput.length() > 0)
		{
			m_SVGDocument.InputText(strInput);
			m_lstDateTime = QDateTime::currentDateTime();
			if (m_SVGDocument.IsNeedRepaint())
			{
				repaint();
			}
		}
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::ResetCursor(MOUSESHAPE mouseshape)
{
	switch (mouseshape)
	{
	case MOUSE_ARROW:
	{
		setCursor(Qt::ArrowCursor);
		break;
	}
	case MOUSE_DRAWPEN:
	{
		QImage drawimage(":/images/drawcursor.png");
		QCursor drawcur(QPixmap::fromImage(drawimage), 10, 10);
		setCursor(drawcur);
		break;
	}
	case MOUSE_ZOOMIN:
	{
		QImage image(":/images/zoomin.png");
		QCursor cur(QPixmap::fromImage(image), 8, 8);
		setCursor(cur);
		break;
	}
	case MOUSE_ZOOMOUT:
	{
		QImage image(":/images/zoomout.png");
		QCursor cur(QPixmap::fromImage(image), 8, 8);
		setCursor(cur);
		break;
	}
	case MOUSE_SELECT:
	{
		QImage image(":/images/select.png");
		QCursor cur(QPixmap::fromImage(image), 8, 8);
		setCursor(cur);
		break;
	}
	case MOUSE_SIZETO:
	{
		QImage drawimage(":/images/movecursor.png");
		QCursor movecur(QPixmap::fromImage(drawimage), 12, 12);
		setCursor(movecur);
		break;
	}
	case MOUSE_OPENHAND:
	{
		QImage drawimage(":/images/hand_open.png");
		QCursor movecur(QPixmap::fromImage(drawimage), 12, 12);
		setCursor(movecur);
		break;
	}
	case MOUSE_CLOSEHAND:
	{
		QImage drawimage(":/images/hand_close.png");
		QCursor movecur(QPixmap::fromImage(drawimage), 12, 12);
		setCursor(movecur);
		break;
	}
	case MOUSE_DRAWCOPY:
	{
		QImage drawimage(":/images/drawcopy.png");
		QCursor movecur(QPixmap::fromImage(drawimage), 12, 12);
		setCursor(movecur);
		break;
	}
	case MOUSE_WE:
	{
		setCursor(Qt::SizeHorCursor);
		break;
	}
	case MOUSE_NS:
	{
		setCursor(Qt::SizeVerCursor);
		break;
	}
	case MOUSE_NE:
	{
		setCursor(Qt::SizeBDiagCursor);
		break;
	}
	case MOUSE_NW:
	{
		setCursor(Qt::SizeFDiagCursor);
		break;
	}
	}
}

void CSVGView::OnMouseMove(QMouseEvent *pMouseEvent)
{
	Qt::KeyboardModifiers   keymodify = QApplication::keyboardModifiers();
	QPoint  pt = mapFromGlobal(pMouseEvent->globalPos());
	MOUSESHAPE mouseshape = m_SVGDocument.OnMouseMove(pt, keymodify.testFlag(Qt::ControlModifier), keymodify.testFlag(Qt::AltModifier));
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	ResetCursor(mouseshape);
	QPoint logpt = m_SVGDocument.GetLogicPoint(pt);
	m_pMainFrame->ShowMousePosition(logpt);
}

void CSVGView::OnLButtonDown(QMouseEvent *pMouseEvent)
{
	QPoint  pt = mapFromGlobal(pMouseEvent->globalPos());
	m_buttondownpt = pt;
	if (m_SVGDocument.GetDrawTool() == DRAWTOOL_CUSTOM)
	{
		QClipboard *clipboard = QApplication::clipboard();
		QString strText = clipboard->text();
		m_SVGDocument.Paste(strText.toStdWString(), pt);
	}
	else
	{
		Qt::KeyboardModifiers   keymodify = QApplication::keyboardModifiers();
		MOUSESHAPE mouseshape = m_SVGDocument.OnMouseLButtonDown(pt, keymodify.testFlag(Qt::ControlModifier), keymodify.testFlag(Qt::AltModifier));

		ResetCursor(mouseshape);
		if (m_timerid >= 0)
		{
			killTimer(m_timerid);
			m_timerid = -1;
		}
		if (m_timerid < 0 && m_SVGDocument.HasTextFlash())
		{
			m_lstDateTime = QDateTime::currentDateTime();
			m_SVGDocument.SetShowFalsh(true);
			m_timerid = startTimer(200);
		}
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnMiddleButtonDown(QMouseEvent *pMouseEvent)
{
	QPoint  pt = mapFromGlobal(pMouseEvent->globalPos());
	MOUSESHAPE mouseshape = m_SVGDocument.OnMouseMButtonDown(pt);
	ResetCursor(mouseshape);
}

void CSVGView::OnMiddleButtonUp(QMouseEvent *pMouseEvent)
{
	QPoint  pt = mapFromGlobal(pMouseEvent->globalPos());
	MOUSESHAPE mouseshape = m_SVGDocument.OnMouseMButtonUp(pt);
	ResetCursor(mouseshape);
}

void CSVGView::OnLButtonDBClick(QMouseEvent *pMouseEvent)
{
	Q_UNUSED(pMouseEvent);
}

void CSVGView::OnLButtonUp(QMouseEvent *pMouseEvent)
{
	Qt::KeyboardModifiers   keymodify = QApplication::keyboardModifiers();
	QPoint  pt = mapFromGlobal(pMouseEvent->globalPos());
	MOUSESHAPE mouseshape = m_SVGDocument.OnMouseLButtonUp(pt, keymodify.testFlag(Qt::ControlModifier));
	if (m_SVGDocument.GetDrawTool() == DRAWTOOL_TEXT)
	{
		m_SVGDocument.SetDrawTool(DRAWTOOL_SELECT);
	}
	repaint();
	ResetCursor(mouseshape);
	ResetProperty();
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::SetDrawTool(DRAWTOOL drawtool)
{
	m_SVGDocument.SetDrawTool(drawtool);
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::ZoomRestore()
{
	m_SVGDocument.OnZoomRestore();
	repaint();
}

void CSVGView::OnRButtonUp(QMouseEvent *pMouseEvent)
{
	DRAWTOOL  lasttool = m_SVGDocument.GetDrawTool();
	m_SVGDocument.SetDrawTool(DRAWTOOL_SELECT);
	ResetCursor(MOUSE_ARROW);

	if (lasttool != DRAWTOOL_SELECT)
		return;

	m_lastmousept = mapFromGlobal(pMouseEvent->globalPos());

	QMenu menu;
	QIcon Icon = QIcon(":/images/editcopy.png");
	menu.addAction(Icon, QString::fromStdWString(L"复制"), this, SLOT(OnCopy()));
	Icon = QIcon(":/images/editpaste.png");
	menu.addAction(Icon, QString::fromStdWString(L"粘贴"), this, SLOT(OnPaste()));
	Icon = QIcon(":/images/delete.png");
	menu.addAction(Icon, QString::fromStdWString(L"删除"), this, SLOT(OnDel()));
	menu.addSeparator();
	Icon = QIcon(":/images/editundo.png");
	menu.addAction(Icon, QString::fromStdWString(L"撤销"), this, SLOT(OnUndo()));
	Icon = QIcon(":/images/editredo.png");
	menu.addAction(Icon, QString::fromStdWString(L"反撤销"), this, SLOT(OnRedo()));
	menu.addSeparator();
	Icon = QIcon(":/images/group.png");
	menu.addAction(Icon, QString::fromStdWString(L"组合"), this, SLOT(OnGroup()));
	Icon = QIcon(":/images/ungroup.png");
	menu.addAction(Icon, QString::fromStdWString(L"解除组合"), this, SLOT(OnUnGroup()));
	menu.addSeparator();

	QMenu * pSubMenu = menu.addMenu(QString::fromStdWString(L"绘制工具"));
	Icon = QIcon(":/images/drawline.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"直线"), this, SLOT(OnDrawLine()));
	Icon = QIcon(":/images/drawrect.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"矩形"), this, SLOT(OnDrawRect()));
	Icon = QIcon(":/images/drawcircle.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"圆"), this, SLOT(OnDrawCircle()));
	Icon = QIcon(":/images/drawellipse.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"椭圆"), this, SLOT(OnDrawEllipse()));
	Icon = QIcon(":/images/drawpolyline.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"折线"), this, SLOT(OnDrawPolyline()));
	Icon = QIcon(":/images/drawpolygon.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"多边形"), this, SLOT(OnDrawPolygon()));
	Icon = QIcon(":/images/drawpath.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"贝塞尔曲线"), this, SLOT(OnDrawPath()));
	Icon = QIcon(":/images/drawtext.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"文本"), this, SLOT(OnDrawText()));
	Icon = QIcon(":/images/drawimage.png");
	pSubMenu->addAction(Icon, QString::fromStdWString(L"图片"), this, SLOT(OnDrawImage()));
	Icon = QIcon(":/images/drawcontinue.png");
	QAction  *pActionContinue = pSubMenu->addAction(Icon, QString::fromStdWString(L"继续绘制"), this, SLOT(OnDrawContinue()));
	pActionContinue->setEnabled(false);
	if (CanContinueDraw())
	{
		pActionContinue->setEnabled(true);
	}
	menu.addSeparator();

	CSVGElement *pElement = m_SVGDocument.GetAloneSelectElement(true);
	if (pElement && pElement->getNodeType() == SVG_IMAGE)
	{
		menu.addAction(QString::fromStdWString(L"替换图片"), this, SLOT(OnReplaceImage()));
	}
	menu.exec(pMouseEvent->globalPos());
}

void CSVGView::OnReplaceImage()
{
	CSVGElement *pElement = m_SVGDocument.GetAloneSelectElement(true);
	if (pElement && pElement->getNodeType() == SVG_IMAGE)
	{
		QFileDialog fileDialog(this, QString::fromStdWString(L"打开文件..."), ".", tr("image (*.png *.jpg *.bmp);;All(*.*)"));
		fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
		fileDialog.setFileMode(QFileDialog::ExistingFile);
		if (fileDialog.exec() != QDialog::Accepted)
			return;
		QString strFilePath = fileDialog.selectedFiles().first();
		QFile file(strFilePath);
		if (file.exists())
		{
			if (file.open(QFile::ReadOnly))
			{
				QByteArray  bytedata = file.readAll();
				QByteArray base64 = bytedata.toBase64();
				file.close();
				QString str = base64;
				std::wstring strdata64 = str.toStdWString();
				CSVGElementImage *pImageElement = (CSVGElementImage*)pElement;
				int nwidth = 0;
				int nheight = 0;
				if (strdata64.length() > 0)
				{
					QImage image = CSVGElementImage::LoadFromBase64(strdata64);
					nwidth = image.width();
					nheight = image.height();
				}
				pImageElement->setAttribute(L"width", global.itostring(nwidth));
				pImageElement->setAttribute(L"height", global.itostring(nheight));
				pImageElement->setAttribute(L"xlink:href", L"data:image/png;base64," + strdata64);

				pImageElement->resetPath();
				m_SVGDocument.ClearAllSelect();
				m_SVGDocument.AddToSelect(pImageElement, Qt::green);
				repaint();
			}
		}
	}
}

void CSVGView::OnCanvasNoBack()
{
	m_SVGDocument.SetHasCanvasBack(false);
}

void CSVGView::OnCanvasDeepBlue()
{
	m_SVGDocument.SetHasCanvasBack(true);
	m_SVGDocument.SetCanvasBackColor(QColor(14, 18, 65));
	repaint();
}

void CSVGView::OnCanvasMidBlue()
{
	m_SVGDocument.SetHasCanvasBack(true);
	m_SVGDocument.SetCanvasBackColor(QColor(21, 28, 100));
	repaint();
}

void CSVGView::OnCanvasLightBlue()
{
	m_SVGDocument.SetHasCanvasBack(true);
	m_SVGDocument.SetCanvasBackColor(QColor(29, 36, 133));
	repaint();
}

void CSVGView::OnCanvasWhite()
{
	m_SVGDocument.SetHasCanvasBack(true);
	m_SVGDocument.SetCanvasBackColor(Qt::white);
	repaint();
}

void CSVGView::OnCanvasDeepBlack()
{
	m_SVGDocument.SetHasCanvasBack(true);
	m_SVGDocument.SetCanvasBackColor(QColor(16, 32, 52));
	repaint();
}

void CSVGView::OnCanvasMoreColor()
{
	QColorDialog dlg;
	if (dlg.exec() == QDialog::Accepted)
	{
		m_SVGDocument.SetHasCanvasBack(true);
		QColor color = dlg.currentColor();
		m_SVGDocument.SetCanvasBackColor(color);
	}
}

void CSVGView::OnCopy()
{
	std::wstring strret = m_SVGDocument.Copy();
	if (strret.length() > 0)
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(QString::fromStdWString(strret));
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnPaste()
{
	QClipboard *clipboard = QApplication::clipboard();
	QString strText = clipboard->text();
	if (strText.length() > 0)
	{
		std::wstring strxml = strText.toStdWString();
		m_SVGDocument.Paste(strxml);
		if (m_SVGDocument.IsNeedRepaint())
		{
			repaint();
		}
	}
	else
	{
		QImage image = clipboard->image();
		if (!image.isNull())
		{
			QPoint pt = m_SVGDocument.GetLogicPoint(m_buttondownpt);
			QByteArray ba;
			QBuffer buffer(&ba);
			image.save(&buffer, "PNG");
			QString str = ba.toBase64();
			std::wstring strdata64 = str.toStdWString();
			int nWidth = image.width();
			int nHeight = image.height();
			std::wstring strxmlImage = L"<image x=\"" + QString::number(pt.x()).toStdWString() + L"\"";
			strxmlImage += L" y=\"" + QString::number(pt.y()).toStdWString() + L"\"";
			strxmlImage += L" width=\"" + QString::number(nWidth).toStdWString() + L"\"";
			strxmlImage += L" height=\"" + QString::number(nHeight).toStdWString() + L"\"";
			strxmlImage += L" xlink:href=\"data:image/png;base64," + strdata64 + L"\"/>";
			m_SVGDocument.Paste(strxmlImage);
			if (m_SVGDocument.IsNeedRepaint())
			{
				repaint();
			}
		}
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnUndo()
{
	m_SVGDocument.Undo();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnRedo()
{
	m_SVGDocument.Redo();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnDel()
{
	if (m_SVGDocument.Delete())
	{
		m_lstDateTime = QDateTime::currentDateTime();
	}
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	ResetProperty();
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnGroup()
{
	Group();
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::OnUnGroup()
{
	UnGroup();
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::wheelEvent(QWheelEvent *event)
{
	int nDelta = event->delta();
	QPoint pt = mapFromGlobal(event->globalPos());
	m_SVGDocument.OnMouseWheel(pt, nDelta);
	repaint();
}

void CSVGView::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	m_SVGDocument.PrepareDraw();
	m_SVGDocument.Draw(&painter);
	return QWidget::paintEvent(event);
}

bool CSVGView::Save(QString &strName)
{
	bool bSaveOk = false;
	if (m_EditPath.strFullPath.length() > 0)
	{
		std::wstring strxmlhead = L"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n";
		std::wstring strxml = m_SVGDocument.Toxml();
		QString str = QString::fromStdWString(strxmlhead + strxml);
		SaveFileTextToUtf8(m_EditPath.strFullPath, str);
		m_SVGDocument.SetModify(false);
		bSaveOk = true;
	}
	if (!bSaveOk)
	{
		return SaveAs(strName);
	}
	return true;
}

bool CSVGView::SaveAs(QString &strName)
{
	bool bRet = false;
	QString  strLstDir = tr("/newfile.svg");
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromStdWString(L"另存为"), strLstDir, QString::fromStdWString(L"svg(*.svg)"));
	m_EditPath.strFullPath = fileName;
	if (m_EditPath.strFullPath.length() > 0)
	{
		bRet = Save(strName);
		if (bRet)
		{
			QFileInfo fileInfo(fileName);
			strName = fileInfo.fileName();
		}
	}
	return bRet;
}

void CSVGView::SaveAsPng()
{
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromStdWString(L"另存为"), QString::fromStdWString(L"新文件"), QString::fromStdWString(L"png(*.png)"));
	std::wstring strxml = m_SVGDocument.ToFileXml();
	QImage image = CSVGDocument::SVGFileAsImage(strxml, L"png");
	image.save(fileName);
}

void CSVGView::SaveAsPng(int nwidth, int nheight)
{
	QString fileName = QFileDialog::getSaveFileName(this, QString::fromStdWString(L"另存为"), QString::fromStdWString(L"新文件"), QString::fromStdWString(L"png(*.png)"));
	std::wstring strxml = m_SVGDocument.ToFileXml();
	QImage image = CSVGDocument::SVGFileAsImage(strxml, L"png", nwidth, nheight);
	image.save(fileName);
}

void CSVGView::SaveAsJpg()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("save as"), tr("new file"), QString::fromStdWString(L"jpg(*.jpg)"));
	std::wstring strxml = m_SVGDocument.ToFileXml();
	QImage image = CSVGDocument::SVGFileAsImage(strxml, L"jpg");
	image.save(fileName, "JPG", 100);
}

void CSVGView::DisConnectPanProperty()
{
	if (m_pMainFrame)
	{
		void OntextChanged(const QString &strName, const QString &strValue);
		disconnect(m_pMainFrame->m_RightProperty, SIGNAL(textChanged(const QString &, const QString &)), this, SLOT(OntextChanged(const QString &, const QString &)));
	}
}

void CSVGView::ConnectPanProperty()
{
	DisConnectPanProperty();
	if (m_pMainFrame)
	{
		void OntextChanged(const QString &strName, const QString &strValue);
		connect(m_pMainFrame->m_RightProperty, SIGNAL(textChanged(const QString &, const QString &)), this, SLOT(OntextChanged(const QString &, const QString &)));
	}
}

void CSVGView::OntextChanged(const QString &strName, const QString &strValue)
{
	std::vector<std::wstring>AttrNames;
	std::vector<std::wstring>AttrValues;
	AttrNames.push_back(strName.toStdWString());
	AttrValues.push_back(strValue.toStdWString());
	m_SVGDocument.SetAttributes(AttrNames, AttrValues, false);
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::ReloadFile()
{
	if (m_SVGDocument.IsModify())
	{
		if (!m_pMainFrame->ShowSaveMessage(this))
		{
			return;
		}
	}
	if (m_EditPath.strFullPath.length() > 0)
	{
		open(m_EditPath.strFullPath.toStdWString());
		m_pMainFrame->ResetViewFileMark(this);
	}
}

bool CSVGView::IsModify()
{
	return m_SVGDocument.IsModify();
}

void CSVGView::SetAttributes(const std::vector<std::wstring>&AttrNames, const std::vector<std::wstring>&AttrValues, bool bChangeDefult)
{
	m_SVGDocument.SetAttributes(AttrNames, AttrValues, bChangeDefult);
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::FillUrl(const std::wstring &strxml)
{
	m_SVGDocument.FillUrl(strxml);
	m_pMainFrame->ResetToolBarEnable();
	repaint();
}

void CSVGView::OnRotate(int nValue, bool bFinish)
{
	m_SVGDocument.Rotate(nValue, bFinish);
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::OnZoomSize(int nWidth, int nHeight, bool bFinish)
{
	m_SVGDocument.ZoomSizeFixValue(nWidth, nHeight, bFinish);
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}


void CSVGView::OnDrawLine()
{
	SetDrawTool(DRAWTOOL_LINE);
}

void CSVGView::OnDrawRect()
{
	SetDrawTool(DRAWTOOL_RECT);
}

void CSVGView::OnDrawCircle()
{
	SetDrawTool(DRAWTOOL_CIRCLE);
}

void CSVGView::OnDrawEllipse()
{
	SetDrawTool(DRAWTOOL_ELLIPSE);
}

void CSVGView::OnDrawPolyline()
{
	SetDrawTool(DRAWTOOL_POLYLINE);
}

void CSVGView::OnDrawPolygon()
{
	SetDrawTool(DRAWTOOL_POLYGON);
}

void CSVGView::OnDrawPath()
{
	SetDrawTool(DRAWTOOL_PATH);
}

void CSVGView::OnDrawText()
{
	SetDrawTool(DRAWTOOL_TEXT);
}

void CSVGView::OnDrawImage()
{
	QFileDialog fileDialog(this, QString::fromStdWString(L"打开文件..."), ".", tr("image (*.png *.jpg *.bmp);;All(*.*)"));
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	if (fileDialog.exec() != QDialog::Accepted)
		return;
	QString strFilePath = fileDialog.selectedFiles().first();
	InsertImage(strFilePath.toStdWString());
}

void CSVGView::OnDrawContinue()
{
	CSVGElement *pElement = m_SVGDocument.GetAloneSelectElement();
	if (pElement)
	{
		if (pElement->getNodeType() == SVG_POLYLINE)
		{
			m_SVGDocument.SetDrawTool(DRAWTOOL_POLYLINEING);
		}
		else if (pElement->getNodeType() == SVG_POLYGON)
		{
			m_SVGDocument.SetDrawTool(DRAWTOOL_POLYGONING);
		}
		else if (pElement->getNodeType() == SVG_PATH)
		{
			m_SVGDocument.SetDrawTool(DRAWTOOL_PATHING);
		}
	}
}

void CSVGView::LayerTop()
{
	m_SVGDocument.LayerTop();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::LayerBottom()
{
	m_SVGDocument.LayerBottom();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::LayerUp()
{
	m_SVGDocument.LayerUP();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::LayerDown()
{
	m_SVGDocument.LayerDown();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::TurnRight()
{
	m_SVGDocument.TurnRight();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::TurnLeft()
{
	m_SVGDocument.TurnLeft();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::MirrorRight()
{
	m_SVGDocument.MirrorRight();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::MirrorBottom()
{
	m_SVGDocument.MirrorBottom();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::Group()
{
	m_SVGDocument.Group();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::UnGroup()
{
	m_SVGDocument.UnGroup();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
	m_pMainFrame->ResetToolBarEnable();
}

void CSVGView::LeftAlign()
{
	m_SVGDocument.LeftAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::TopAlign()
{
	m_SVGDocument.TopAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::RightAlign()
{
	m_SVGDocument.RightAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::BottomAlign()
{
	m_SVGDocument.BottomAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::VerLineAlign()
{
	m_SVGDocument.VerLineAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::HorLineAlign()
{
	m_SVGDocument.HorLineAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::HorSpaceAlign()
{
	m_SVGDocument.HorSpaceAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::VerSpaceAlign()
{
	m_SVGDocument.VerSpaceAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::SameWidthAlign()
{
	m_SVGDocument.SameWidthAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::SameHeightAlign()
{
	m_SVGDocument.SameHeightAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

void CSVGView::SameSizeAlign()
{
	m_SVGDocument.SameSizeAlign();
	if (m_SVGDocument.IsNeedRepaint())
	{
		repaint();
	}
}

bool CSVGView::CanUndo()
{
	return m_SVGDocument.CanUndo();
}

bool CSVGView::CanRedo()
{
	return m_SVGDocument.CanRedo();
}

bool CSVGView::CanCopy()
{
	if (m_SVGDocument.GetSelectElements().size() > 0)
		return true;
	return false;
}

bool CSVGView::CanPaste()
{
	QClipboard *clipboard = QApplication::clipboard();
	QString strText = clipboard->text();
	if (strText.length() > 0 && strText.indexOf("<") >= 0)
	{
		return true;
	}
	else
	{
		if (!clipboard->image().isNull())
		{
			return true;
		}
	}
	return false;
}

bool CSVGView::CanContinueDraw()
{
	CSVGElement *pElement = m_SVGDocument.GetAloneSelectElement();
	if (pElement && (pElement->getNodeType() == SVG_POLYLINE
		|| pElement->getNodeType() == SVG_POLYGON
		|| pElement->getNodeType() == SVG_PATH))
	{
		return true;
	}
	return false;
}

bool CSVGView::CanRotate()
{
	if (m_SVGDocument.GetSelectElements().size() > 0)
		return true;
	return false;
}

bool CSVGView::CanGroup()
{
	if (m_SVGDocument.GetSelectElements().size() >= 2)
		return true;
	return false;
}

bool CSVGView::CanUnGroup()
{
	QVector<CSVGElement *> vcShapes = m_SVGDocument.GetSelectElements();
	for (int i = 0; i < vcShapes.size(); ++i)
	{
		if (vcShapes[i]->getNodeType() == SVG_G)
		{
			return true;
		}
	}
	return false;
}

bool CSVGView::HasSelectShape()
{
	if (m_SVGDocument.GetSelectElements().size() > 0)
		return true;
	return false;
}

bool CSVGView::HasSelectMoreOneShape()
{
	if (m_SVGDocument.GetSelectElements().size() >= 2)
		return true;
	return false;
}

bool CSVGView::hasSelectMoveTwoShape()
{
	if (m_SVGDocument.GetSelectElements().size() >= 3)
		return true;
	return false;
}

void CSVGView::ResetProperty()
{
	m_pMainFrame->m_RightProperty->LockNotify(false);

	QVector<CSVGElement *>vcSelectElement = m_SVGDocument.GetSelectElements();
	if (vcSelectElement.size() == 0)
	{
		m_pMainFrame->m_RightProperty->ClearProperty();
		if (m_lstsvgtype != 5000)
		{
			m_pMainFrame->m_RightProperty->ClearProperty();
			m_lstsvgtype = 5000;
		}
		m_pMainFrame->m_RightProperty->AddHeadName(QString::fromStdWString(L"画布大小"));

		CSVGElement *pSVGElement = m_SVGDocument.GetRootSVGElement();
		if (pSVGElement)
		{
			std::wstring  strwidth = pSVGElement->getAttribute(L"width");
			std::wstring  strheight = pSVGElement->getAttribute(L"height");
			if (strwidth.length() == 0 || global.atof_3(strwidth) <= 1.1
				|| strheight.length() == 0 || global.atof_3(strheight) <= 1.1)
			{
				std::wstring  strViewBox = pSVGElement->getAttribute(L"viewBox");
				std::vector<std::wstring> vcSize = global.tokenize(strViewBox, L" ");
				if (vcSize.size() == 4)
				{
					strwidth = vcSize[2];
					strheight = vcSize[3];
				}
			}
			m_pMainFrame->m_RightProperty->AddProperty("width", QString::fromStdWString(strwidth));
			m_pMainFrame->m_RightProperty->AddProperty("height", QString::fromStdWString(strheight));
		}
	}
	if (vcSelectElement.size() == 1)
	{
		CSVGElement * pChild = m_SVGDocument.GetSelectChild();
		CSVGElement * pElement = vcSelectElement[0];
		if (pChild)
		{
			pElement = pChild;
		}
		if (pElement)
		{
			m_pMainFrame->m_RightProperty->ClearProperty();
			std::wstring strTagName = pElement->getTagName();
			QString strLabel = QString::fromStdWString(L"图形属性");
			strLabel += QString::fromStdWString(strTagName);
			strLabel += ")";
			m_pMainFrame->m_RightProperty->AddHeadName(strLabel);

			std::set<std::wstring> setNames = pElement->getAttrNames();
			std::set<std::wstring>::iterator iter = setNames.begin();

			std::vector<std::wstring>vcImport;
			vcImport.push_back(L"id");
			vcImport.push_back(L"name");
			vcImport.push_back(L"x");
			vcImport.push_back(L"y");
			vcImport.push_back(L"x1");
			vcImport.push_back(L"y1");
			vcImport.push_back(L"x2");
			vcImport.push_back(L"y2");
			vcImport.push_back(L"cx");
			vcImport.push_back(L"cy");
			vcImport.push_back(L"r");
			vcImport.push_back(L"d");
			vcImport.push_back(L"points");
			vcImport.push_back(L"width");
			vcImport.push_back(L"height");
			vcImport.push_back(L"rx");
			vcImport.push_back(L"ry");
			vcImport.push_back(L"transform");
			vcImport.push_back(L"fill");
			vcImport.push_back(L"fill-opacity");
			vcImport.push_back(L"stroke");
			vcImport.push_back(L"stroke-opacity");
			vcImport.push_back(L"stroke-width");
			vcImport.push_back(L"stroke-dasharray");

			std::vector<std::wstring>vcName;
			for (int i = 0; i < (int)vcImport.size(); ++i)
			{
				if (setNames.find(vcImport[i]) != setNames.end())
				{
					vcName.push_back(vcImport[i]);
					setNames.erase(vcImport[i]);
				}
			}

			for (; iter != setNames.end(); ++iter)
			{
				vcName.push_back(*iter);
			}
			std::vector<std::wstring>vcNewName;
			std::vector<std::wstring>vcValue;
			for (int i = 0; i < (int)vcName.size(); ++i)
			{
				std::wstring strAttrName = vcName[i];
				QString strQAttrName = QString::fromStdWString(strAttrName);
				if (strQAttrName.indexOf("ty") != 0)
				{
					std::wstring strValue = pElement->getAttribute(strAttrName);
					vcNewName.push_back(vcName[i]);
					vcValue.push_back(strValue);
				}
			}
			std::wstring strTextLabel;
			std::wstring strText;
			if (pElement->getNodeType() == SVG_TEXT)
			{
				strTextLabel = L"textcontent";
				strText = ((CSVGElementText*)pElement)->GetText();
				vcNewName.push_back(L"textcontent");
				vcValue.push_back(strText);
				std::wstring strFamily = pElement->getAttribute(L"font-family");
				std::wstring strFontSize = pElement->getAttribute(L"font-size");
				m_pMainFrame->SetCurrentFontName(QString::fromStdWString(strFamily));
				m_pMainFrame->SetCurrentFontSize(QString::fromStdWString(strFontSize));
			}
			for (int i = 0; i < (int)vcNewName.size(); ++i)
			{
				m_pMainFrame->m_RightProperty->AddProperty(QString::fromStdWString(vcNewName[i]), QString::fromStdWString(vcValue[i]));
			}
		}
	}
	m_pMainFrame->m_RightProperty->LockNotify(true);
}

std::wstring CSVGView::GetSelectWidth()
{
	return m_SVGDocument.GetSelectWidth();
}

std::wstring CSVGView::GetSelectHeight()
{
	return m_SVGDocument.GetSelectHeight();
}

void CSVGView::ShowGrid()
{
	bool bShowGrid = m_SVGDocument.IsShowGrid();
	m_SVGDocument.SetShowGrid(!bShowGrid);
	repaint();
}

void CSVGView::ShowRule()
{
	bool bShowRule = m_SVGDocument.IsShowRule();
	m_SVGDocument.SetShowRule(!bShowRule);
	repaint();
}

bool CSVGView::HasLockShape()
{
	return m_SVGDocument.HasLockShape();
}

void CSVGView::Lock()
{
	m_SVGDocument.Lock();
	repaint();
}

void CSVGView::UnLock()
{
	m_SVGDocument.UnLock();
}
