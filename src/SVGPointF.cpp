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
#include "SVGPointF.h"

CSVGPoint::CSVGPoint()
{
	x = 0;
	y = 0;
}

CSVGPoint::CSVGPoint(int nx, int ny)
{
	x = nx;
	y = ny;
}

//////////////////////////////////////////////////////////////////////////
CSVGPointF::CSVGPointF()
{
	x = 0;
	y = 0;
}

CSVGPointF::CSVGPointF(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}
CSVGPointF::CSVGPointF(const CSVGPointF &p)
{
	this->x = p.x;
	this->y = p.y;
}
CSVGPointF::~CSVGPointF()
{

}
const CSVGPointF &CSVGPointF::operator =(const CSVGPointF&p)
{
	if (this != &p)
	{
		x = p.x;
		y = p.y;
	}
	return *this;
}
bool CSVGPointF::operator!(void)
{
	return (x == 0.0&&y == 0.0);
}

bool CSVGPointF::operator==(const CSVGPointF& o)
{
	return (x == o.x&&y == o.y);
}

CSVGPointF CSVGPointF::matrixTransform(const CSVGMatrix &_mat)
{
	CSVGPointF res(x*_mat.m_a + y*_mat.m_c + _mat.m_e,
		x*_mat.m_b + y*_mat.m_d + _mat.m_f);
	return res;
}

float CSVGPointF::length()const
{
	return std::sqrt(x * x + y * y);
}

CSVGPointF CSVGPointF::normalize()
{
	float inv = 1 / length();
	return CSVGPointF(x * inv, y * inv);
}

CSVGPointF CSVGPointF::add(const CSVGPointF &v)
{
	return CSVGPointF(x + v.x, y + v.y);
}

CSVGPointF CSVGPointF::multiply(float f)
{
	return CSVGPointF(x * f, y * f);
}

float CSVGPointF::dot(const CSVGPointF &v)
{
	return x * v.x + y * v.y;
}

float CSVGPointF::angle(const CSVGPointF &v)
{
	return (float)(std::acos(dot(v) / (length() * v.length())) * 180 / 3.1415926f);
}
