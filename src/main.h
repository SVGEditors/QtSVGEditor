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
#include <string>
#include <QStringList>

extern QString       gCurrentSVGPath;

bool SaveFileTextToUtf8(QString strFilePath, QString strContent);
bool SaveFileText(QString strFilePath, QByteArray strContent);
QString GetFileText(QString strFilePath);
QStringList GetAllFilePath(QString strFolder);
QStringList GetFilePathsInOneLever(QString strFolder);
QStringList GetFolderInOneLever(QString strFolder);
QStringList GetFilePathsInOneLevelWithType(QString strFolder, QString strType);//strType=".svg"
std::wstring StrReplace(std::wstring strContent, std::wstring strReplace, std::wstring strDest);
