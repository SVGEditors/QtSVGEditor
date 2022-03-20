#ifndef _TOOLFUN_
#define _TOOLFUN_
#include "ExportMarco.h"
#define  _USE_MATH_DEFINES
#include <math.h>
#include "SVGPoint.h"
#include "SVGRect.h"
#include "BasetypeDef.h"


#define TGetAValue(rgba)      (LOBYTE((rgba)>>24))

#define TRGBA(r,g,b,a)        ((COLORREF)((((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|((((DWORD)(BYTE)(a))<<24))))

#define DB_EQUAL_EXT(a,b,pre) ( abs(((a)-(b))) <= pow(0.1,pre) )?1:0
#define DB_EQUAL(a,b)		  DB_EQUAL_EXT(a,b,4)
#define PT_EQUAL(pt1,pt2)	  ( (pt1.x==pt2.x)&&(pt1.y==pt2.y))?1:0

struct POINTF2
{
	float x;
	float y;
	POINTF2()
	{
		x = 0;
		y = 0;
	}
	POINTF2(float fx ,float fy)
	{
		x = fx;
		y = fy;
	}
};


//字符串转浮点数。
float  atof_3(const char* _String);

COLOR_TYPE  GetColorType(const char* str);
//支持三种颜色表示方式 as: "#FF0000" or "$FF0000" or: rgb(255,255,0) or "blue"
COLORREF    StrToColor(const char *str);
std::string     ColorToStr(const COLORREF &color);
std::string g_replace_all_distinct(std::string str,const std::string old_value,const std::string new_value);
float g_angle(POINT o,POINT s,POINT e);//获得两个直线的夹角(弧度)
void stl_replace_string( std::string& context, std::string oldref, std::string newref );
std::string GetImageTempPath();

void  AToUTF8( std::string &strbuff);
void  UTF8ToA(const char * pszUTF8,int nLen,std::string &retstr);
std::string trimRight(const std::string& str);
const char* ftostring(float fvalue);
inline bool IsNodigitorAlpha(char c)
{
	if((c >='a' && c <='z') || (c >='A' && c <= 'Z') || (c >='0' && c <= '9') )
		return false;

	if(c == '+' || c == '-' || c == '.')
		return false;

	return true;
}
//根据URL字符串,返回里面的ID
//例如url(#linearGradient1)将返回linearGradient1
std::string UrlStringGetId(const std::string & str);

void  AddToVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs);
void  CopyVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs);
bool  IsVectorSame(std::vector<long>&Index1,std::vector<long>&Index2);

std::string  GetRandStr();
//将字符串根据分割符号分开
//str:输入的字符串
//delimiters:分割的标识符
std::vector<std::string>  g_tokenize(const std::string str,const std::string delimiters);
/*   /|\
	/ | \
   /  |  \
  /_ _|_ _\
其中halfwidth 为一个"--"
高度为:
|
|
|
|
*/


#include <vector>
#define WUCHA 0.001
#define DBEQUAL_EXT(a,b,pre) (( abs(((a)-(b))) <= pow(0.1,pre) )?1:0)
#define DBEQUAL(a,b)		  DBEQUAL_EXT(a,b,4)
struct LINE
{
	POINT pt1;
	POINT pt2;
};
//获取平行线（根据左线得右线） ,dDistance指两平行线的距离,如果dDistance>0;返回的线是在右边
void  GetparallelLine(CSVGPoint ptStart,CSVGPoint ptEnd,CSVGPoint &GetptStart,
					  CSVGPoint &GetptEnd,float dDistance);
int GetQuadrant(CSVGPoint ptStart,CSVGPoint ptEnd);
CSVGPoint GetParaPostion(CSVGPoint ptStart,CSVGPoint ptEnd,float dDistance,BOOL bStart);
bool PtInPolygonEx(const std::vector<CSVGPoint>& polygon, CSVGPoint& pt);
bool PtInPolygonEx(const std::list<CSVGPoint>& polygon, CSVGPoint& pt);
bool GetInterPostion(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint ptStart2,CSVGPoint ptEnd2,CSVGPoint& pInter);
bool IsPosInLineRight(CSVGPoint ptStart,CSVGPoint ptEnd,CSVGPoint pos);
void GetPolygonFillLines(std::vector<CSVGPoint>& vcPoint,std::vector<LINE>&Lines);
bool PtInLine(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter);
float PtInLinePos(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter);//左边0,右边1.中间占百分比.
float PtInLinePos2(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter);//左边<=0,右边>=1.中间占百分比.
bool IsLineInter(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint ptStart2,CSVGPoint ptEnd2);
CSVGPoint GetVerPoint(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt);//获得垂足点
CSVGPoint GetInterPoint(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt,float k);//获取交点


void GetArrow(CSVGPoint startpt,CSVGPoint endpt,float halfwidth,float height,std::vector<CSVGPoint>&Arrowpt);

void GetArrowPoint(CSVGPoint pt1, CSVGPoint pt2, float width, float height, std::vector<CSVGPoint>& ArrowPts);

void GetBroLineArrow( CSVGPoint pt1, CSVGPoint pt2, float width, std::vector<CSVGPoint>& ArrowPts );

bool PtInRectF(CSVGRect rect,float x,float y);

bool DbEqual(float f1, float f2);
IStream * MakeStreamFromBuff(const char *pBuff, int nSize);
int SVGCONTROL_API base64_encode(const  char *src, int srclen, char *out,int ndeslen);
int SVGCONTROL_API base64_decode(const  char *src, int srclen, char *out,int ndeslen);

std::string GetIdByUrl(std::string url);
void AddAttrTextValue(const char *pattr, float val, std::string &strtext);
void AddAttrTextValue(const char *pattr, const char *val, std::string &strtext);

class SVGCONTROL_API Base64Utility {
public:
	Base64Utility();
	int base64_encode(char *src, int srclen, char *dst, int tail);
	int base64_decode(char *src, int srclen, char *dst);
private:
	void base_64_init(void);
};

class SVGCONTROL_API Timer
{
public:
	static void Start(int index,const char* lpszDesc);
	static void Stop(int index);
	static void Print(int index,char* lpszBuf,int maxlen);
	static void Print(char* lpszBuf,int maxlen);
	static void OutPut();
	static void InitUpdate(void);
	static std::string GetTime(int index);
	static void ReSet(int index);

	struct TInterval
	{
		LONGLONG llStart;
		char     szDesc[128];
		double   dblInv;
	};

	static double dbfFreq;
	static bool bInit;
	static TInterval timerGroup[32];
};



#endif
