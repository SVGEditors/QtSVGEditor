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
#include "SVGGlobal.h"
#include <QDateTime>
#include <QTextCodec>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

CSVGGlobal global;

static const unsigned char basemap[256] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,
	255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255 };

CSVGGlobal::CSVGGlobal()
{
	nIndex = 0;
	InitColor();
	InitTagNameMap();
	InitAttrNameMap();
	std::wmemset(gspace, 0, 256);
	gspace[L'\r'] = 1;
	gspace[L'\n'] = 1;
	gspace[L'\t'] = 1;
	gspace[L' '] = 1;

	m_fmulmm2pixel = 1;
	m_fmulpt2mm = 1;
	m_fpt2pixel = 1;
	m_fpixel2pt = 1;
	m_fscreenscale = 1;
}

CSVGGlobal::~CSVGGlobal()
{

}

void CSVGGlobal::InitTagNameMap()
{
	m_vcTagName.clear();
	m_vcTagName.push_back(L"element_node");
	m_vcTagName.push_back(L"attribute_node");
	m_vcTagName.push_back(L"text_node");
	m_vcTagName.push_back(L"[CDATA[");
	m_vcTagName.push_back(L"entity_reference_node");
	m_vcTagName.push_back(L"entity_node");
	m_vcTagName.push_back(L"processing_instruction_node");
	m_vcTagName.push_back(L"comment_node");
	m_vcTagName.push_back(L"document_node");
	m_vcTagName.push_back(L"DOCTYPE");
	m_vcTagName.push_back(L"document_fragment_node");
	m_vcTagName.push_back(L"notation_node");
	m_vcTagName.push_back(L"a");
	m_vcTagName.push_back(L"altglyph");
	m_vcTagName.push_back(L"altglyphdef");
	m_vcTagName.push_back(L"altglyphitem");
	m_vcTagName.push_back(L"animate");
	m_vcTagName.push_back(L"animatecolor");
	m_vcTagName.push_back(L"animatemotion");
	m_vcTagName.push_back(L"animatetransform");
	m_vcTagName.push_back(L"circle");
	m_vcTagName.push_back(L"clipPath");
	m_vcTagName.push_back(L"color-profile");
	m_vcTagName.push_back(L"cursor");
	m_vcTagName.push_back(L"definition-src");
	m_vcTagName.push_back(L"defs");
	m_vcTagName.push_back(L"desc");
	m_vcTagName.push_back(L"ellipse");
	m_vcTagName.push_back(L"feblend");
	m_vcTagName.push_back(L"fecolormatrix");
	m_vcTagName.push_back(L"fecomponenttransfer");
	m_vcTagName.push_back(L"fecomposite");
	m_vcTagName.push_back(L"feconvolvematrix");
	m_vcTagName.push_back(L"fediffuselighting");
	m_vcTagName.push_back(L"fedisplacementmap");
	m_vcTagName.push_back(L"fedistantlight");
	m_vcTagName.push_back(L"feflood");
	m_vcTagName.push_back(L"fefunca");
	m_vcTagName.push_back(L"fefuncb");
	m_vcTagName.push_back(L"fefuncg");
	m_vcTagName.push_back(L"fefuncr");
	m_vcTagName.push_back(L"fegaussianblur");
	m_vcTagName.push_back(L"feimage");
	m_vcTagName.push_back(L"femerge");
	m_vcTagName.push_back(L"femergenode");
	m_vcTagName.push_back(L"femorphology");
	m_vcTagName.push_back(L"feoffset");
	m_vcTagName.push_back(L"fepointlight");
	m_vcTagName.push_back(L"fespecularlighting");
	m_vcTagName.push_back(L"fespotlight");
	m_vcTagName.push_back(L"fetile");
	m_vcTagName.push_back(L"feturbulence");
	m_vcTagName.push_back(L"filter");
	m_vcTagName.push_back(L"font");
	m_vcTagName.push_back(L"font-face");
	m_vcTagName.push_back(L"font-face-format");
	m_vcTagName.push_back(L"font-face-name");
	m_vcTagName.push_back(L"font-face-src");
	m_vcTagName.push_back(L"font-face-uri");
	m_vcTagName.push_back(L"foreignobject");
	m_vcTagName.push_back(L"g");
	m_vcTagName.push_back(L"glyph");
	m_vcTagName.push_back(L"glyphref");
	m_vcTagName.push_back(L"hkern");
	m_vcTagName.push_back(L"image");
	m_vcTagName.push_back(L"line");
	m_vcTagName.push_back(L"linearGradient");
	m_vcTagName.push_back(L"marker");
	m_vcTagName.push_back(L"mask");
	m_vcTagName.push_back(L"metadata");
	m_vcTagName.push_back(L"missing-glyph");
	m_vcTagName.push_back(L"mpath");
	m_vcTagName.push_back(L"path");
	m_vcTagName.push_back(L"pattern");
	m_vcTagName.push_back(L"polygon");
	m_vcTagName.push_back(L"polyline");
	m_vcTagName.push_back(L"radialGradient");
	m_vcTagName.push_back(L"rect");
	m_vcTagName.push_back(L"script");
	m_vcTagName.push_back(L"set");
	m_vcTagName.push_back(L"stop");
	m_vcTagName.push_back(L"style");
	m_vcTagName.push_back(L"svg");
	m_vcTagName.push_back(L"switch");
	m_vcTagName.push_back(L"symbol");
	m_vcTagName.push_back(L"text");
	m_vcTagName.push_back(L"textpath");
	m_vcTagName.push_back(L"title");
	m_vcTagName.push_back(L"tref");
	m_vcTagName.push_back(L"tspan");
	m_vcTagName.push_back(L"use");
	m_vcTagName.push_back(L"view");
	m_vcTagName.push_back(L"vkern");
	m_vcTagName.push_back(L"a:audio");
	m_vcTagName.push_back(L"textcontent");
	m_vcTagName.push_back(L"gtemp");
	m_vcTagName.push_back(L"tylayout");

	for (size_t i = 0; i < m_vcTagName.size(); i++)
	{
		m_tagNameHash.AddHash(m_vcTagName[i], (unsigned short)i);
	}
}

void  CSVGGlobal::InitAttrNameMap()
{
	m_vcAttrName.clear();
	m_vcAttrName.push_back(L"fill");
	m_vcAttrName.push_back(L"fill-opacity");
	m_vcAttrName.push_back(L"fill-rule");
	m_vcAttrName.push_back(L"font-family");
	m_vcAttrName.push_back(L"font-size");
	m_vcAttrName.push_back(L"font-size-adjust");
	m_vcAttrName.push_back(L"font-stretch");
	m_vcAttrName.push_back(L"font-style");
	m_vcAttrName.push_back(L"font-variant");
	m_vcAttrName.push_back(L"font-weight");
	m_vcAttrName.push_back(L"text-decoration");
	m_vcAttrName.push_back(L"text-anchor");
	m_vcAttrName.push_back(L"opacity");
	m_vcAttrName.push_back(L"stop-color");
	m_vcAttrName.push_back(L"stop-opacity");
	m_vcAttrName.push_back(L"style");
	m_vcAttrName.push_back(L"stroke");
	m_vcAttrName.push_back(L"stroke-dasharray");
	m_vcAttrName.push_back(L"stroke-dashoffset");
	m_vcAttrName.push_back(L"stroke-linecap");
	m_vcAttrName.push_back(L"stroke-linejoin");
	m_vcAttrName.push_back(L"stroke-miterlimit");
	m_vcAttrName.push_back(L"stroke-opacity");
	m_vcAttrName.push_back(L"stroke-width");
	m_vcAttrName.push_back(L"visibility");
	m_vcAttrName.push_back(L"display");
	m_vcAttrName.push_back(L"writing-mode");
	m_vcAttrName.push_back(L"class");
	m_vcAttrName.push_back(L"filter");
	m_vcAttrName.push_back(L"clip-path");
	m_vcAttrName.push_back(L"x");
	m_vcAttrName.push_back(L"y");
	m_vcAttrName.push_back(L"dx");
	m_vcAttrName.push_back(L"dy");
	m_vcAttrName.push_back(L"width");
	m_vcAttrName.push_back(L"height");
	m_vcAttrName.push_back(L"rx");
	m_vcAttrName.push_back(L"ry");
	m_vcAttrName.push_back(L"x1");
	m_vcAttrName.push_back(L"x2");
	m_vcAttrName.push_back(L"y1");
	m_vcAttrName.push_back(L"y2");
	m_vcAttrName.push_back(L"cx");
	m_vcAttrName.push_back(L"cy");
	m_vcAttrName.push_back(L"r");
	m_vcAttrName.push_back(L"points");
	m_vcAttrName.push_back(L"d");
	m_vcAttrName.push_back(L"xmlns");
	m_vcAttrName.push_back(L"xmlns:a");
	m_vcAttrName.push_back(L"xmlns:xlink");
	m_vcAttrName.push_back(L"fx");
	m_vcAttrName.push_back(L"fy");
	m_vcAttrName.push_back(L"offset");
	m_vcAttrName.push_back(L"gradientUnits");
	m_vcAttrName.push_back(L"xlink:href");
	m_vcAttrName.push_back(L"id");
	m_vcAttrName.push_back(L"refX");
	m_vcAttrName.push_back(L"refY");
	m_vcAttrName.push_back(L"markerWidth");
	m_vcAttrName.push_back(L"markerHeight");
	m_vcAttrName.push_back(L"markerUnits");
	m_vcAttrName.push_back(L"orient");
	m_vcAttrName.push_back(L"patternContentUnits");
	m_vcAttrName.push_back(L"transform");
	m_vcAttrName.push_back(L"gradientTransform");
	m_vcAttrName.push_back(L"patternTransform");
	m_vcAttrName.push_back(L"onload");
	m_vcAttrName.push_back(L"viewBox");
	m_vcAttrName.push_back(L"tyvisibility");

	for (size_t i = 0; i < m_vcAttrName.size(); i++)
	{
		m_attrNameHash.AddHash(m_vcAttrName[i], (unsigned short)i);
	}

	InitStyleAttr();
}

unsigned short CSVGGlobal::getIndexByTagName(const std::wstring &tagName)
{
	return m_tagNameHash.FindIndex(tagName);
}

std::wstring  CSVGGlobal::getTagNameByIndex(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_vcTagName.size())
	{
		return m_vcTagName[nIndex];
	}
	return L"";
}

unsigned short CSVGGlobal::getIndexByAttrName(const std::wstring &attrName)
{
	int nIndex = m_attrNameHash.FindIndex(attrName);
	if (nIndex < 0)
	{
		nIndex = m_attrNameHash.GetNextIndex();
		m_attrNameHash.AddHash(attrName, nIndex);
		m_vcAttrName.push_back(attrName);
	}
	return nIndex;
}

std::wstring  CSVGGlobal::getAttrNameByIndex(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_vcAttrName.size())
	{
		return m_vcAttrName[nIndex];
	}
	return L"";
}

std::wstring CSVGGlobal::GetColorByWord(const std::wstring &word)
{
	std::map<std::wstring, std::wstring>::iterator iter = m_colormap.find(word);
	if (iter != m_colormap.end())
	{
		return iter->second;
	}
	return L"";
}

void CSVGGlobal::InitColor()
{
	m_colormap[L"aliceblue"] = L"rgb(240,248,255)";
	m_colormap[L"antiquewhite"] = L"rgb(250,235,215)";
	m_colormap[L"aqua"] = L"rgb(0,255,255)";
	m_colormap[L"aquamarine"] = L"rgb(127,255,212)";
	m_colormap[L"azure"] = L"rgb(240,255,255)";
	m_colormap[L"beige"] = L"rgb(245,245,220)";
	m_colormap[L"bisque"] = L"rgb(255,228,196)";
	m_colormap[L"black"] = L"rgb(0,0,0)";
	m_colormap[L"blanchedalmond"] = L"rgb(255,235,205)";
	m_colormap[L"blue"] = L"rgb(0,0,255)";
	m_colormap[L"blueviolet"] = L"rgb(138,43,226)";
	m_colormap[L"brown"] = L"rgb(165,42,42)";
	m_colormap[L"burlywood"] = L"rgb(222,184,135)";
	m_colormap[L"cadetblue"] = L"rgb(95,158,160)";
	m_colormap[L"chartreuse"] = L"rgb(127,255,0)";
	m_colormap[L"chocolate"] = L"rgb(210,105,30)";
	m_colormap[L"coral"] = L"rgb(255,127,80)";
	m_colormap[L"lightpink"] = L"rgb(255,182,193)";
	m_colormap[L"lightsalmon"] = L"rgb(255,160,122)";
	m_colormap[L"lightseagreen"] = L"rgb(32,178,170)";
	m_colormap[L"lightskyblue"] = L"rgb(135,206,250)";
	m_colormap[L"lightslategray"] = L"rgb(119,136,153)";
	m_colormap[L"lightslategrey"] = L"rgb(119,136,153)";
	m_colormap[L"lightsteelblue"] = L"rgb(176,196,222)";
	m_colormap[L"lightyellow"] = L"rgb(255,255,224)";
	m_colormap[L"lime"] = L"rgb(0,255,0)";
	m_colormap[L"limegreen"] = L"rgb(50,205,50)";
	m_colormap[L"linen"] = L"rgb(250,240,230)";
	m_colormap[L"magenta"] = L"rgb(255,0,255)";
	m_colormap[L"maroon"] = L"rgb(128,0,0)";
	m_colormap[L"mediumaquamarine"] = L"rgb(102,205,170)";
	m_colormap[L"mediumblue"] = L"rgb(0,0,205)";
	m_colormap[L"mediumorchid"] = L"rgb(186,85,211)";
	m_colormap[L"cornflowerblue"] = L"rgb(100,149,237)";
	m_colormap[L"cornsilk"] = L"rgb(255,248,220)";
	m_colormap[L"crimson"] = L"rgb(220,20,60)";
	m_colormap[L"cyan"] = L"rgb(0,255,255)";
	m_colormap[L"darkblue"] = L"rgb(0,0,139)";
	m_colormap[L"darkcyan"] = L"rgb(0,139,139)";
	m_colormap[L"darkgoldenrod"] = L"rgb(184,134,11)";
	m_colormap[L"darkgray"] = L"rgb(169,169,169)";
	m_colormap[L"darkgreen"] = L"rgb(0,100,0)";
	m_colormap[L"darkgrey"] = L"rgb(169,169,169)";
	m_colormap[L"darkkhaki"] = L"rgb(189,183,107)";
	m_colormap[L"darkmagenta"] = L"rgb(139,0,139)";
	m_colormap[L"darkolivegreen"] = L"rgb(85,107,47)";
	m_colormap[L"darkorange"] = L"rgb(255,140,0)";
	m_colormap[L"darkorchid"] = L"rgb(153,50,204)";
	m_colormap[L"darkred"] = L"rgb(139,0,0)";
	m_colormap[L"darksalmon"] = L"rgb(233,150,122)";
	m_colormap[L"darkseagreen"] = L"rgb(143,188,143)";
	m_colormap[L"darkslateblue"] = L"rgb(72,61,139)";
	m_colormap[L"darkslategray"] = L"rgb(47,79,79)";
	m_colormap[L"darkslategrey"] = L"rgb(47,79,79)";
	m_colormap[L"darkturquoise"] = L"rgb(0,206,209)";
	m_colormap[L"darkviolet"] = L"rgb(148,0,211)";
	m_colormap[L"deeppink"] = L"rgb(255,20,147)";
	m_colormap[L"deepskyblue"] = L"rgb(0,191,255)";
	m_colormap[L"dimgray"] = L"rgb(105,105,105)";
	m_colormap[L"dimgrey"] = L"rgb(105,105,105)";
	m_colormap[L"dodgerblue"] = L"rgb(30,144,255)";
	m_colormap[L"firebrick"] = L"rgb(178,34,34)";
	m_colormap[L"floralwhite"] = L"rgb(255,250,240)";
	m_colormap[L"forestgreen"] = L"rgb(34,139,34)";
	m_colormap[L"fuchsia"] = L"rgb(255,0,255)";
	m_colormap[L"gainsboro"] = L"rgb(220,220,220)";
	m_colormap[L"ghostwhite"] = L"rgb(248,248,255)";
	m_colormap[L"gold"] = L"rgb(255,215,0)";
	m_colormap[L"goldenrod"] = L"rgb(218,165,32)";
	m_colormap[L"gray"] = L"rgb(128,128,128)";
	m_colormap[L"green"] = L"rgb(0,128,0)";
	m_colormap[L"greenyellow"] = L"rgb(173,255,47)";
	m_colormap[L"honeydew"] = L"rgb(240,255,240)";
	m_colormap[L"hotpink"] = L"rgb(255,105,180)";
	m_colormap[L"indianred"] = L"rgb(205,92,92)";
	m_colormap[L"indigo"] = L"rgb(75,0,130)";
	m_colormap[L"ivory"] = L"rgb(255,255,240)";
	m_colormap[L"khaki"] = L"rgb(240,230,140)";
	m_colormap[L"lavender"] = L"rgb(230,230,250)";
	m_colormap[L"lavenderblush"] = L"rgb(255,240,245)";
	m_colormap[L"mediumpurple"] = L"rgb(147,112,219)";
	m_colormap[L"mediumseagreen"] = L"rgb(60,179,113)";
	m_colormap[L"mediumslateblue"] = L"rgb(123,104,238)";
	m_colormap[L"mediumspringgreen"] = L"rgb(0,250,154)";
	m_colormap[L"mediumturquoise"] = L"rgb(72,209,204)";
	m_colormap[L"mediumvioletred"] = L"rgb(199,21,133)";
	m_colormap[L"midnightblue"] = L"rgb(25,25,112)";
	m_colormap[L"mintcream"] = L"rgb(245,255,250)";
	m_colormap[L"mistyrose"] = L"rgb(255,228,225)";
	m_colormap[L"moccasin"] = L"rgb(255,228,181)";
	m_colormap[L"navajowhite"] = L"rgb(255,222,173)";
	m_colormap[L"navy"] = L"rgb(0,0,128)";
	m_colormap[L"oldlace"] = L"rgb(253,245,230)";
	m_colormap[L"olive"] = L"rgb(128,128,0)";
	m_colormap[L"olivedrab"] = L"rgb(107,142,35)";
	m_colormap[L"orange"] = L"rgb(255,165,0)";
	m_colormap[L"orangered"] = L"rgb(255,69,0)";
	m_colormap[L"orchid"] = L"rgb(218,112,214)";
	m_colormap[L"palegoldenrod"] = L"rgb(238,232,170)";
	m_colormap[L"palegreen"] = L"rgb(152,251,152)";
	m_colormap[L"paleturquoise"] = L"rgb(175,238,238)";
	m_colormap[L"palevioletred"] = L"rgb(219,112,147)";
	m_colormap[L"papayawhip"] = L"rgb(255,239,213)";
	m_colormap[L"peachpuff"] = L"rgb(255,218,185)";
	m_colormap[L"peru"] = L"rgb(205,133,63)";
	m_colormap[L"pink"] = L"rgb(255,192,203)";
	m_colormap[L"plum"] = L"rgb(221,160,221)";
	m_colormap[L"powderblue"] = L"rgb(176,224,230)";
	m_colormap[L"purple"] = L"rgb(128,0,128)";
	m_colormap[L"red"] = L"rgb(255,0,0)";
	m_colormap[L"rosybrown"] = L"rgb(188,143,143)";
	m_colormap[L"royalblue"] = L"rgb(65,105,225)";
	m_colormap[L"saddlebrown"] = L"rgb(139,69,19)";
	m_colormap[L"salmon"] = L"rgb(250,128,114)";
	m_colormap[L"sandybrown"] = L"rgb(244,164,96)";
	m_colormap[L"seagreen"] = L"rgb(46,139,87)";
	m_colormap[L"seashell"] = L"rgb(255,245,238)";
	m_colormap[L"sienna"] = L"rgb(160,82,45)";
	m_colormap[L"silver"] = L"rgb(192,192,192)";
	m_colormap[L"skyblue"] = L"rgb(135,206,235)";
	m_colormap[L"slateblue"] = L"rgb(106,90,205)";
	m_colormap[L"slategray"] = L"rgb(112,128,144)";
	m_colormap[L"slategrey"] = L"rgb(112,128,144)";
	m_colormap[L"snow"] = L"rgb(255,250,250)";
	m_colormap[L"springgreen"] = L"rgb(0,255,127)";
	m_colormap[L"steelblue"] = L"rgb(70,130,180)";
	m_colormap[L"tan"] = L"rgb(210,180,140)";
	m_colormap[L"teal"] = L"rgb(0,128,128)";
	m_colormap[L"lawngreen"] = L"rgb(124,252,0)";
	m_colormap[L"lemonchiffon"] = L"rgb(255,250,205)";
	m_colormap[L"lightblue"] = L"rgb(173,216,230)";
	m_colormap[L"lightcoral"] = L"rgb(240,128,128)";
	m_colormap[L"lightcyan"] = L"rgb(224,255,255)";
	m_colormap[L"lightgoldenrodyellow"] = L"rgb(250,250,210)";
	m_colormap[L"lightgray"] = L"rgb(211,211,211)";
	m_colormap[L"lightgreen"] = L"rgb(144,238,144)";
	m_colormap[L"lightgrey"] = L"rgb(211,211,211)";
	m_colormap[L"thistle"] = L"rgb(216,191,216)";
	m_colormap[L"tomato"] = L"rgb(255,99,71)";
	m_colormap[L"turquoise"] = L"rgb(64,224,208)";
	m_colormap[L"violet"] = L"rgb(238,130,238)";
	m_colormap[L"wheat"] = L"rgb(245,222,179)";
	m_colormap[L"white"] = L"rgb(255,255,255)";
	m_colormap[L"whitesmoke"] = L"rgb(245,245,245)";
	m_colormap[L"yellow"] = L"rgb(255,255,0)";
	m_colormap[L"yellowgreen"] = L"rgb(154,205,50)";
}

COLOR_TYPE CSVGGlobal::GetColorType(const std::wstring &str)
{
	if (str.length() == 0)
		return COLOR_NULL;
	if (str == L"url")
		return COLOR_URL;
	else if (str.length() >= 1 && str[0] == L'#')
		return COLOR_RGB;
	else if (str.length() >= 3 && (str.substr(0, 3) == L"rgb" || str.substr(0, 3) == L"RGB"))
		return COLOR_RGBEX;
	else if (str.length() >= 4 && str.substr(0, 4) == L"none")
		return COLOR_NULL;
	else
		return COLOR_WORD;
}

SVGCOLORREF CSVGGlobal::StrToColor(const std::wstring &str)
{
	std::wstring str2 = str;
	COLOR_TYPE clrtype = GetColorType(str);
	if (clrtype == COLOR_WORD)
	{
		str2 = GetColorByWord(str.c_str());
	}
	SVGCOLORREF color = 0;
	int r = 0, g = 0, b = 0;
	if (std::wcsncmp(str2.c_str(), L"rgb", 3) == 0 || std::wcsncmp(str2.c_str(), L"RGB", 3) == 0)
	{
		int npos1 = str2.find(L'(');
		npos1++;
		int npos2 = str2.find(L')');
		int nSize = npos2 - npos1 + 1;
		if (nSize <= 0)
			return SVGRGBA(0, 0, 0, 0);
		wchar_t *buff = new wchar_t[npos2 - npos1 + 1];
		if (buff)
		{
			std::wmemset(buff, 0, npos2 - npos1 + 1);
			if (npos2 > npos1)
			{
				const wchar_t *pstr = str2.c_str();
				if ((int)str2.length() > (npos2 - npos1))
				{
					pstr += npos1;
					std::wmemcpy(buff, pstr, (npos2 - npos1));
				}
				const wchar_t* sep = L",";
				wchar_t *next_token;
				wchar_t* token = std::wcstok(buff, sep, &next_token);
				int ncount = 0;
				while (token)
				{
					int ndata = global.strtoi(token);
					switch (ncount)
					{
					case 0:
						r = ndata;
						break;
					case 1:
						g = ndata;
						break;
					case 2:
						b = ndata;
						break;
					}
					ncount++;
					token = std::wcstok(NULL, sep, &next_token);
				}
			}
			delete[]buff;
		}
	}
	else
	{
		if (str2 == L"none")
		{
			return SVGRGBA(0, 0, 0, 0);
		}
		str2 = g_replace_all_distinct(str2, L"$", L"#");
		if (str2.length() == 4)
		{
			wchar_t c1 = str2[1];
			wchar_t c2 = str2[2];
			wchar_t c3 = str2[3];
			str2.clear();
			str2.push_back(L'#');
			str2.push_back(c1);
			str2.push_back(c1);
			str2.push_back(c2);
			str2.push_back(c2);
			str2.push_back(c3);
			str2.push_back(c3);
		}
		std::swscanf(str2.c_str(), L"#%x", &color);
		r = SVGLOBYTE((color) >> 16);
		g = SVGLOBYTE(((SVGWORD)(color)) >> 8);
		b = SVGLOBYTE(color);
	}
	color = SVGRGBA(r, g, b, 255);

	return color;
}

QColor CSVGGlobal::StrToQColor(const QString &strcolor)
{
	SVGCOLORREF  clr = StrToColor(strcolor.toStdWString());
	return SVGColorToQ(clr);
}

QColor CSVGGlobal::StdToQColor(const std::wstring &strcolor)
{
	SVGCOLORREF  clr = StrToColor(strcolor);
	return SVGColorToQ(clr);
}

std::wstring CSVGGlobal::g_replace_all_distinct(const std::wstring &str, const std::wstring &old_value, const std::wstring &new_value)
{
	std::wstring strret = str;
	for (std::wstring::size_type pos(0); pos != std::wstring::npos; pos += new_value.length())
	{
		if ((pos = strret.find(old_value, pos)) != std::wstring::npos)
			strret.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return strret;
}

std::wstring CSVGGlobal::MakeRandStr()
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString strVal = datetime.toString("yyyy_MM_dd_hh_mm_ss_");
	std::wstring strrand = global.ftostring(++nIndex);
	std::wstring str = strVal.toStdWString() + strrand;
	return str;
}

std::wstring CSVGGlobal::MakeId()
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString strVal = datetime.toString("MMddhhmmss");
	std::wstring strrand = ftostring(++nIndex);
	std::wstring str = strVal.toStdWString() + strrand;
	return str;
}
std::wstring CSVGGlobal::ftostring(float fvalue)
{
	wchar_t buff[256];
	if (std::abs(fvalue - (int)fvalue) <= 0.0001)
	{
		std::swprintf(buff, 256, L"%d", (int)fvalue);
	}
	else
	{
		std::swprintf(buff, 256, L"%.2f", fvalue);
	}
	return buff;
}

std::wstring CSVGGlobal::ftostring4(float fvalue)
{
	wchar_t buff[256];
	if (std::abs(fvalue - (int)fvalue) <= 0.0001)
	{
		std::swprintf(buff, 256, L"%d", (int)fvalue);
	}
	else
	{
		std::swprintf(buff, 256, L"%.4f", fvalue);
	}
	return buff;
}

std::wstring CSVGGlobal::itostring(int nval)
{
	wchar_t buff[256];
	std::swprintf(buff, 256, L"%d", nval);
	return buff;
}

std::wstring CSVGGlobal::AddAttrTextValue(const std::wstring &attrname, float val, const std::wstring &strtext)
{
	std::wstring strtxt = strtext;
	strtxt += L" ";
	strtxt += attrname;
	strtxt += L"=\"";
	strtxt += ftostring(val);
	strtxt += L"\"";
	return strtxt;
}

std::wstring CSVGGlobal::AddAttrTextValue(const std::wstring &attrname, const std::wstring &val, const std::wstring &strtext)
{
	std::wstring strtxt = strtext;
	if (attrname.length() > 0 && val.length() > 0)
	{
		strtxt += L" ";
		strtxt += attrname;
		strtxt += L"=\"";
		strtxt += val;
		strtxt += L"\"";
	}
	return strtxt;
}

bool CSVGGlobal::ISSpace(int nVal)
{
	if (nVal == '\t' || nVal == '\r' || nVal == '\n' || nVal == ' ')
	{
		return true;
	}
	return false;
}

bool CSVGGlobal::ISDigit(int nVal)
{
	if (nVal >= '0' && nVal <= '9')
	{
		return true;
	}
	return false;
}

int CSVGGlobal::strtoi(const std::wstring &str)
{
	if (str.length() == 0)
		return 0;
	return (int)atof_3(str);
}

float  CSVGGlobal::atof_3(const std::wstring &str)
{
	if (str.length() == 0)
		return 0;
	wchar_t *pBuff = new wchar_t[str.length() + 1];
	std::wmemset(pBuff, 0, str.length() + 1);
	std::wmemcpy(pBuff, str.c_str(), str.length());

	while (global.ISSpace((int)(wchar_t)*pBuff))
		++pBuff;

	int nSign = 1;
	if (*pBuff && *pBuff == L'-')
	{
		nSign = -1;
		++pBuff;
	}
	int nval = 0;
	while (*pBuff&&global.ISDigit(*pBuff))
	{
		nval = 10 * nval + (*pBuff - L'0');
		++pBuff;
	}
	if (*pBuff == L'.')
		++pBuff;
	int  nPower = 1;
	while (*pBuff&&global.ISDigit(*pBuff))
	{
		nval = 10 * nval + (*pBuff - L'0');
		nPower *= 10;
		if (nPower == 1000)
			break;
		if (*pBuff == L'e' || *pBuff == L'E')
			break;
		++pBuff;
	}
	float fval = (float)(nSign*nval) / nPower;

	while (*pBuff && (*pBuff != L'%') && (*pBuff != L'e') && (*pBuff != L'E') && (*pBuff != L'i') && (*pBuff != L'p') && (*pBuff != L'P'))
		++pBuff;
	bool bhaveE = false;
	if (*pBuff && (*pBuff == L'e' || *pBuff == L'E') && *(pBuff + 1) != L'm')
	{
		bhaveE = true;
	}
	if (*pBuff == L'e' && *(pBuff + 1) == L'm')
	{
		fval *= 16;
	}
	if (*pBuff == L'i' && *(pBuff + 1) == L'n')
	{
		fval *= 96;
	}
	if (*pBuff == L'p' && *(pBuff + 1) == L't')
	{
		fval *= m_fpt2pixel;
	}
	if (*pBuff == L'%')
	{
		fval = fval / 100;
	}
	if (bhaveE)
	{
		std::wstring strValue = ++pBuff;
		if (strValue.length() > 0)
		{
			int nSqrt = QString::fromStdWString(strValue).toInt();
			fval = fval * pow((float)10.0, nSqrt);
		}
	}
	return fval;
}

std::wstring CSVGGlobal::QColorToStr(const QColor &clr)
{
	return ColorToStr(SVGRGBA(clr.red(), clr.green(), clr.blue(), clr.alpha()));
}

QColor CSVGGlobal::SVGColorToQ(const SVGCOLORREF & color)
{
	QColor qclr(SVGGetRValue(color), SVGGetGValue(color), SVGGetBValue(color));
	return qclr;
}

std::wstring CSVGGlobal::ColorToStr(const SVGCOLORREF & color)
{
	int r = SVGGetRValue(color);
	int g = SVGGetGValue(color);
	int b = SVGGetBValue(color);

	std::wstring str;
	str = L"#";
	wchar_t szBuffer[8];
	std::swprintf(szBuffer, 8, L"%X", r);
	if (r <= 16)
		str += L"0";
	str += szBuffer;
	std::swprintf(szBuffer, 8, L"%X", g);
	if (g <= 16)
		str += L"0";
	str += szBuffer;
	std::swprintf(szBuffer, 8, L"%X", b);
	if (b <= 16)
		str += L"0";
	str += szBuffer;
	return str;
}

void CSVGGlobal::InitStyleAttr()
{
	std::wmemset(m_styleAttr, 0, 256);
	m_styleAttr[m_attrNameHash.FindIndex(L"visibility")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"display")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"fill")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"fill-opacity")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"fill-rule")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-family")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-size")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-size-adjust")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-stretch")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-style")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-variant")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"font-weight")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"text-decoration")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"text-anchor")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"opacity")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stop-color")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stop-opacity")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"style")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"class")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-dasharray")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-dashoffset")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-linecap")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-linejoin")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-miterlimit")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-opacity")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"stroke-width")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"clip-path")] = 1;
	m_styleAttr[m_attrNameHash.FindIndex(L"tyvisibility")] = 1;
}

bool CSVGGlobal::IsStyleAttr(int nIndex)
{
	if (nIndex >= 0 && nIndex < 255 && m_styleAttr[nIndex] == 1)
		return true;
	return false;
}
std::wstring CSVGGlobal::UrlStringGetId(const std::wstring & str)
{
	std::wstring ret = str;
	size_t pos = ret.find(L'#');
	if (pos != std::wstring::npos)
	{
		ret = ret.substr(pos + 1, ret.length() - pos - 1);
	}
	pos = ret.find(L')');
	if (pos != std::wstring::npos)
	{
		ret = ret.substr(0, pos);
	}
	return ret;
}

std::vector<std::wstring> CSVGGlobal::tokenize(const std::wstring &str, const std::wstring &delimiters)
{
	std::vector<std::wstring> tokens;
	std::wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
	std::wstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while (std::wstring::npos != pos || std::wstring::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
	return tokens;
}


int CSVGGlobal::GetQuadrant(const CSVGPointF & ptStart, const CSVGPointF & ptEnd)
{
	int nType = 0;
	if ((ptEnd.y - ptStart.y) >= 0 && (ptEnd.x - ptStart.x) > 0)
	{
		nType = 1;
	}
	else if ((ptEnd.y - ptStart.y) > 0 && (ptEnd.x - ptStart.x) <= 0)
	{
		nType = 2;
	}
	else if ((ptEnd.y - ptStart.y) <= 0 && (ptEnd.x - ptStart.x) < 0)
	{
		nType = 3;
	}
	else if ((ptEnd.y - ptStart.y) < 0 && (ptEnd.x - ptStart.x) >= 0)
	{
		nType = 4;
	}
	return nType;
}

CSVGPointF CSVGGlobal::GetParaPostion(const CSVGPointF & ptStart, const CSVGPointF & ptEnd, double dDistance, bool bStart)
{
	CSVGPointF GetPos;
	CSVGPointF ptPos;
	if (bStart)
		ptPos = ptStart;
	else
		ptPos = ptEnd;

	double digree = atan(std::fabs(ptEnd.y - ptStart.y) / std::fabs(ptEnd.x - ptStart.x));
	int nType = GetQuadrant(ptStart, ptEnd);
	switch (nType)
	{
	case 1:
	{
		GetPos.x = float(ptPos.x + dDistance * sin(digree));
		GetPos.y = float(ptPos.y - dDistance * cos(digree));
	}
	break;
	case 2:
	{
		GetPos.x = float(ptPos.x + dDistance * sin(digree));
		GetPos.y = float(ptPos.y + dDistance * cos(digree));
	}
	break;
	case 3:
	{
		GetPos.x = float(ptPos.x - dDistance * sin(digree));
		GetPos.y = float(ptPos.y + dDistance * cos(digree));
	}
	break;
	case 4:
	{
		GetPos.x = float(ptPos.x - dDistance * sin(digree));
		GetPos.y = float(ptPos.y - dDistance * cos(digree));
	}
	break;
	default:
		break;
	}
	return GetPos;
}

void CSVGGlobal::GetparallelLine(const CSVGPointF &ptStart, const CSVGPointF & ptEnd, CSVGPointF &GetPtStart, CSVGPointF &GetPtEnd, double dDistance)
{
	GetPtStart = GetParaPostion(ptStart, ptEnd, dDistance, true);
	GetPtEnd = GetParaPostion(ptStart, ptEnd, dDistance, false);
}

CSVGPointF CSVGGlobal::GetPosInLine(const CSVGPointF & ptStart, const CSVGPointF & ptEnd, double dDistance)
{
	float fxoffset = ptEnd.x - ptStart.x;
	if (std::fabs(fxoffset) < 0.0001)
	{
		fxoffset = 0.0001f;
	}
	double digree = atan(std::fabs(ptEnd.y - ptStart.y) / std::fabs(ptEnd.x - ptStart.x));
	int nType = GetQuadrant(ptStart, ptEnd);
	CSVGPointF GetPos;
	switch (nType)
	{
	case 1:
	{
		GetPos.x = float(ptEnd.x - dDistance * cos(digree));
		GetPos.y = float(ptEnd.y - dDistance * sin(digree));
	}
	break;
	case 2:
	{
		GetPos.x = float(ptEnd.x + dDistance * cos(digree));
		GetPos.y = float(ptEnd.y - dDistance * sin(digree));
	}
	break;
	case 3:
	{
		GetPos.x = float(ptEnd.x + dDistance * cos(digree));
		GetPos.y = float(ptEnd.y + dDistance * sin(digree));
	}
	break;
	case 4:
	{
		GetPos.x = float(ptEnd.x - dDistance * cos(digree));
		GetPos.y = float(ptEnd.y + dDistance * sin(digree));
	}
	break;
	default:
		break;
	}
	return GetPos;

}

int quad(CSVGPointF po, CSVGPointF pt)
{
	int re;
	if (pt.x < po.x)
	{
		if (pt.y < po.y)
			re = 2;
		else
			re = 1;
	}
	else
	{
		if (pt.y < po.y)
			re = 3;
		else
			re = 0;
	}
	return re;
}
bool CSVGGlobal::PtInPolygonEx(const std::vector<CSVGPointF>& polygon, CSVGPointF& pt)
{
	size_t n = (int)polygon.size();
	if (n == 0)
		return false;
	int oldquad, newquad;
	size_t a, b, wind = 0;;
	size_t lastpt = n - 1;
	oldquad = quad(pt, polygon[lastpt]);
	for (size_t i = 0; i < n; i++)
	{
		newquad = quad(pt, polygon[i]);
		if (newquad != oldquad)
		{
			if (((oldquad + 1) & 3) == newquad)
				wind++;
			else if (((newquad + 1) & 3) == oldquad)
				wind--;
			else
			{
				a = (int)polygon[lastpt].y - (int)polygon[i].y;
				a *= (int)(pt.x - polygon[lastpt].x);
				b = (int)polygon[lastpt].x - (int)polygon[i].x;
				a += (int)polygon[lastpt].y * b;
				b *= (int)pt.y;
				if (a > b)
					wind += 2;
				else
					wind -= 2;
			}
		}
		lastpt = i;
		oldquad = newquad;
	}
	if (wind)
		return true;
	return false;
}

bool CSVGGlobal::PtInPolygonEx(const std::list<CSVGPointF>& polygon, CSVGPointF& pt)
{
	std::list<CSVGPointF>::const_iterator iter = polygon.begin();
	std::list<CSVGPointF>::const_iterator iterend = polygon.end();
	std::vector<CSVGPointF>vcpoints;
	for (; iter != iterend; ++iter)
	{
		vcpoints.push_back(*iter);
	}
	return PtInPolygonEx(vcpoints, pt);
}


int CSVGGlobal::base64_decode(const  char *src, int srclen, char *out, int ndeslen)
{
	if (!src || srclen == 0 || !out || ndeslen == 0)
		return 0;
	unsigned long t = 0;
	unsigned long y = 0;
	unsigned long z = 0;
	unsigned char c;
	int g = 3;
	for (int i = 0; i < srclen; ++i)
	{
		c = (unsigned char)basemap[src[i]];
		if (c == 255)
			return 0;
		if (c == 253)
			continue;
		if (c == 254)
		{
			c = 0;
			g--;
		}
		t = (t << 6) | c;
		if (++y == 4)
		{
			if (z > ((unsigned long)ndeslen - 1))
				return 0;
			out[z++] = (unsigned char)((t >> 16) & 255);
			if (g > 1) out[z++] = (unsigned char)((t >> 8) & 255);
			if (g > 2) out[z++] = (unsigned char)(t & 255);
			y = t = 0;
		}
	}
	return z;
}

QString CSVGGlobal::GetCorrectUnicode(const QByteArray &ba)
{
	QTextCodec::ConverterState state;
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QString text = codec->toUnicode(ba.constData(), ba.size(), &state);
	if (state.invalidChars > 0)
	{
		text = QTextCodec::codecForName("GBK")->toUnicode(ba);
	}
	else
	{
		text = ba;
	}
	return text;
}


inline std::wstring &ltrim(std::wstring &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

inline std::wstring &rtrim(std::wstring &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

inline std::wstring &trim(std::wstring &str)
{
	std::wstring::size_type pos = str.find_last_not_of(' ');
	if (pos != std::wstring::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if (pos != std::wstring::npos)
		{
			str.erase(0, pos);
		}
	}
	else
	{
		str.erase(str.begin(), str.end());
	}
	return str;
}
