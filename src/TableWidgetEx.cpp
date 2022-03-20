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
#include "TableWidgetEx.h"



CTableWidgetEx::CTableWidgetEx(QWidget *parent)
	:QTableWidget(parent)
{
	m_nrow = -1;
	m_ncol = -1;
	setMouseTracking(true);
}

CTableWidgetEx::~CTableWidgetEx()
{
}

void CTableWidgetEx::mouseMoveEvent(QMouseEvent *event)
{
	int  nRow = -1;
	int  nCol = -1;

	QPoint pt = event->pos();
	bool bNeedBreak = false;
	int nHeight = 0;
	for (int i = 0; i < rowCount(); ++i)
	{
		if (bNeedBreak)
			break;
		nHeight += rowHeight(i);
		int nWidth = 0;
		for (int j = 0; j < columnCount(); ++j)
		{
			nWidth += columnWidth(j);
			if (pt.x() <= nWidth && pt.y() <= nHeight)
			{
				nRow = i;
				nCol = j;
				bNeedBreak = true;
				break;
			}
		}
	}
	if (nRow != m_nrow || nCol != m_ncol)
	{
		m_nrow = nRow;
		m_ncol = nCol;
		emit mouseInChange(nRow, nCol);
	}

	return QTableWidget::mouseMoveEvent(event);
}
