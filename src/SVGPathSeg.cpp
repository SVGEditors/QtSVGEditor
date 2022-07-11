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
#include "SVGPathSeg.h"
#include "SVGGlobal.h"

CSVGPathSeg::CSVGPathSeg(unsigned short type)
	:segtype(type)
{
}

CSVGPathSeg::~CSVGPathSeg()
{
}

unsigned short CSVGPathSeg::getType()
{
	return segtype;
}


//////////////////////////////////////////////////////////////////////////
CSVGPathSegArc::CSVGPathSegArc()
	:CSVGPathSeg(PATH_A)
{

}

int CSVGPathSegArc::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nLen = std::swprintf(lpszBuf, maxlen, L"A%.2f %.2f %.2f %d %d %.2f %.2f", r1, r2, angle, largeArcFlag, sweepFlag, x, y);
	return nLen;
}
//////////////////////////////////////////////////////////////////////////
CSVGPathSegClosePath::CSVGPathSegClosePath()
	: CSVGPathSeg(PATH_Z)
{
	x = -100000;
	y = -100000;
}

int CSVGPathSegClosePath::Encode(wchar_t* lpszBuf, int maxlen)
{
	Q_UNUSED(maxlen);
	lpszBuf[0] = L'Z';
	lpszBuf[1] = L'\0';
	return 1;
}
//////////////////////////////////////////////////////////////////////////
CSVGPathSegCurvetoCubic::CSVGPathSegCurvetoCubic()
	: CSVGPathSeg(PATH_C)
{

}

int CSVGPathSegCurvetoCubic::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nLen = std::swprintf(lpszBuf, maxlen - 1, L"C%.2f %.2f %.2f %.2f %.2f %.2f", x1, y1, x2, y2, x, y);
	return nLen;
}
//////////////////////////////////////////////////////////////////////////
CSVGPathSegCurvetoQuadratic::CSVGPathSegCurvetoQuadratic()
	: CSVGPathSeg(PATH_Q)
{

}

int CSVGPathSegCurvetoQuadratic::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nLen = std::swprintf(lpszBuf, maxlen - 1, L"Q%.2f %.2f %.2f %.2f", x1, y1, x, y);
	return nLen;
}

CSVGPathSegCurvetoCubicSmooth::CSVGPathSegCurvetoCubicSmooth()
	: CSVGPathSeg(PATH_S)
{

}

int CSVGPathSegCurvetoCubicSmooth::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nLen = std::swprintf(lpszBuf, maxlen - 1, L"S%.2f %.2f %.2f %.2f", x2, y2, x, y);
	return nLen;
}

CSVGPathSegMoveto::CSVGPathSegMoveto()
	: CSVGPathSeg(PATH_M)
{

}

int CSVGPathSegMoveto::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nlen = std::swprintf(lpszBuf, maxlen - 1, L"M%.2f %.2f", x, y);
	return nlen;
}

CSVGPathSegLineto::CSVGPathSegLineto()
	: CSVGPathSeg(PATH_L)
{

}

int CSVGPathSegLineto::Encode(wchar_t* lpszBuf, int maxlen)
{
	int nlen = std::swprintf(lpszBuf, maxlen - 1, L"L%.2f %.2f", x, y);
	return nlen;
}
CSVGPathSegCurvetoQuadraticSmooth::CSVGPathSegCurvetoQuadraticSmooth()
	: CSVGPathSeg(PATH_T)
{

}
int CSVGPathSegCurvetoQuadraticSmooth::Encode(wchar_t* lpszBuf, int maxlen)
{
	Q_UNUSED(lpszBuf);
	Q_UNUSED(maxlen);
	return 0;
}
