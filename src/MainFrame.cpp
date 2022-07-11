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
#include "MainFrame.h"
#include <QPainter>
#include <QDockWidget>
#include <QFileDialog>
#include <QTextCodec>
#include <QDomDocument>
#include <QFontComboBox>
#include <QToolButton>
#include <QDesktopServices> 
#include <QProcess>
#include <QMessageBox>
#include <QImage>
#include <QClipboard>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>

#include "ToolButtonColorMenu.h"
#include "ToolButtonComboxMenu.h"
#include "NewFileView.h"
#include "SVGView.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif
#include "main.h"
#include "DlgAbout.h"
#include "MenuRotate.h"

CMainFrame::CMainFrame(QWidget *parent)
	: QMainWindow(parent)
{
	m_pactionSave = NULL;
	m_pActionundo = NULL;
	m_pActionredo = NULL;
	m_pActioncopy = NULL;
	m_pActionpaste = NULL;
	m_pActionundo = NULL;
	m_pActionDrawLine = NULL;
	m_pActionDrawRect = NULL;
	m_pActionDrawCircle = NULL;
	m_pActionDrawEllipse = NULL;
	m_pActionDrawPolyline = NULL;
	m_pActionDrawPolygon = NULL;
	m_pActionDrawpath = NULL;
	m_pActionDrawtext = NULL;
	m_pActionDrawImage = NULL;
	m_pActionDrawContinue = NULL;
	m_pActionArrow = NULL;
	m_pActionRestore = NULL;
	m_pActionZoomin = NULL;
	m_pActionZoomout = NULL;
	m_pActionZoommove = NULL;
	m_pActionRotate = NULL;
	m_pActionGroup = NULL;
	m_pActionUnGroup = NULL;
	m_pActionMoveTop = NULL;
	m_pActionMoveBottom = NULL;
	m_pActionMoveup = NULL;
	m_pActionMoveDown = NULL;
	m_pActionTurnRight = NULL;
	m_pActionTurnLeft = NULL;
	m_pActionMoveHor = NULL;
	m_pActionMoveVer = NULL;
	m_pActionAlignTop = NULL;
	m_pActionAlignBottom = NULL;
	m_pActionAlignLeft = NULL;
	m_pActionAlignRight = NULL;
	m_pActionAlignHorMid = NULL;
	m_pActionAlignVerMid = NULL;
	m_pActionAlignHorSame = NULL;
	m_pActionAlignVerSame = NULL;
	m_pActionAlignWidthSame = NULL;
	m_pActionAlignHeightSame = NULL;
	m_pActionAlignSizeSame = NULL;

	m_pActionLock = NULL;
	m_pActionUnLock = NULL;

	this->startTimer(200);
	m_bLockTabSelChange = false;
	m_bShowCanvasRule = true;
	m_bShowCanvasGrid = false;
	m_statusLabelmousept = NULL;
	setWindowTitle(tr("QtSVGEditor"));
	createDockWindows();

	setupFileActions();
	setupViewActions();
	setupEditActions();
	setupDrawActions();
	setupZoomActions();
	setupGroupActions();
	setupFontActions();
	addToolBarBreak();
	setupAlignActions();
	setupColorActions();
	setupCanvasActions();
	setupLockActions();
	setupHelp();
	setupStatusBar();

	m_pcurrentView = NULL;
	m_pTabWidget = new CTabWidgetEx(this);

	m_pTabWidget->setTabsClosable(true);
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabcurrentChanged(int)));

	m_pTabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pTabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OncustomContextMenuRequested(const QPoint&)));

	CNewFileView *pNewView = new CNewFileView();
	m_pTabWidget->addTab(pNewView, QString::fromStdWString(L"╋"));

	((QTabBar*)(m_pTabWidget->tabBar()))->setTabButton(m_pTabWidget->indexOf(pNewView), QTabBar::RightSide, NULL);

	connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClose(int)));

	setCentralWidget(m_pTabWidget);
	setAcceptDrops(true);
}

void CMainFrame::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasUrls())
	{
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
	}
}

void CMainFrame::dropEvent(QDropEvent *event)
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
				OpenFile(strPath);
			}
		}
	}
}

void CMainFrame::setupFileActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"文件"));
	QMenu *menu = menuBar()->addMenu(QString::fromStdWString(L"文件(&F)"));

	const QIcon newIcon = QIcon(":/images/filenew.png");
	QAction *a = menu->addAction(newIcon, QString::fromStdWString(L"新建(&N)"), this, SLOT(fileNew()));
	tb->addAction(a);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(QKeySequence::New);

	const QIcon openIcon = QIcon(":/images/fileopen.png");
	a = menu->addAction(openIcon, QString::fromStdWString(L"打开(&O)"), this, SLOT(fileOpen()));
	a->setShortcut(QKeySequence::Open);
	tb->addAction(a);

	menu->addSeparator();

	const QIcon saveIcon = QIcon(":/images/filesave.png");
	m_pactionSave = menu->addAction(saveIcon, QString::fromStdWString(L"保存(&S)"), this, SLOT(fileSave()));
	m_pactionSave->setShortcut(QKeySequence::Save);
	m_pactionSave->setEnabled(true);
	tb->addAction(m_pactionSave);

	a = menu->addAction(QString::fromStdWString(L"另存为"), this, SLOT(fileSaveAs()));
	a->setPriority(QAction::LowPriority);
	menu->addSeparator();
	m_pActionExportPng = menu->addAction(QString::fromStdWString(L"导出PNG"), this, SLOT(fileSaveASPng()));
	a->setPriority(QAction::LowPriority);
	m_pActionExportJpg = menu->addAction(QString::fromStdWString(L"导出JPG"), this, SLOT(fileSaveASJpg()));
	a->setPriority(QAction::LowPriority);

	menu->addSeparator();


	a = menu->addAction(QString::fromStdWString(L"退出"), this, SLOT(close()));
	a->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void CMainFrame::setupEditActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"编辑"));
	QMenu *menu = menuBar()->addMenu(QString::fromStdWString(L"编辑(&E)"));
	QIcon Icon = QIcon(":/images/editundo.png");
	m_pActionundo = menu->addAction(Icon, QString::fromStdWString(L"撤销(&U)"), this, SLOT(undo()));
	tb->addAction(m_pActionundo);


	Icon = QIcon(":/images/editredo.png");
	m_pActionredo = menu->addAction(Icon, QString::fromStdWString(L"反撤销(&V)"), this, SLOT(redo()));
	tb->addAction(m_pActionredo);
	menu->addSeparator();

	Icon = QIcon(":/images/editcopy.png");
	m_pActioncopy = menu->addAction(Icon, QString::fromStdWString(L"复制(&C)"), this, SLOT(copy()));
	tb->addAction(m_pActioncopy);

	Icon = QIcon(":/images/editpaste.png");
	m_pActionpaste = menu->addAction(Icon, QString::fromStdWString(L"粘贴(&V)"), this, SLOT(paste()));
	tb->addAction(m_pActionpaste);
}

void CMainFrame::setupViewActions()
{
	QMenu *menu = menuBar()->addMenu(QString::fromStdWString(L"视图(&V)"));
	connect(menu, SIGNAL(aboutToShow()), this, SLOT(OnMenuViewToShow()));

	m_ActionPropertyView = menu->addAction(QString::fromStdWString(L"属性视图"), this, SLOT(perpertyview()));
	m_ActionPropertyView->setCheckable(true);
}

void CMainFrame::setupDrawActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"绘制"));
	QIcon icon = QIcon(":/images/drawline.png");
	m_pActionDrawLine = tb->addAction(icon, QString::fromStdWString(L"直线"), this, SLOT(drawline()));

	icon = QIcon(":/images/drawrect.png");
	m_pActionDrawRect = tb->addAction(icon, QString::fromStdWString(L"矩形"), this, SLOT(drawrect()));

	icon = QIcon(":/images/drawcircle.png");
	m_pActionDrawCircle = tb->addAction(icon, QString::fromStdWString(L"圆"), this, SLOT(drawcircle()));

	icon = QIcon(":/images/drawellipse.png");
	m_pActionDrawEllipse = tb->addAction(icon, QString::fromStdWString(L"椭圆"), this, SLOT(drawellipse()));

	icon = QIcon(":/images/drawpolyline.png");
	m_pActionDrawPolyline = tb->addAction(icon, QString::fromStdWString(L"折线"), this, SLOT(drawpolyline()));

	icon = QIcon(":/images/drawpolygon.png");
	m_pActionDrawPolygon = tb->addAction(icon, QString::fromStdWString(L"多边形"), this, SLOT(drawpolygon()));

	icon = QIcon(":/images/drawpath.png");
	m_pActionDrawpath = tb->addAction(icon, QString::fromStdWString(L"贝塞尔曲线"), this, SLOT(drawpath()));

	icon = QIcon(":/images/drawtext.png");
	m_pActionDrawtext = tb->addAction(icon, QString::fromStdWString(L"文本"), this, SLOT(drawtext()));


	icon = QIcon(":/images/drawimage.png");
	m_pActionDrawImage = tb->addAction(icon, QString::fromStdWString(L"图片"), this, SLOT(drawimage()));

	icon = QIcon(":/images/drawcontinue.png");
	m_pActionDrawContinue = tb->addAction(icon, QString::fromStdWString(L"继续绘制"), this, SLOT(drawcontinue()));
}

void CMainFrame::setupZoomActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"放大"));

	QIcon icon = QIcon(":/images/arrow.png");
	m_pActionArrow = tb->addAction(icon, QString::fromStdWString(L"箭头鼠标"), this, SLOT(arrow()));
	tb->addSeparator();

	icon = QIcon(":/images/zoomrestore.png");
	m_pActionRestore = tb->addAction(icon, QString::fromStdWString(L"恢复缩放"), this, SLOT(zoomrestore()));

	icon = QIcon(":/images/zoomin.png");
	m_pActionZoomin = tb->addAction(icon, QString::fromStdWString(L"放大"), this, SLOT(zoomin()));

	icon = QIcon(":/images/zoomout.png");
	m_pActionZoomout = tb->addAction(icon, QString::fromStdWString(L"缩小"), this, SLOT(zoomout()));

	icon = QIcon(":/images/move.png");
	m_pActionZoommove = tb->addAction(icon, QString::fromStdWString(L"移动"), this, SLOT(zoommove()));

	tb->addSeparator();
	icon = QIcon(":/images/rotate.png");
	m_pActionRotate = tb->addAction(icon, QString::fromStdWString(L"旋转"), this, SLOT(rotate()));
}

void CMainFrame::setupGroupActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"组合"));
	QIcon icon = QIcon(":/images/group.png");
	m_pActionGroup = tb->addAction(icon, QString::fromStdWString(L"组合"), this, SLOT(OnGroup()));

	icon = QIcon(":/images/ungroup.png");
	m_pActionUnGroup = tb->addAction(icon, QString::fromStdWString(L"解除组合"), this, SLOT(OnUnGroup()));
	tb->addSeparator();

	icon = QIcon(":/images/layertop.png");
	m_pActionMoveTop = tb->addAction(icon, QString::fromStdWString(L"移到顶层"), this, SLOT(layertop()));

	icon = QIcon(":/images/layerbottom.png");
	m_pActionMoveBottom = tb->addAction(icon, QString::fromStdWString(L"移到底层"), this, SLOT(layerbottom()));

	icon = QIcon(":/images/layerup.png");
	m_pActionMoveup = tb->addAction(icon, QString::fromStdWString(L"上移一层"), this, SLOT(layerup()));

	icon = QIcon(":/images/layerdown.png");
	m_pActionMoveDown = tb->addAction(icon, QString::fromStdWString(L"下移一层"), this, SLOT(layerdown()));
	tb->addSeparator();

	icon = QIcon(":/images/turnoverhor.png");
	m_pActionMoveHor = tb->addAction(icon, QString::fromStdWString(L"水平翻转"), this, SLOT(mirrorright()));

	icon = QIcon(":/images/turnoverver.png");
	m_pActionMoveVer = tb->addAction(icon, QString::fromStdWString(L"垂直翻转"), this, SLOT(mirrorbottom()));

	icon = QIcon(":/images/turnright.png");
	m_pActionTurnRight = tb->addAction(icon, QString::fromStdWString(L"向右旋转"), this, SLOT(turnright()));

	icon = QIcon(":/images/turnleft.png");
	m_pActionTurnLeft = tb->addAction(icon, QString::fromStdWString(L"向左旋转"), this, SLOT(turnleft()));
}
void CMainFrame::setupFontActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"字体"));

	typedef void (QComboBox::*QComboStringSignal)(const QString &);

	m_comboFont = new QFontComboBox(tb);
	tb->addWidget(m_comboFont);
	m_comboFont->setFixedWidth(150);
	connect(m_comboFont, static_cast<QComboStringSignal>(&QComboBox::activated), this, &CMainFrame::OntextFamilyChange);


	m_comboSize = new QComboBox(tb);
	m_comboSize->setObjectName("comboSize");
	tb->addWidget(m_comboSize);
	m_comboSize->setEditable(true);
	m_comboSize->setFixedWidth(80);

	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach(int size, standardSizes)
	{
		m_comboSize->addItem(QString::number(size));
	}
	m_comboSize->setCurrentText("24");
	connect(m_comboSize, static_cast<QComboStringSignal>(&QComboBox::activated), this, &CMainFrame::OntextFontSizeChange);


	const QIcon boldIcon = QIcon(":/images/textbold.png");
	m_pActionTextBold = tb->addAction(boldIcon, QString::fromStdWString(L"加粗"), this, SLOT(OntextBold()));
	QFont bold;
	bold.setBold(true);
	m_pActionTextBold->setFont(bold);

	const QIcon italicIcon = QIcon(":/images/textitalic.png");
	m_pActionTextItalic = tb->addAction(italicIcon, QString::fromStdWString(L"斜体"), this, SLOT(OntextItalic()));
	QFont italic;
	italic.setItalic(true);
	m_pActionTextItalic->setFont(italic);

	const QIcon underlineIcon = QIcon(":/images/textunder.png");
	m_pActionTextUnderline = tb->addAction(underlineIcon, QString::fromStdWString(L"下划线"), this, SLOT(OntextUnderLine()));
	QFont underline;
	underline.setUnderline(true);
	m_pActionTextUnderline->setFont(underline);

	m_ptextColorbutton = new CToolButtonColorMenu(0, Qt::blue, tb);
	m_ptextColorbutton->setCheckable(false);
	connect(m_ptextColorbutton, SIGNAL(ColorSelected(QColor)), this, SLOT(OntextColor(QColor)));
	tb->addWidget(m_ptextColorbutton);
}

void CMainFrame::setupAlignActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"对齐方式"));

	QIcon icon = QIcon(":/images/aligntop.png");
	m_pActionAlignTop = tb->addAction(icon, QString::fromStdWString(L"上对齐"), this, SLOT(aligntop()));

	icon = QIcon(":/images/alignbottom.png");
	m_pActionAlignBottom = tb->addAction(icon, QString::fromStdWString(L"下对齐"), this, SLOT(alignbottom()));

	icon = QIcon(":/images/alignleft.png");
	m_pActionAlignLeft = tb->addAction(icon, QString::fromStdWString(L"左对齐"), this, SLOT(alignleft()));

	icon = QIcon(":/images/alignright.png");
	m_pActionAlignRight = tb->addAction(icon, QString::fromStdWString(L"右对齐"), this, SLOT(alignright()));

	icon = QIcon(":/images/alignmidver.png");
	m_pActionAlignVerMid = tb->addAction(icon, QString::fromStdWString(L"垂直对齐"), this, SLOT(alignverline()));

	icon = QIcon(":/images/alignmidhor.png");
	m_pActionAlignHorMid = tb->addAction(icon, QString::fromStdWString(L"水平对齐"), this, SLOT(alignhorline()));


	icon = QIcon(":/images/alignsamespacehor.png");
	m_pActionAlignHorSame = tb->addAction(icon, QString::fromStdWString(L"水平间距一致"), this, SLOT(alignhorspace()));

	icon = QIcon(":/images/alignsamespacever.png");
	m_pActionAlignVerSame = tb->addAction(icon, QString::fromStdWString(L"垂直间距一致"), this, SLOT(alignverspace()));

	icon = QIcon(":/images/alignsamewidth.png");
	m_pActionAlignWidthSame = tb->addAction(icon, QString::fromStdWString(L"一样宽"), this, SLOT(alignsamewidth()));

	icon = QIcon(":/images/alignsameheight.png");
	m_pActionAlignHeightSame = tb->addAction(icon, QString::fromStdWString(L"一样高"), this, SLOT(alignsameheight()));

	icon = QIcon(":/images/alignsamesize.png");
	m_pActionAlignSizeSame = tb->addAction(icon, QString::fromStdWString(L"大小一致"), this, SLOT(alignsamesize()));
}

void CMainFrame::setupColorActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"颜色"));

	m_textlinewidthbutton = new CToolButtonComboxMenu(0, 1, tb);
	m_textlinewidthbutton->setFixedSize(100, 32);
	m_textlinewidthbutton->setIconSize(QSize(100, 32));
	m_textlinewidthbutton->setAutoRaise(true);
	m_textlinewidthbutton->setToolButtonStyle(Qt::ToolButtonIconOnly);

	connect(m_textlinewidthbutton, SIGNAL(OnSelected(int)), this, SLOT(OnStrokeWidth(int)));
	tb->addWidget(m_textlinewidthbutton);
	tb->setIconSize(QSize(160, 32));

	m_textlinetypebutton = new CToolButtonComboxMenu(1, 0, tb);
	m_textlinetypebutton->setFixedSize(100, 32);
	m_textlinetypebutton->setIconSize(QSize(100, 32));
	m_textlinetypebutton->setAutoRaise(true);
	m_textlinetypebutton->setToolButtonStyle(Qt::ToolButtonIconOnly);
	connect(m_textlinetypebutton, SIGNAL(OnSelected(int)), this, SLOT(OnStrokeDashType(int)));
	tb->addWidget(m_textlinetypebutton);

	m_lineColorbutton = new CToolButtonColorMenu(1, Qt::black, tb);
	m_lineColorbutton->setFixedWidth(40);
	m_lineColorbutton->setCheckable(false);
	connect(m_lineColorbutton, SIGNAL(ColorSelected(QColor)), this, SLOT(OnStrokeColor(QColor)));
	tb->addWidget(m_lineColorbutton);

	m_fillColorbutton = new CToolButtonColorMenu(2, Qt::blue, tb);
	m_fillColorbutton->setFixedWidth(40);
	m_fillColorbutton->setCheckable(false);
	connect(m_fillColorbutton, SIGNAL(ColorSelected(QColor)), this, SLOT(OnFillColor(QColor)));
	connect(m_fillColorbutton, SIGNAL(colorSelectedRadient(const std::wstring&)), this, SLOT(OnFillUrl(const std::wstring&)));
	connect(m_fillColorbutton, SIGNAL(colorSelectedPattern(const std::wstring&)), this, SLOT(OnFillUrl(const std::wstring&)));
	tb->addWidget(m_fillColorbutton);
}

void CMainFrame::setupCanvasActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"画布"));
	QIcon newIcon = QIcon(":/images/showrule.png");
	m_pActionRule = tb->addAction(newIcon, QString::fromStdWString(L"显示标尺"), this, SLOT(OnShowRule()));
	newIcon = QIcon(":/images/showgrid.png");
	m_pActionGrid = tb->addAction(newIcon, QString::fromStdWString(L"显示网格"), this, SLOT(OnShowGrid()));
}

void CMainFrame::setupLockActions()
{
	QToolBar *tb = addToolBar(QString::fromStdWString(L"锁住图形"));
	QIcon newIcon = QIcon(":/images/lock.png");
	m_pActionLock = tb->addAction(newIcon, QString::fromStdWString(L"锁住"), this, SLOT(OnLock()));
	newIcon = QIcon(":/images/unlock.png");
	m_pActionUnLock = tb->addAction(newIcon, QString::fromStdWString(L"解除锁住"), this, SLOT(OnUnlock()));
}

void CMainFrame::ShowMousePosition(QPoint pt)
{
	if (m_statusLabelmousept)
	{
		QString str = "(" + QString::number(pt.x()) + "," + QString::number(pt.y()) + ")";
		m_statusLabelmousept->setText(str);
	}
}

void CMainFrame::setupStatusBar()
{
	statusBar()->setSizeGripEnabled(false);
	m_statusLabelmousept = new QLabel("(0,0)");
	m_statusLabelmousept->setAlignment(Qt::AlignLeft);
	m_statusLabelmousept->setMinimumSize(200, 15);
	statusBar()->addWidget(m_statusLabelmousept);
}

void CMainFrame::OnShowRule()
{
	if (m_pcurrentView)
	{
		m_bShowCanvasRule = !m_bShowCanvasRule;
		std::wstring strparam = L"false";
		if (m_bShowCanvasRule)
		{
			strparam = L"true";
		}
		m_pcurrentView->ShowRule();
	}
}

void CMainFrame::OnShowGrid()
{
	if (m_pcurrentView)
	{
		m_bShowCanvasGrid = !m_bShowCanvasGrid;
		std::wstring strparam = L"false";
		if (m_bShowCanvasGrid)
		{
			strparam = L"true";
		}
		m_pcurrentView->ShowGrid();
	}
}

void CMainFrame::OnLock()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->Lock();
	}
	ResetToolBarEnable();
}

void CMainFrame::OnUnlock()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->UnLock();
	}
	ResetToolBarEnable();
}

void CMainFrame::perpertyview()
{
	if (m_pPropertyDock->isVisible())
	{
		m_pPropertyDock->hide();
		m_ActionPropertyView->setChecked(false);
	}
	else
	{
		m_pPropertyDock->show();
		m_ActionPropertyView->setChecked(true);
	}
}

void CMainFrame::OntextFamilyChange(const QString &f)
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"font-family");
		vcAttrValue.push_back(f.toStdWString());
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OntextFontSizeChange(const QString &f)
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"font-size");
		vcAttrValue.push_back(f.toStdWString() + L"pt");
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OntextBold()
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"font-weight");
		vcAttrValue.push_back(L"Bold");
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OntextItalic()
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"font-style");
		vcAttrValue.push_back(L"Italic");
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OntextUnderLine()
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"text-decoration");
		vcAttrValue.push_back(L"Underline");
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OntextColor(QColor clr)
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"fill");
		vcAttrValue.push_back(global.QColorToStr(clr));
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::setupHelp()
{
	QMenu *menu = menuBar()->addMenu(QString::fromStdWString(L"帮助(&H)"));
	menu->addAction(QString::fromStdWString(L"关于(&A)"), this, SLOT(OnAbout()));
}

void CMainFrame::OnAbout()
{
	CDlgAbout dlg;
	dlg.exec();
}

void CMainFrame::OnMenuViewToShow()
{
	m_ActionPropertyView->setChecked(false);
	if (m_pPropertyDock->isVisible())
	{
		m_ActionPropertyView->setChecked(true);
	}
}


void CMainFrame::fileNew()
{
	CSVGView *psvgView = new CSVGView(this);

	QString strNextText = QString::fromStdWString(L"新建文件");
	QString strText = m_pTabWidget->tabText(m_pTabWidget->count() - 2);
	if (strText.indexOf(QString::fromStdWString(L"新建文件")) >= 0)
	{
		QString strNumber = strText;
		strNumber = strNumber.replace(QString::fromStdWString(L"新建文件"), "");
		int nNextNum = 1 + strNumber.toInt();
		strNextText = QString::fromStdWString(L"新建文件");
		strNextText += QString::number(nNextNum);
	}
	int nIndex = m_pTabWidget->insertTab(m_pTabWidget->count() - 1, psvgView, strNextText);
	m_pTabWidget->setCurrentIndex(nIndex);
	m_pTabWidget->setTabToolTip(nIndex, strNextText);
	m_pcurrentView = psvgView;
	m_pcurrentView->ConnectPanProperty();
	m_RightProperty->ClearProperty();
	m_pcurrentView->ResetProperty();
	ResetToolBarEnable();
}

void CMainFrame::OpenFile(const QString &strPath)
{
	if (strPath.lastIndexOf(".svg") > 0)
	{
		for (int i = 0; i < m_pTabWidget->count(); ++i)
		{
			QWidget *tabwidget = m_pTabWidget->widget(i);
			QString strclassName = tabwidget->metaObject()->className();
			if (tabwidget && strclassName == "CSVGView")
			{
				CSVGView *psvgView = (CSVGView*)m_pTabWidget->widget(i);
				if (strPath == psvgView->m_EditPath.strFullPath)
				{
					m_pTabWidget->setCurrentWidget(psvgView);
					return;
				}
			}
		}
		QFileInfo fileinfo = QFileInfo(strPath);
		CSVGView *pSVGView = new CSVGView(this);
		int nIndex = m_pTabWidget->insertTab(m_pTabWidget->count() - 1, pSVGView, fileinfo.fileName());
		pSVGView->m_EditPath.strFullPath = strPath;
		m_pTabWidget->setCurrentIndex(nIndex);
		m_pTabWidget->setTabToolTip(nIndex, strPath);
		m_pcurrentView = pSVGView;
		m_pcurrentView->open(strPath.toStdWString());
		QString lstDir = fileinfo.absolutePath();
	}
}

void CMainFrame::fileOpen()
{
	QFileDialog fileDialog(this, QString::fromStdWString(L"打开文件..."), ".", "svg(*.svg);;All(*.*)");
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::ExistingFiles);
	if (fileDialog.exec() != QDialog::Accepted)
		return;
	for (int i = 0; i < fileDialog.selectedFiles().size(); ++i)
	{
		const QString fn = fileDialog.selectedFiles().at(i);
		OpenFile(fn);
	}
}

void CMainFrame::ResetViewFileMark(CSVGView *pSVGView)
{
	for (int i = 0; i < m_pTabWidget->count(); ++i)
	{
		QWidget *tabwidget = m_pTabWidget->widget(i);
		QString strclassName = tabwidget->metaObject()->className();
		if (tabwidget && strclassName == "CSVGView")
		{
			CSVGView *psvgView = (CSVGView*)m_pTabWidget->widget(i);
			if (psvgView == pSVGView && psvgView->m_EditPath.strFullPath.length() > 0)
			{
				QFileInfo fileinfo(psvgView->m_EditPath.strFullPath);
				QString strName = fileinfo.fileName();
				if (psvgView->m_SVGDocument.IsModify())
				{
					strName.replace(".svg", ".svg*", Qt::CaseInsensitive);
				}
				m_pTabWidget->tabBar()->setTabText(i, strName);
				break;
			}
		}
	}
}

void CMainFrame::fileSave()
{
	if (m_pcurrentView)
	{
		QString strName;
		m_pcurrentView->Save(strName);

		if (m_pcurrentView->m_EditPath.strFullPath.length() > 0)
		{
			for (int i = 0; i < m_pTabWidget->count(); ++i)
			{
				if (m_pTabWidget->widget(i) == m_pcurrentView)
				{
					m_pTabWidget->setTabToolTip(i, m_pcurrentView->m_EditPath.strFullPath);
					if (strName.length() > 0)
					{
						m_pTabWidget->setTabText(i, strName);
					}
				}
			}
		}
		gCurrentSVGPath = m_pcurrentView->m_EditPath.strFullPath;
	}
	ResetToolBarEnable();
}

void CMainFrame::fileSaveAs()
{
	if (m_pcurrentView)
	{
		QString strName;
		m_pcurrentView->SaveAs(strName);
		if (m_pcurrentView->m_EditPath.strFullPath.length() > 0)
		{
			for (int i = 0; i < m_pTabWidget->count(); ++i)
			{
				if (m_pTabWidget->widget(i) == m_pcurrentView)
				{
					m_pTabWidget->setTabToolTip(i, m_pcurrentView->m_EditPath.strFullPath);
					if (strName.length() > 0)
					{
						m_pTabWidget->setTabText(i, strName);
					}
				}
			}
		}
		gCurrentSVGPath = m_pcurrentView->m_EditPath.strFullPath;
	}
	ResetToolBarEnable();
}

void CMainFrame::fileSaveASPng()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SaveAsPng();
	}
}

void CMainFrame::fileSaveASJpg()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SaveAsJpg();
	}
}

void CMainFrame::copy()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnCopy();
	}
}

void CMainFrame::paste()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnPaste();
	}
}

void CMainFrame::undo()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnUndo();
	}
}

void CMainFrame::redo()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnRedo();
	}
}

void CMainFrame::drawline()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_LINE);
	}
}

void CMainFrame::drawrect()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_RECT);
	}
}
void CMainFrame::drawcircle()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_CIRCLE);
	}
}
void CMainFrame::drawellipse()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_ELLIPSE);
	}
}
void CMainFrame::drawpolyline()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_POLYLINE);
	}
}
void CMainFrame::drawpolygon()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_POLYGON);
	}
}

void CMainFrame::drawpath()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_PATH);
	}
}
void CMainFrame::drawarc()
{
}
void CMainFrame::drawtext()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_TEXT);
	}
}

void CMainFrame::drawtextAAA()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_TEXTAAA);
	}
}

void CMainFrame::drawtextv()
{
}
void CMainFrame::drawimage()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnDrawImage();
	}
}

void CMainFrame::drawcontinue()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->OnDrawContinue();
	}
}

void CMainFrame::drawcustom(const QString &strText)
{
	if (m_pcurrentView)
	{
		if (strText.length() > 0)
		{
			QClipboard *clipboard = QApplication::clipboard();
			clipboard->setText(strText);
		}
		m_pcurrentView->SetDrawTool(DRAWTOOL_CUSTOM);
	}
}

void CMainFrame::arrow()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_SELECT);
	}
}

void CMainFrame::arrowmatrix()
{

}

void CMainFrame::zoomrestore()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->ZoomRestore();
	}
}
void CMainFrame::zoomout()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_ZOOMOUT);
	}
}
void CMainFrame::zoomin()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_ZOOMIN);
	}
}
void CMainFrame::zoommove()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SetDrawTool(DRAWTOOL_MOVE);
	}
}

void CMainFrame::rotate()
{
	if (m_pcurrentView)
	{
		CMenuRotate menu(m_pcurrentView);
		menu.exec(QCursor::pos());
	}
}

void CMainFrame::createDockWindows()
{
	m_pPropertyDock = new QDockWidget(QString::fromStdWString(L"属性视图"), this);
	m_pPropertyDock->resize(500, 600);
	m_pPropertyDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_RightProperty = new CPanePropertyTableView(m_pPropertyDock);
	m_pPropertyDock->setWidget(m_RightProperty);
	addDockWidget(Qt::RightDockWidgetArea, m_pPropertyDock);
}

void CMainFrame::OnFillColor(QColor clr)
{
	if (m_pcurrentView)
	{
		if (clr.alpha() == 0)
		{
			std::vector<std::wstring>vcAttrName;
			std::vector<std::wstring>vcAttrValue;
			vcAttrName.push_back(L"fill-opacity");
			vcAttrValue.push_back(L"0");
			m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
		}
		else
		{
			std::vector<std::wstring>vcAttrName;
			std::vector<std::wstring>vcAttrValue;
			vcAttrName.push_back(L"fill-opacity");
			vcAttrName.push_back(L"fill");
			vcAttrValue.push_back(L"1");
			vcAttrValue.push_back(global.QColorToStr(clr));
			m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
		}
	}
}

void CMainFrame::OnFillUrl(const std::wstring &str)
{
	if (m_pcurrentView)
	{
		m_pcurrentView->FillUrl(str);
	}
}

void CMainFrame::OpenFolder(QAction * action)
{
	QString fullfilename = action->data().toString();
#ifdef Q_OS_WIN
	QProcess process;
	fullfilename = fullfilename.replace("\\\\", "\\");
	fullfilename = fullfilename.replace("//", "\\");
	fullfilename = fullfilename.replace("/", "\\");
	QString cmd = QString("explorer.exe /select,\"%1\"").arg(fullfilename);
	process.startDetached(cmd);
#else
	QFileInfo fileinfo(fullfilename);
	QString filepath = fileinfo.absolutePath();//文件夹路径
	QDesktopServices::openUrl("file:///" + fileinfo.dir().absolutePath());
#endif
}

void CMainFrame::OpenByDefault(QAction * action)
{
	QString fullfilename = action->data().toString();
	QDesktopServices::openUrl("file:///" + fullfilename);
}

void CMainFrame::ReLoadFile(QAction * action)
{
	QVariant var = action->data();
	CSVGView *pViewer = (CSVGView *)var.value<void *>();
	pViewer->ReloadFile();
}

void CMainFrame::OnCloseTab(QAction * action)
{
	if (m_pTabWidget)
	{
		int nIndex = action->data().toInt();
		tabClose(nIndex);
	}
}

void CMainFrame::OnCloseAllTab(QAction * action)
{
	Q_UNUSED(action);
	if (m_pTabWidget)
	{
		for (int i = m_pTabWidget->count() - 1; i >= 0; --i)
		{
			tabClose(i);
		}
	}
}

void CMainFrame::OnCloseRightTab(QAction * action)
{
	if (m_pTabWidget)
	{
		int nIndex = action->data().toInt();
		for (int i = m_pTabWidget->count() - 1; i >= 0; --i)
		{
			if (i > nIndex)
			{
				tabClose(i);
			}
		}
	}
}

void CMainFrame::OncustomContextMenuRequested(const QPoint& pt)
{
	QTabBar* pTabBar = m_pTabWidget->tabBar();
	for (int i = 0; i < pTabBar->count(); ++i)
	{
		QRect rc = pTabBar->tabRect(i);
		if (rc.contains(pt))
		{
			QWidget *pWidget = m_pTabWidget->widget(i);
			if (pWidget)
			{
				QString strClassName = pWidget->metaObject()->className();
				if (strClassName == "CSVGView")
				{
					CSVGView *pSVGView = (CSVGView*)pWidget;
					QPoint pos = pTabBar->mapToGlobal(pt);
					QMenu menu;
					//////////////////////////////////////////////////////////////////////////
					QActionGroup *closeTab = new QActionGroup(this);
					connect(closeTab, &QActionGroup::triggered, this, &CMainFrame::OnCloseTab);
					QAction *act = menu.addAction(QString::fromStdWString(L"关闭文档"));
					act->setData(i);
					closeTab->addAction(act);
					QActionGroup *closeAllTab = new QActionGroup(this);
					connect(closeAllTab, &QActionGroup::triggered, this, &CMainFrame::OnCloseAllTab);
					act = menu.addAction(QString::fromStdWString(L"关闭所有文档"));
					act->setData(i);
					closeAllTab->addAction(act);
					QActionGroup *closeElseAllTab = new QActionGroup(this);
					connect(closeElseAllTab, &QActionGroup::triggered, this, &CMainFrame::OnCloseRightTab);
					act = menu.addAction(QString::fromStdWString(L"关闭右边文档"));
					act->setData(i);
					closeElseAllTab->addAction(act);
					//////////////////////////////////////////////////////////////////////////
					if (pSVGView->m_EditPath.strFullPath.length() > 0)
					{
						menu.addSeparator();
						QActionGroup *findFolder = new QActionGroup(this);
						connect(findFolder, &QActionGroup::triggered, this, &CMainFrame::OpenFolder);
						act = menu.addAction(QString::fromStdWString(L"打开目录"));
						act->setData(pSVGView->m_EditPath.strFullPath);
						findFolder->addAction(act);
						//////////////////////////////////////////////////////////////////////////
						QActionGroup *ActionOpenBydefault = new QActionGroup(this);
						connect(ActionOpenBydefault, &QActionGroup::triggered, this, &CMainFrame::OpenByDefault);
						act = menu.addAction(QString::fromStdWString(L"用默认程序打开"));
						act->setData(pSVGView->m_EditPath.strFullPath);
						ActionOpenBydefault->addAction(act);

						//////////////////////////////////////////////////////////////////////////
						QActionGroup *reload = new QActionGroup(this);
						connect(reload, &QActionGroup::triggered, this, &CMainFrame::ReLoadFile);
						QAction *actreload = menu.addAction(QString::fromStdWString(L"重新加载"));
						QVariant var = QVariant::fromValue((void *)pSVGView);
						actreload->setData(var);
						reload->addAction(actreload);
					}
					menu.exec(pos);
				}
			}
			break;
		}
	}
}

void CMainFrame::OnStrokeColor(QColor clr)
{
	if (m_pcurrentView)
	{
		if (clr.alpha() == 0)
		{
			std::vector<std::wstring>vcAttrName;
			std::vector<std::wstring>vcAttrValue;
			vcAttrName.push_back(L"stroke-opacity");
			vcAttrValue.push_back(L"0");
			m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
		}
		else
		{
			std::vector<std::wstring>vcAttrName;
			std::vector<std::wstring>vcAttrValue;
			vcAttrName.push_back(L"stroke-opacity");
			vcAttrValue.push_back(L"1");
			vcAttrName.push_back(L"stroke");
			vcAttrValue.push_back(global.QColorToStr(clr));
			m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
		}
	}
}

void CMainFrame::OnStrokeWidth(int nIndex)
{
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"stroke-width");
		vcAttrValue.push_back(global.itostring(nIndex));
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OnStrokeDashType(int nIndex)
{
	std::wstring dasharray = L"";
	switch (nIndex)
	{
	case 0:
	{
		dasharray = L"";
	}
	break;
	case 1:
		dasharray = L"18 8 10 10";
		break;
	case 2:
		dasharray = L"4 4";
		break;
	case 3:
		dasharray = L"12 5 5 5 8 8";
		break;
	case 4:
		dasharray = L"12 5 5 5 5 5 8 8";
		break;
	case 5:
		dasharray = L"18 28";
		break;
	case 6:
		dasharray = L"18 10 10 10";
		break;
	case 7:
		dasharray = L"18 6 14 14";
		break;
	case 8:
		dasharray = L"12 12 8 8";
		break;
	case 9:
		dasharray = L"14 8 14 8";
		break;
	case 10:
		dasharray = L"10 12";
		break;
	case 11:
		dasharray = L"10 8";
		break;
	case 12:
		dasharray = L"4 2";
		break;
	case 13:
		dasharray = L"2 3";
		break;
	case 14:
		dasharray = L"1 2";
		break;
	default:
		dasharray = L"";
	}
	if (m_pcurrentView)
	{
		std::vector<std::wstring>vcAttrName;
		std::vector<std::wstring>vcAttrValue;
		vcAttrName.push_back(L"stroke-dasharray");
		vcAttrValue.push_back(dasharray);
		m_pcurrentView->SetAttributes(vcAttrName, vcAttrValue, true);
	}
}

void CMainFrame::OnGroup()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->Group();
	}
}

void CMainFrame::OnUnGroup()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->UnGroup();
	}
}

void CMainFrame::layertop()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->LayerTop();
	}
}

void CMainFrame::layerbottom()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->LayerBottom();
	}
}

void CMainFrame::layerup()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->LayerUp();
	}
}

void CMainFrame::layerdown()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->LayerDown();
	}
}

void CMainFrame::mirrorright()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->MirrorRight();
	}
}

void CMainFrame::mirrorbottom()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->MirrorBottom();
	}
}

void CMainFrame::turnright()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->TurnRight();
	}
}

void CMainFrame::turnleft()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->TurnLeft();
	}
}

void CMainFrame::alignleft()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->LeftAlign();
	}
}

void CMainFrame::aligntop()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->TopAlign();
	}
}

void CMainFrame::alignright()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->RightAlign();
	}
}

void CMainFrame::alignverline()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->VerLineAlign();
	}
}

void CMainFrame::alignhorline()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->HorLineAlign();
	}
}

void CMainFrame::alignbottom()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->BottomAlign();
	}
}

void CMainFrame::alignhorspace()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->HorSpaceAlign();
	}
}

void CMainFrame::alignverspace()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->VerSpaceAlign();
	}
}

void CMainFrame::alignsamewidth()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SameWidthAlign();
	}
}

void CMainFrame::alignsameheight()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SameHeightAlign();
	}
}

void CMainFrame::alignsamesize()
{
	if (m_pcurrentView)
	{
		m_pcurrentView->SameSizeAlign();
	}
}

void CMainFrame::tabClose(int index)
{
	m_bLockTabSelChange = false;
	QWidget * pWidget = m_pTabWidget->widget(index);
	if (pWidget)
	{
		QString strClass = pWidget->metaObject()->className();
		if (strClass == "CNewFileView")
		{
			return;
		}
		int nTabCount = m_pTabWidget->count();
		if (nTabCount == index + 2)
		{//如果移除的是最后一个视图,则选中前面的;
			m_bLockTabSelChange = true;
		}
		if (ShowSaveMessage(pWidget))
		{
			if (m_pcurrentView)
			{
				m_pcurrentView->DisConnectPanProperty();
			}
			m_pcurrentView = NULL;
			m_pTabWidget->removeTab(index);
			if (m_bLockTabSelChange)
			{
				m_pTabWidget->setCurrentIndex(index - 1);
				QWidget *pWidget = m_pTabWidget->currentWidget();
				QString strClassName = pWidget->metaObject()->className();
				if (strClassName == "CSVGView")
				{
					m_pcurrentView = (CSVGView *)pWidget;
				}
				m_bLockTabSelChange = false;
			}
			if (nTabCount == 2)
			{
				fileNew();
			}
		}
	}
	ResetToolBarEnable();
}

void CMainFrame::ResetToolBarEnable()
{
	m_pactionSave->setEnabled(false);
	m_pActionundo->setEnabled(false);
	m_pActionredo->setEnabled(false);
	m_pActioncopy->setEnabled(false);
	m_pActionpaste->setEnabled(false);
	m_pActionDrawLine->setEnabled(false);
	m_pActionDrawRect->setEnabled(false);
	m_pActionDrawCircle->setEnabled(false);
	m_pActionDrawEllipse->setEnabled(false);
	m_pActionDrawPolyline->setEnabled(false);
	m_pActionDrawPolygon->setEnabled(false);
	m_pActionDrawpath->setEnabled(false);
	m_pActionDrawtext->setEnabled(false);
	m_pActionDrawImage->setEnabled(false);
	m_pActionDrawContinue->setEnabled(false);
	m_pActionArrow->setEnabled(false);
	m_pActionRestore->setEnabled(false);
	m_pActionZoomin->setEnabled(false);
	m_pActionZoomout->setEnabled(false);
	m_pActionZoommove->setEnabled(false);
	m_pActionRotate->setEnabled(false);
	m_pActionGroup->setEnabled(false);
	m_pActionUnGroup->setEnabled(false);
	m_pActionMoveTop->setEnabled(false);
	m_pActionMoveBottom->setEnabled(false);
	m_pActionMoveup->setEnabled(false);
	m_pActionMoveDown->setEnabled(false);
	m_pActionTurnRight->setEnabled(false);
	m_pActionTurnLeft->setEnabled(false);
	m_pActionMoveHor->setEnabled(false);
	m_pActionMoveVer->setEnabled(false);
	m_pActionAlignTop->setEnabled(false);
	m_pActionAlignBottom->setEnabled(false);
	m_pActionAlignLeft->setEnabled(false);
	m_pActionAlignRight->setEnabled(false);
	m_pActionAlignHorMid->setEnabled(false);
	m_pActionAlignVerMid->setEnabled(false);
	m_pActionAlignHorMid->setEnabled(false);
	m_pActionAlignVerMid->setEnabled(false);
	m_pActionAlignHorSame->setEnabled(false);
	m_pActionAlignVerSame->setEnabled(false);
	m_pActionAlignWidthSame->setEnabled(false);
	m_pActionAlignHeightSame->setEnabled(false);
	m_pActionAlignSizeSame->setEnabled(false);
	m_pActionTextBold->setEnabled(false);
	m_pActionTextItalic->setEnabled(false);
	m_pActionTextUnderline->setEnabled(false);
	m_comboFont->setEnabled(false);
	m_comboSize->setEnabled(false);

	m_pActionRule->setEnabled(false);
	m_pActionGrid->setEnabled(false);
	m_pActionLock->setEnabled(false);
	m_pActionUnLock->setEnabled(false);
	m_ptextColorbutton->setEnabled(false);
	m_textlinewidthbutton->setEnabled(false);
	m_textlinetypebutton->setEnabled(false);
	m_lineColorbutton->setEnabled(false);
	m_fillColorbutton->setEnabled(false);
	m_pActionExportPng->setEnabled(false);
	m_pActionExportJpg->setEnabled(false);

	if (m_pcurrentView)
	{
		m_pactionSave->setEnabled(true);
		m_pActionundo->setEnabled(m_pcurrentView->CanUndo());
		m_pActionredo->setEnabled(m_pcurrentView->CanRedo());
		m_pActioncopy->setEnabled(m_pcurrentView->CanCopy());
		m_pActionpaste->setEnabled(m_pcurrentView->CanPaste());
		m_pActionDrawLine->setEnabled(true);
		m_pActionDrawRect->setEnabled(true);
		m_pActionDrawCircle->setEnabled(true);
		m_pActionDrawEllipse->setEnabled(true);
		m_pActionDrawPolyline->setEnabled(true);
		m_pActionDrawPolygon->setEnabled(true);
		m_pActionDrawpath->setEnabled(true);
		m_pActionDrawtext->setEnabled(true);
		m_pActionDrawImage->setEnabled(true);
		m_pActionDrawContinue->setEnabled(m_pcurrentView->CanContinueDraw());
		m_pActionArrow->setEnabled(true);
		m_pActionRestore->setEnabled(true);
		m_pActionZoomin->setEnabled(true);
		m_pActionZoomout->setEnabled(true);
		m_pActionZoommove->setEnabled(true);
		m_pActionRotate->setEnabled(m_pcurrentView->CanRotate());
		m_pActionGroup->setEnabled(m_pcurrentView->CanGroup());
		m_pActionUnGroup->setEnabled(m_pcurrentView->CanUnGroup());
		m_pActionMoveTop->setEnabled(true);
		m_pActionMoveBottom->setEnabled(true);
		m_pActionMoveup->setEnabled(true);
		m_pActionMoveDown->setEnabled(true);
		m_pActionTurnRight->setEnabled(m_pcurrentView->HasSelectShape());
		m_pActionTurnLeft->setEnabled(m_pcurrentView->HasSelectShape());
		m_pActionMoveHor->setEnabled(m_pcurrentView->HasSelectShape());
		m_pActionMoveVer->setEnabled(m_pcurrentView->HasSelectShape());
		m_pActionAlignTop->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignBottom->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignLeft->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignRight->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignHorMid->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignVerMid->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignHorSame->setEnabled(m_pcurrentView->hasSelectMoveTwoShape());
		m_pActionAlignVerSame->setEnabled(m_pcurrentView->hasSelectMoveTwoShape());
		m_pActionAlignWidthSame->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignHeightSame->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionAlignSizeSame->setEnabled(m_pcurrentView->HasSelectMoreOneShape());
		m_pActionLock->setEnabled(m_pcurrentView->HasSelectShape());
		m_pActionUnLock->setEnabled(m_pcurrentView->HasLockShape());

		m_pActionRule->setEnabled(true);
		m_pActionGrid->setEnabled(true);

		m_pActionTextBold->setEnabled(true);
		m_pActionTextItalic->setEnabled(true);
		m_pActionTextUnderline->setEnabled(true);
		m_comboFont->setEnabled(true);
		m_comboSize->setEnabled(true);

		m_ptextColorbutton->setEnabled(true);
		m_textlinewidthbutton->setEnabled(true);
		m_textlinetypebutton->setEnabled(true);
		m_lineColorbutton->setEnabled(true);
		m_fillColorbutton->setEnabled(true);
		m_pActionExportPng->setEnabled(true);
		m_pActionExportJpg->setEnabled(true);

		ResetViewFileMark(m_pcurrentView);
	}
}

void CMainFrame::tabcurrentChanged(int index)
{
	if (m_bLockTabSelChange)
		return;
	if (m_pcurrentView)
	{
		m_pcurrentView->DisConnectPanProperty();
		m_pcurrentView = NULL;
	}
	int nTabCount = m_pTabWidget->count();
	for (int i = 0; i < nTabCount; ++i)
	{
		if (i == index)
		{
			QWidget * pWidget = m_pTabWidget->widget(i);
			QString classname = pWidget->metaObject()->className();
			if (classname == "CNewFileView")
			{
				fileNew();
			}
			else if (classname == "CSVGView")
			{
				m_pcurrentView = (CSVGView*)pWidget;
				m_pcurrentView->ConnectPanProperty();
				gCurrentSVGPath = m_pcurrentView->m_EditPath.strFullPath;
				m_pPropertyDock->show();
				m_RightProperty->ClearProperty();
				m_pcurrentView->ResetProperty();
			}
		}
	}
	ResetToolBarEnable();
}

void CMainFrame::SetCurrentFontName(QString strFontName)
{
	m_comboFont->setCurrentText(strFontName);
}

void CMainFrame::SetCurrentFontSize(QString strFontSize)
{
	m_comboSize->setCurrentText(strFontSize);
}

void CMainFrame::timerEvent(QTimerEvent *event)
{
	QMainWindow::timerEvent(event);
}

void CMainFrame::closeEvent(QCloseEvent *e)
{
	if (maybeSave())
	{
		e->accept();
	}
	else
	{
		e->ignore();
	}
}

bool CMainFrame::maybeSave()
{
	int nTabCount = m_pTabWidget->count();
	for (int i = 0; i < nTabCount; ++i)
	{
		QWidget *pWidget = m_pTabWidget->widget(i);
		if (pWidget)
		{
			if (!ShowSaveMessage(pWidget))
			{
				return false;
			}
		}
	}
	return true;
}

bool CMainFrame::ShowSaveMessage(QWidget *pWidget)
{
	if (pWidget)
	{
		QString strClassName = pWidget->metaObject()->className();
		if (strClassName == "CSVGView")
		{
			CSVGView *pView = (CSVGView*)pWidget;
			if (!pView->IsModify())
				return true;
			QString strLabel = pView->m_EditPath.strFullPath;
			if (strLabel.length() == 0)
			{
				strLabel = QString::fromStdWString(L"新建文件");
			}
			QMessageBox box(QMessageBox::Warning, QString::fromStdWString(L"警告"), strLabel + QString::fromStdWString(L"文件已经修改,是否保存?"));
			box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			box.setButtonText(QMessageBox::Save, QString::fromStdWString(L"保存"));
			box.setButtonText(QMessageBox::Discard, QString::fromStdWString(L"不保存"));
			box.setButtonText(QMessageBox::Cancel, QString::fromStdWString(L"取消"));
			int ret = box.exec();
			if (ret == QMessageBox::Save)
			{
				QString strName;
				if (!pView->Save(strName))
				{
					return false;
				}
			}
			else if (ret == QMessageBox::Cancel)
			{
				return false;
			}
		}
	}
	return true;
}

void CMainFrame::ForceCloseFile(const QString &strPath)
{
	for (int i = 0; i < m_pTabWidget->count(); ++i)
	{
		QWidget *tabwidget = m_pTabWidget->widget(i);
		QString strclassName = tabwidget->metaObject()->className();
		if (tabwidget && strclassName == "CSVGView")
		{
			CSVGView *psvgView = (CSVGView*)m_pTabWidget->widget(i);
			if (psvgView->m_EditPath.strFullPath == strPath)
			{
				psvgView->m_SVGDocument.SetModify(false);
				tabClose(i);
				break;
			}
		}
	}
}

void CMainFrame::ReNameFile(const QString &strOldPath, const QString &strNewPath)
{
	for (int i = 0; i < m_pTabWidget->count(); ++i)
	{
		QWidget *tabwidget = m_pTabWidget->widget(i);
		QString strclassName = tabwidget->metaObject()->className();
		if (tabwidget && strclassName == "CSVGView")
		{
			CSVGView *psvgView = (CSVGView*)m_pTabWidget->widget(i);
			if (psvgView->m_EditPath.strFullPath == strOldPath)
			{
				psvgView->m_EditPath.strFullPath = strNewPath;
				QFileInfo fileinfo(strNewPath);
				QString strName = fileinfo.fileName();
				m_pTabWidget->tabBar()->setTabText(i, strName);
				ResetViewFileMark(psvgView);
				break;
			}
		}
	}
}
