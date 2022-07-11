
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
#include "main.h"
#include "MainFrame.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDomDocument>
#include <QTranslator>
#include <QDir>
#include <QScreen>

QString       gCurrentSVGPath;
QString      gLastFilePath;


bool SaveFileTextToUtf8(QString strFilePath, QString strContent)
{
	QString strTempPath = strFilePath + "temp";
	QByteArray  utf8Arr = strContent.toUtf8();
	QFile file(strTempPath);
	if (file.open(QFile::WriteOnly))
	{
		file.write(utf8Arr);
		file.close();
		QFile::setPermissions(strFilePath, QFileDevice::ReadOther | QFileDevice::WriteOther);
		QFile::remove(strFilePath);
		QFile::rename(strTempPath, strFilePath);
		return true;
	}
	return false;
}

QString GetFileText(QString strFilePath)
{
	QString strText;
	QFile file(strFilePath);
	if (file.open(QFile::ReadOnly))
	{
		strText = file.readAll();
		file.close();
	}
	return strText;
}

bool SaveFileText(QString strFilePath, QByteArray strContent)
{
	QString strTempPath = strFilePath + "temp";
	QFile file(strTempPath);
	if (file.open(QFile::WriteOnly))
	{
		file.write(strContent);
		file.close();
		QFile::setPermissions(strFilePath, QFileDevice::ReadOther | QFileDevice::WriteOther);
		QFile::remove(strFilePath);
		QFile::rename(strTempPath, strFilePath);
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QTranslator qtTranslator;
	if (qtTranslator.load(":/translate/zh_CN.qm"))
	{
		app.installTranslator(&qtTranslator);
	}


	QScreen *primaryScreen = app.primaryScreen();
	qreal  fdpix = primaryScreen->logicalDotsPerInch();
	float frate = fdpix / 96.0;
	if (frate < 1.1)
	{
		global.m_fscreenscale = 1.0;
	}
	else if (frate < 1.4)
	{
		global.m_fscreenscale = 1.25;
	}
	else if (frate < 1.6)
	{
		global.m_fscreenscale = 1.5;
	}
	else if (frate < 1.8)
	{
		global.m_fscreenscale = 1.75;
	}
	else
	{
		global.m_fscreenscale = 2.0;
	}
	global.m_fmulmm2pixel = 0.03937008f * 96;
	global.m_fmulpt2mm = 0.3527f;
	global.m_fpt2pixel = global.m_fmulpt2mm *global.m_fmulmm2pixel;
	global.m_fpixel2pt = 1.0f / global.m_fpt2pixel;

	CMainFrame mw;
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mw);
	mw.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
	mw.move((availableGeometry.width() - mw.width()) / 2,
		(availableGeometry.height() - mw.height()) / 2);
	mw.setWindowState(Qt::WindowMaximized);
	mw.show();
	return app.exec();
}

QStringList findFiles(const QString &startDir)
{
	QStringList filters;
	filters.push_back("*.*");
	QStringList names;
	QDir dir(startDir);
	foreach(QString file, dir.entryList(filters, QDir::Files))
		names += startDir + '/' + file;
	foreach(QString subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
		names += findFiles(startDir + '/' + subdir);
	return names;
}

QStringList GetAllFilePath(QString strFolder)
{
	QStringList retlst = findFiles(strFolder);
	return retlst;
}

bool sortbyNumber(QString str1, QString str2)
{
	int nNumber1 = 0;
	int nNumber2 = 0;
	QFileInfo fileinfo1(str1);
	QString name1 = fileinfo1.fileName();
	QFileInfo fileinfo2(str2);
	QString name2 = fileinfo2.fileName();
	for (int i = 0; i < name1.length(); ++i)
	{
		QChar c = name1[i];
		if (c >= '0' && c <= '9')
		{
			nNumber1 = nNumber1 * 10 + QString(c).toInt();
		}
		else
		{
			break;
		}
	}
	for (int i = 0; i < name2.length(); ++i)
	{
		QChar c = name2[i];
		if (c >= '0' && c <= '9')
		{
			nNumber2 = nNumber2 * 10 + QString(c).toInt();
		}
		else
		{
			break;
		}
	}
	if (nNumber1 < nNumber2)
		return true;

	return false;
}

QStringList GetFilePathsInOneLever(QString strFolder)
{
	QStringList filters;
	filters.push_back("*.*");
	QStringList names;
	QDir dir(strFolder);
	foreach(QString file, dir.entryList(filters, QDir::Files))
		names += strFolder + '/' + file;
	std::sort(names.begin(), names.end(), sortbyNumber);

	return names;
}

QStringList GetFolderInOneLever(QString strFolder)
{
	QStringList filters;
	filters.push_back("*.*");
	QStringList names;
	QDir dir(strFolder);
	foreach(QString file, dir.entryList(filters, QDir::AllDirs | QDir::NoDotAndDotDot))
		names += strFolder + '/' + file;
	std::sort(names.begin(), names.end(), sortbyNumber);

	return names;
}

QStringList GetFilePathsInOneLevelWithType(QString strFolder, QString strType)
{
	QStringList vcFiles = GetFilePathsInOneLever(strFolder);
	for (int i = vcFiles.size() - 1; i >= 0; --i)
	{
		if (vcFiles[i].indexOf(strType, 0, Qt::CaseInsensitive) <= 0)
		{
			vcFiles.removeAt(i);
		}
	}
	return vcFiles;
}

std::wstring StrReplace(std::wstring strContent, std::wstring strReplace, std::wstring strDest)
{
	while (true)
	{
		size_t pos = strContent.find(strReplace);
		if (pos != std::wstring::npos)
		{
			wchar_t pBuf[1] = { L'\0' };
			strContent.replace(pos, strReplace.length(), pBuf, 0);
			strContent.insert(pos, strDest);
		}
		else
		{
			break;
		}
	}
	return strContent;
}
