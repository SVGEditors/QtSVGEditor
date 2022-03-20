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


//�ַ���ת��������
float  atof_3(const char* _String);

COLOR_TYPE  GetColorType(const char* str);
//֧��������ɫ��ʾ��ʽ as: "#FF0000" or "$FF0000" or: rgb(255,255,0) or "blue"
COLORREF    StrToColor(const char *str);
std::string     ColorToStr(const COLORREF &color);
std::string g_replace_all_distinct(std::string str,const std::string old_value,const std::string new_value);
float g_angle(POINT o,POINT s,POINT e);//�������ֱ�ߵļн�(����)
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
//����URL�ַ���,���������ID
//����url(#linearGradient1)������linearGradient1
std::string UrlStringGetId(const std::string & str);

void  AddToVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs);
void  CopyVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs);
bool  IsVectorSame(std::vector<long>&Index1,std::vector<long>&Index2);

std::string  GetRandStr();
//���ַ������ݷָ���ŷֿ�
//str:������ַ���
//delimiters:�ָ�ı�ʶ��
std::vector<std::string>  g_tokenize(const std::string str,const std::string delimiters);
/*   /|\
	/ | \
   /  |  \
  /_ _|_ _\
����halfwidth Ϊһ��"--"
�߶�Ϊ:
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
//��ȡƽ���ߣ��������ߵ����ߣ� ,dDistanceָ��ƽ���ߵľ���,���dDistance>0;���ص��������ұ�
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
float PtInLinePos(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter);//���0,�ұ�1.�м�ռ�ٷֱ�.
float PtInLinePos2(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter);//���<=0,�ұ�>=1.�м�ռ�ٷֱ�.
bool IsLineInter(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint ptStart2,CSVGPoint ptEnd2);
CSVGPoint GetVerPoint(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt);//��ô����
CSVGPoint GetInterPoint(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt,float k);//��ȡ����


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
