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
#include "SVGRectF.h"

CSVGRect::CSVGRect()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}
CSVGRect::CSVGRect(int  nx, int  ny, int  nwidth, int  nheight)
{
	x = nx;
	y = ny;
	width = nwidth;
	height = nheight;
}

CSVGLine CSVGRect::ToLine()
{
	return CSVGLine(x, y, x + width, y + height);
}

//////////////////////////////////////////////////////////////////////////
CSVGRectF::CSVGRectF()
	:x(0), y(0), width(0), height(0)
{
}

CSVGRectF::~CSVGRectF()
{

}
CSVGRectF::CSVGRectF(float fx, float fy, float fwidth, float fheight)
	:x(fx), y(fy), width(fwidth), height(fheight)
{
}
const CSVGRectF &CSVGRectF::operator =(const CSVGRectF&p)
{
	if (this != &p)
	{
		x = p.x;
		y = p.y;
		width = p.width;
		height = p.height;
	}
	return *this;
}

bool  CSVGRectF::operator==(const CSVGRectF& res) const
{
	if ((DBEQUAL(x, res.x))
		&& (DBEQUAL(y, res.y))
		&& (DBEQUAL(width, res.width))
		&& (DBEQUAL(height, res.height))
		)
	{
		return  true;
	}
	return false;
}

void CSVGRectF::Empty()
{
	x = y = width = height = 0;
}
bool CSVGRectF::IsEmpty()const
{
	const float nzero = (float)0.0000005;
	if (std::abs(width) < nzero || std::abs(height) < nzero)
		return true;

	return false;
}

bool CSVGRectF::PtInRect(const CSVGPointF &pt)const
{
	if (pt.x >= x && pt.y >= y)
	{
		if (pt.x <= (x + width) && pt.y <= (y + height))
		{
			return true;
		}
	}
	return false;
}

bool CSVGRectF::Intersectrect(const CSVGRectF &rect)const
{
	if (x <= rect.x + rect.width &&
		y <= rect.y + rect.height&&
		x + width >= rect.x &&
		y + height >= rect.y)
		return true;

	return false;
}
bool CSVGRectF::IsIncludeRect(const CSVGRectF &rect) const
{
	if (x <= rect.x
		&&y <= rect.y
		&&x + width >= rect.x + rect.width
		&&y + height >= rect.y + rect.height)
		return true;

	return false;
}

CSVGRectF CSVGRectF::matrixTransform(const CSVGMatrix &_mat)
{
	CSVGPointF ptlt(x, y);
	CSVGPointF ptrt(x + width, y);
	CSVGPointF ptlb(x, y + height);
	CSVGPointF ptrb(x + width, y + height);
	ptlt = ptlt.matrixTransform(_mat);
	ptrt = ptrt.matrixTransform(_mat);
	ptlb = ptlb.matrixTransform(_mat);
	ptrb = ptrb.matrixTransform(_mat);

	float newx = SVGmin(SVGmin(SVGmin(ptlt.x, ptrt.x), ptlb.x), ptrb.x);
	float newy = SVGmin(SVGmin(SVGmin(ptlt.y, ptrt.y), ptlb.y), ptrb.y);
	float newbrx = SVGmax(SVGmax(SVGmax(ptlt.x, ptrt.x), ptlb.x), ptrb.x);
	float newbry = SVGmax(SVGmax(SVGmax(ptlt.y, ptrt.y), ptlb.y), ptrb.y);

	return CSVGRectF(newx, newy, newbrx - newx, newbry - newy);
}
CSVGRectF CSVGRectF::Inflate(float _x, float _y)
{
	CSVGRectF rc = *this;
	rc.x -= _x;
	rc.y -= _y;
	rc.width += 2 * _x;
	rc.height += 2 * _y;
	return rc;
}

bool CSVGRectF::RectInThis(const CSVGRectF &rect)
{
	if (rect.x >= x
		&& rect.y >= y
		&& rect.x + rect.width <= x + width
		&& rect.y + rect.height <= y + height
		)
		return true;
	return false;
}

void CSVGRectF::AddRect(const CSVGRectF& rect)
{
	if (x > rect.x)
	{
		x = rect.x;
	}
	if (y > rect.y)
	{
		y = rect.y;
	}
	if ((x + width) < (rect.x + rect.width))
	{
		width = rect.x + rect.width - x;
	}
	if ((y + height) < (rect.y + rect.height))
	{
		height = rect.y + rect.height - y;
	}
}

CSVGLineF CSVGRectF::ToLineF()const
{
	return CSVGLineF(x, y, x + width, y + height);
}
