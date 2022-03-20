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
#include "SVGLineF.h"


CSVGLine::CSVGLine()
	:x1(0), y1(0), x2(0), y2(0)
{
}

CSVGLine::CSVGLine(const CSVGPoint &pt1, const  CSVGPoint& pt2)
{
	x1 = pt1.x;
	y1 = pt1.y;
	x2 = pt2.x;
	y2 = pt2.y;
}

CSVGLine::~CSVGLine()
{

}
CSVGLine::CSVGLine(int nx1, int ny1, int nx2, int ny2)
	:x1(nx1), y1(ny1), x2(nx2), y2(ny2)
{
}

const CSVGLine &CSVGLine::operator =(const CSVGLine&p)
{
	if (this != &p)
	{
		x1 = p.x1;
		y1 = p.y1;
		x2 = p.x2;
		y2 = p.y2;
	}
	return *this;
}

bool  CSVGLine::operator==(const CSVGLine& res) const
{
	if (x1 == res.x1
		&&y1 == res.y1
		&& x2 == res.x2
		&& y2 == res.y2)
	{
		return  true;
	}
	return false;
}

void CSVGLine::SetEmpty()
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
}

bool CSVGLine::IsEmpty()
{
	if (DBEQUAL(x1, 0)
		&& DBEQUAL(y1, 0)
		&& DBEQUAL(x2, 0)
		&& DBEQUAL(y2, 0))
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
CSVGLineF::CSVGLineF()
	:x1(0), y1(0), x2(0), y2(0)
{
}

CSVGLineF::CSVGLineF(const CSVGPointF &pt1, const  CSVGPointF& pt2)
{
	x1 = pt1.x;
	y1 = pt1.y;
	x2 = pt2.x;
	y2 = pt2.y;
}

CSVGLineF::~CSVGLineF()
{

}
CSVGLineF::CSVGLineF(float fx1, float fy1, float fx2, float fy2)
	:x1(fx1), y1(fy1), x2(fx2), y2(fy2)
{
}
const CSVGLineF &CSVGLineF::operator =(const CSVGLineF&p)
{
	if (this != &p)
	{
		x1 = p.x1;
		y1 = p.y1;
		x2 = p.x2;
		y2 = p.y2;
	}
	return *this;
}

bool  CSVGLineF::operator==(const CSVGLineF& res) const
{
	if ((DBEQUAL(x1, res.x1))
		&& (DBEQUAL(y1, res.y1))
		&& (DBEQUAL(x2, res.x2))
		&& (DBEQUAL(y2, res.y2))
		)
	{
		return  true;
	}
	return false;
}

void CSVGLineF::SetEmpty()
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
}

bool CSVGLineF::IsEmpty()
{
	if (DBEQUAL(x1, 0)
		&& DBEQUAL(y1, 0)
		&& DBEQUAL(x2, 0)
		&& DBEQUAL(y2, 0))
		return true;
	return false;
}

CSVGLineF CSVGLineF::matrixTransform(const CSVGMatrix &_mat)
{
	CSVGPointF pt1(x1, y1);
	CSVGPointF pt2(x2, y2);
	pt1 = pt1.matrixTransform(_mat);
	pt2 = pt2.matrixTransform(_mat);
	return CSVGLineF(pt1.x, pt1.y, pt2.x, pt2.y);
}
