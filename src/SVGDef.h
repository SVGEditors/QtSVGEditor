﻿/**
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
#ifndef _SVG_DEF_
#define _SVG_DEF_
#include <cmath>
#include <stdio.h>
typedef unsigned int   SVGCOLORREF;
typedef unsigned char  SVGBYTE;
typedef unsigned short SVGWORD;

#define SVGRGBA(r,g,b,a)       ((SVGCOLORREF)(((SVGBYTE)(r)|((SVGWORD)((SVGBYTE)(g))<<8))|(((SVGWORD)(SVGBYTE)(b))<<16)|(((SVGWORD)(SVGBYTE)(a))<<24)))
#define SVGLOBYTE(w)           ((SVGBYTE)(((SVGCOLORREF)(w)) & 0xff))
#define SVGGetRValue(rgba)      (SVGLOBYTE(rgba))
#define SVGGetGValue(rgba)      (SVGLOBYTE(((SVGWORD)(rgba)) >> 8))
#define SVGGetBValue(rgba)      (SVGLOBYTE((rgba)>>16))
#define SVGGetAValue(rgba)      (SVGLOBYTE((rgba)>>24))
#define DBEQUAL_EXT(a,b,pre)  ( std::abs(((a)-(b))) <= std::pow(0.1,pre) )?1:0
#define DBEQUAL(a,b)		   DBEQUAL_EXT(a,b,4)
#define SVGPI                  3.1415926

#define SVGmax(a,b) (((a) > (b)) ? (a) : (b))
#define SVGmin(a,b) (((a) < (b)) ? (a) : (b))

typedef enum SVGDOM_NODETYPE
{
	XML_ELEMENT = 0,
	XML_ATTRIBUTE,
	XML_TEXT,
	XML_DATA,
	XML_ENTITYREFERENCE,
	XML_ENTITY,
	XML_PROCESSING,
	XML_COMMENT,
	XML_DOCUMENT,
	XML_DOCTYPE,
	XML_DOCFRAGMENT,
	XML_NOTATION,
	SVG_A,
	SVG_ALTGLYPH,
	SVG_ALTGLYPHDEF,
	SVG_ALTGLYPHITEM,
	SVG_ANIMATE,
	SVG_ANIMATECOLOR,
	SVG_ANIMATEMOTION,
	SVG_ANIMATETRANSFORM,
	SVG_CIRCLE,
	SVG_CLIPPATH,
	SVG_COLOR_PROFILE,
	SVG_CURSOR,
	SVG_DEFINITION_SRC,
	SVG_DEFS,
	SVG_DESC,
	SVG_ELLIPSE,
	SVG_FEBLEND,
	SVG_FECOLORMATRIX,
	SVG_FECOMPONENTTRANSFER,
	SVG_FECOMPOSITE,
	SVG_FECONVOLVEMATRIX,
	SVG_FEDIFFUSELIGHTING,
	SVG_FEDISPLACEMENTMAP,
	SVG_FEDISTANTLIGHT,
	SVG_FEFLOOD,
	SVG_FEFUNCA,
	SVG_FEFUNCB,
	SVG_FEFUNCG,
	SVG_FEFUNCR,
	SVG_FEGAUSSIANBLUR,
	SVG_FEIMAGE,
	SVG_FEMERGE,
	SVG_FEMERGENODE,
	SVG_FEMORPHOLOGY,
	SVG_FEOFFSET,
	SVG_FEPOINTLIGHT,
	SVG_FESPECULARLIGHTING,
	SVG_FESPOTLIGHT,
	SVG_FETILE,
	SVG_FETURBULENCE,
	SVG_FILTER,
	SVG_FONT,
	SVG_FONT_FACE,
	SVG_FONT_FACE_FORMAT,
	SVG_FONT_FACE_NAME,
	SVG_FONT_FACE_SRC,
	SVG_FONT_FACE_URI,
	SVG_FOREIGNOBJECT,
	SVG_G,
	SVG_GLYPH,
	SVG_GLYPHREF,
	SVG_HKERN,
	SVG_IMAGE,
	SVG_LINE,
	SVG_LINEARGRADIENT,
	SVG_MARKER,
	SVG_MASK,
	SVG_METADATA,
	SVG_MISSING_GLYPH,
	SVG_MPATH,
	SVG_PATH,
	SVG_PATTERN,
	SVG_POLYGON,
	SVG_POLYLINE,
	SVG_RADIALGRADIENT,
	SVG_RECT,
	SVG_SCRIPT,
	SVG_SET,
	SVG_STOP,
	SVG_STYLE,
	SVG_SVG,
	SVG_SWITCH,
	SVG_SYMBOL,
	SVG_TEXT,
	SVG_TEXTPATH,
	SVG_TITLE,
	SVG_TREF,
	SVG_TSPAN,
	SVG_USE,
	SVG_VIEW,
	SVG_VKERN,
	SVG_AAUDIO,
	SVG_TEXTCONTENT,
	SVG_GTEMP,
}TSVGElementType;

//////////////////////////////////////////////////////////////////////////
enum SVGDOM_ATTR
{
	ATTR_FILL = 0,
	ATTR_FILL_OPACITY,
	ATTR_FILL_RULE,
	ATTR_FONT_FAMILY,
	ATTR_FONT_SIZE,
	ATTR_FONT_SIZE_ADJUST,
	ATTR_FONT_STRETCH,
	ATTR_FONT_STYLE,
	ATTR_FONT_VARIANT,
	ATTR_FONT_WEIGHT,
	ATTR_TEXT_DECORATION,
	ATTR_TEXT_ANCHOR,
	ATTR_OPACITY,
	ATTR_STOP_COLOR,
	ATTR_STOP_OPACITY,
	ATTR_STYLE,
	ATTR_STROKE,
	ATTR_STROKE_DASHARRAY,
	ATTR_STROKE_DASHOFFSET,
	ATTR_STROKE_LINECAP,
	ATTR_STROKE_LINEJOIN,
	ATTR_STROKE_MITERLIMIT,
	ATTR_STROKE_OPACITY,
	ATTR_STROKE_WIDTH,
	ATTR_VISIBILITY,
	ATTR_DISPLAY,
	ATTR_WRITING_MODE,
	ATTR_CLASS,
	ATTR_FILTER,
	ATTR_CLIP_PATH,
	ATTR_X,
	ATTR_Y,
	ATTR_DX,
	ATTR_DY,
	ATTR_WIDTH,
	ATTR_HEIGHT,
	ATTR_RX,
	ATTR_RY,
	ATTR_X1,
	ATTR_X2,
	ATTR_Y1,
	ATTR_Y2,
	ATTR_CX,
	ATTR_CY,
	ATTR_R,
	ATTR_POINTS,
	ATTR_D,
	ATTR_XMLNS,
	ATTR_XMLNS_A,
	ATTR_XMLNS_XLINK,
	ATTR_FX,
	ATTR_FY,
	ATTR_OFFSET,
	ATTR_GRADIENTUNITS,
	ATTR_XLINK_HREF,
	ATTR_ID,
	ATTR_REFX,
	ATTR_REFY,
	ATTR_MARKERWIDTH,
	ATTR_MARKERHEIGHT,
	ATTR_MARKERUNITS,
	ATTR_ORIENT,
	ATTR_PATTERNCONTENTUNITS,
	ATTR_TRANSFORM,
	ATTR_GRADIENTTRANSFORM,
	ATTR_PATTERNTRANSFORM,
	ATTR_ONLOAD,
	ATTR_VIEWBOX,
	ATTR_TYVISIBILITY
};

enum TransformTypes
{
	TY_SVG_TRANSFORM_UNKNOWN = 0,
	TY_SVG_TRANSFORM_MATRIX = 1,
	TY_SVG_TRANSFORM_TRANSLATE = 2,
	TY_SVG_TRANSFORM_SCALE = 3,
	TY_SVG_TRANSFORM_ROTATE = 4,
	TY_SVG_TRANSFORM_SKEWX = 5,
	TY_SVG_TRANSFORM_SKEWY = 6
};

enum GDIColortype
{
	UNKNOWTYPE = 0,
	SINGLECOLOR = 1,
	LINEARCOLOR,
	RADIALCOLOR,
	PATTERNCOLOR,
};

enum COLOR_TYPE
{
	COLOR_NULL = 0,
	COLOR_RGB = 1,
	COLOR_RGBEX,
	COLOR_WORD,
	COLOR_URL,
};


enum  FocusType
{
	focusnormal = 0,
	focusrect8p = 1,
	focusrect4p = 2
};


enum MouseType
{
	MT_SELECT = 0,
	MT_SIZE = 1,
};

enum HandleType
{
	LEFT_TOP = 100001,
	TOP_MID = 100002,
	RIGHT_TOP = 100003,
	RIGHT_MID = 100004,
	RIGHT_BOTTOM = 100005,
	BOTTOM_MID = 100006,
	LEFT_BOTTOM = 100007,
	LEFT_MID = 100008,
	ROTATE_HANDLE = 100009
};

#define LINE_HEAD 1
#define LINE_END  2

enum FILL_TYPE
{
	FT_LEFT_RIGHT,
	FT_TOP_BOTTOM,
	FT_LEFTTOP_RIGHTBOTTOM,
	FT_RIGHTTOP_LEFTBOTTOM
};

enum SelectMode
{
	SM_NONE = 0,
	SM_MOVE,
	SM_SIZE,
	SM_ROTATE,
	SM_RECTSEL,
	SM_ROTATEMOVE
};

enum mousetype
{
	MT_MATRIX = 0,
	MT_COORDINATE
};

struct RECTF
{
	float    x;
	float    y;
	float    width;
	float    height;
	RECTF()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
};

#define  T_TRANSFORM   "transform"
#define  BACKRECT	   "backrect"

enum tooltype
{
	TOOLTYPE_SELECT = 0,
	TOOLTYPE_CIRCLE,
	TOOLTYPE_ELLIPSE,
	TOOLTYPE_IMAGE,
	TOOLTYPE_LINE,
	TOOLTYPE_MOVE,
	TOOLTYPE_PATHARC,
	TOOLTYPE_PATHFAN,
	TOOLTYPE_PATH,
	TOOLTYPE_POLYGON,
	TOOLTYPE_POLYLINE,
	TOOLTYPE_TEXT,
	TOOLTYPE_TEXTV,
	TOOLTYPE_RECT,
	TOOLTYPE_ZOOMIN,
	TOOLTYPE_ZOOMOUT,
	TOOLTYPE_MEASURE,
	TOOLTYPE_ARC,
	TOOLTYPE_CONTINUE_POLYLINE,
	TOOLTYPE_CONTINUE_POLYGON,
	TOOLTYPE_CONTINUE_ARC,
	TOOLTYPE_CONTINUE_PATH
};

#define  MIN_SIZE     5
#define  HITTEST_SIZE 3

#endif
