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
#pragma once

enum PathType
{
	PATH_NULL = -1,
	PATH_M = 0,
	PATH_m0,
	PATH_L,
	PATH_l0,
	PATH_C,
	PATH_c0,
	PATH_Q,
	PATH_q0,
	PATH_A,
	PATH_a0,
	PATH_H,
	PATH_h0,
	PATH_V,
	PATH_v0,
	PATH_S,
	PATH_s0,
	PATH_T,
	PATH_t0,
	PATH_Z,
	PATH_z0,
	PATH_LAST
};

static char lpPath[] = {
	'M',
	'm',
	'L',
	'l',
	'C',
	'c',
	'Q',
	'q',
	'A',
	'a',
	'H',
	'h',
	'V',
	'v',
	'S',
	's',
	'T',
	't',
	'Z',
	'z'
};

class  CSVGPathSeg
{
public:
	CSVGPathSeg(unsigned short type);
	virtual ~CSVGPathSeg();
	unsigned short getType();
	virtual int Encode(wchar_t*, int) { return 0; };
public:
	unsigned short segtype;
	float  x;
	float  y;
};

//////////////////////////////////////////////////////////////////////////
//rx ry x-axis-rotation large-arc-flag sweep-flag x y	
class   CSVGPathSegArc : public CSVGPathSeg
{
public:
	CSVGPathSegArc();
	float  r1;
	float  r2;
	float  angle;
	bool   largeArcFlag;
	bool   sweepFlag;
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
};
//////////////////////////////////////////////////////////////////////////
class   CSVGPathSegClosePath : public CSVGPathSeg
{
public:
	CSVGPathSegClosePath();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
};

//////////////////////////////////////////////////////////////////////////
class   CSVGPathSegCurvetoCubic : public CSVGPathSeg
{
public:
	CSVGPathSegCurvetoCubic();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
	float x1;
	float y1;

	float x2;
	float y2;
};

class   CSVGPathSegCurvetoQuadratic : public CSVGPathSeg
{
public:
	CSVGPathSegCurvetoQuadratic();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
	float x1;
	float y1;
};

class   CSVGPathSegCurvetoCubicSmooth : public CSVGPathSeg
{
public:
	CSVGPathSegCurvetoCubicSmooth();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
	float x2;
	float y2;
};

class   CSVGPathSegMoveto : public CSVGPathSeg
{
public:
	CSVGPathSegMoveto();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
};

class   CSVGPathSegLineto : public CSVGPathSeg
{
public:
	CSVGPathSegLineto();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
};

class   CSVGPathSegCurvetoQuadraticSmooth : public CSVGPathSeg
{
public:
	CSVGPathSegCurvetoQuadraticSmooth();
	virtual int Encode(wchar_t* lpszBuf, int maxlen);
};
