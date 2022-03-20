
#include "ToolFun.h"
_USE_MATH_DEFINES
#include "math.h"
#include <stdlib.h>
#include <ShlObj.h>
#include <io.h>
#include <direct.h>
#include "SVGGlobal.h"
#define ZERO (float)(1E-5)

float gndip = 0.9;

float getdpi()
{
	/*if (gndip > 0.9)
		return gndip;
	Gdiplus::Bitmap bitmap(10, 10);
	Gdiplus::Graphics *pGraphic = Gdiplus::Graphics::FromImage(&bitmap);
	float fdpix = 120;
	if (pGraphic)
	{
		fdpix = pGraphic->GetDpiX();
		delete pGraphic;
	}	
	if (DBEQUAL(fdpix, 120))
	{
		gndip = 1.25;
	}
	else if (DBEQUAL(fdpix, 144))
	{
		gndip = 1.5;
	}
	else if (DBEQUAL(fdpix, 192))
	{
		gndip = 2.0;
	}
	else
	{
		gndip = 1.0;
	}
	return gndip;*/
	return 1.0;
}


float  atof_3(const char* _String)
{
	if(!_String)
		return 0.0;
	while (isspace((int)(unsigned char)*_String))   
		++_String;  
	int nSign = 1;
	//保存符号位
	if(*_String && *_String == '-')
	{
		nSign = -1;
		++_String;
	}
	//保存整数位
	int nval = 0;
	while(*_String&&isdigit(*_String))
	{
		nval = 10*nval+(*_String-'0');
		++_String;
	}
	//小数点
	if(*_String == '.')
		++_String;
	//小数部分
	int  nPower = 1;
	while(*_String&&isdigit(*_String))
	{
		nval = 10*nval+(*_String-'0');
		nPower *= 10;
		if(nPower == 1000)
			break;
		if(*_String == 'e' || *_String == 'E')
			break;
		++_String;
	}
	float fval = (float)(nSign*nval)/nPower;
	//科学计数法处理 如:8.000000e-002
	//去掉e

	while(*_String && (*_String != 'e') && (*_String != 'E') && (*_String != 'i'))
		++_String;
	bool bhaveE = false;
	if(*_String && (*_String == 'e' || *_String == 'E' ) && *(_String+1) != 'm')
	{
		bhaveE = true;
	}
	//单位是em的情况
	if (*_String == 'e' && *(_String + 1) == 'm')
	{
		fval *= 12;
		fval *= getdpi();
	}
	if (*_String == 'i' && *(_String + 1) == 'n')
	{
		fval *= 96;
		fval *= getdpi();
	}
	if(bhaveE)
	{
		int nSqrt = atoi((++_String));
		fval = fval*pow((float)10.0,nSqrt);
	}

	return fval;
}

// -------------------------------------------------------------------------
// 返回顶角在o点，起始边为os，终止边为oe的夹角
// (单位：弧度)  
// -------------------------------------------------------------------------
float g_angle(POINT o,POINT s,POINT e)
{
	float cosfi,fi,norm;
	float dsx = (float)s.x - (float)o.x;
	float dsy = (float)s.y - (float)o.y;
	float dex = (float)e.x - (float)o.x;
	float dey = (float)e.y - (float)o.y;
	cosfi=dsx*dex+dsy*dey;
	norm = (dsx * dsx + dsy * dsy) * (dex * dex + dey * dey);
	if( norm <= ZERO)//起点和终点重合的情况
		norm = ZERO;

	cosfi /= sqrt( norm );
	if (cosfi >= 1.0 ) 
		return 0;
	if (cosfi <= -1.0 ) 
		return -(float)M_PI;
	fi=acos(cosfi);
	if (dsx*dey-dsy*dex>0) 
		return fi;// 说明矢量os 在矢量 oe的顺时针方向
	return -fi;
}

std::string g_replace_all_distinct(std::string str,const std::string old_value,const std::string new_value)   
{ 
	std::string strret = str;
	for(std::string::size_type pos(0); pos!=std::string::npos; pos+=new_value.length()) 
	{   
		if((pos=strret.find(old_value,pos))!=std::string::npos)   
			strret.replace(pos,old_value.length(),new_value);   
		else
			break;   
	}   
	return strret;   
}   

inline COLOR_TYPE GetColorType(const char* str)
{
	if(!str || strlen(str) == 0)
		return COLOR_NULL;
	if (strncmp(str, "url", 3) == 0)
		return COLOR_URL;
	else if (strncmp(str, "#", 1) == 0)
		return COLOR_RGB;
	else if (strncmp(str, "rgb", 3) == 0 || strncmp(str, "RGB", 3) == 0)
		return COLOR_RGBEX;
	else if (strncmp(str, "none", 4) == 0)
		return COLOR_NULL;
	else
		return COLOR_WORD;
}

IStream * MakeStreamFromBuff(const char *pBuff, int nSize)
{
	if (nSize <= 0)
		return NULL;
	IStream *I = NULL;
	LARGE_INTEGER liTemp = { 0 };
	CreateStreamOnHGlobal(NULL, true, &I);
	I->Seek(liTemp, STREAM_SEEK_SET, NULL);
	I->Write(pBuff, nSize, NULL);
	return I;
}

std::string GetIdByUrl(std::string url)
{
	std::string ret = url;
	size_t pos = ret.find('#');
	if (pos != std::string::npos)
	{
		ret = ret.substr(pos+1, ret.length() - pos-1);
	}
	pos = ret.find(')');
	if (pos != std::string::npos)
	{
		ret = ret.substr(0, pos);
	}
	return ret;
}
char colorbuff[32];
COLORREF StrToColor(const char *str)
{
	if (!str)
		return 0;
	memset(colorbuff, 0, 32);
	memcpy(colorbuff, str, strlen(str));

	COLOR_TYPE clrtype = GetColorType(colorbuff);
	if(clrtype == COLOR_WORD)
	{//如果是单词类型的颜色,先转换为普通颜色
		std::string wordclr = global.GetColorByWord(str);
		memcpy(colorbuff, wordclr.c_str(), wordclr.length());
	}
	COLORREF color = 0;
	//处理rgb(255,0,0)形式的颜色
	if (strncmp(colorbuff, "rgba", 4) == 0 || strncmp(colorbuff, "RGBA", 4) == 0)
	{
		int r = 0, g = 0, b = 0, a = 0;
		char *pTemp = colorbuff;
		pTemp += 5;
		for (int i = 0; i < 4; ++i)
		{
			const char * val = pTemp;
			while (*pTemp != 0 && *pTemp != ',' && *pTemp != ')')
			{
				pTemp++;
			}
			char c = *pTemp;
			*pTemp = 0;
			if (i == 0)
			{
				r = atoi(val);
			}
			else if (i == 1)
			{
				g = atoi(val);
			}
			else if (i == 2)
			{
				b = atoi(val);
			}
			else if (i == 3)
			{
				float fval = atof_3(val);
				fval = fval * 255;
				if (fval > 255)
					fval = 255;
				a = (int)fval;
			}
			*pTemp = c;
			if (*pTemp != 0)
				pTemp++;
		}
		color = TRGBA(r, g, b, a);
	}
	else if(strncmp(colorbuff,"rgb",3) == 0 || strncmp(colorbuff,"RGB",3) == 0)
	{
		int r = 0,g = 0, b = 0;
		char *pTemp = colorbuff;
		pTemp += 4;
		for (int i = 0; i < 3; ++i)
		{
			const char * val = pTemp;
			while (*pTemp != 0 && *pTemp != ',' && *pTemp != ')')
			{
				pTemp++;				
			}
			char c = *pTemp;
			*pTemp = 0;
			if (i == 0)
			{
				r = atoi(val);
			}
			else if (i == 1)
			{
				g = atoi(val);
			}
			else if (i == 2)
			{
				b = atoi(val);
			}
			*pTemp = c;
			if (*pTemp != 0)
				pTemp++;
		}
		color = TRGBA(r, g, b, 255);
	}
	
	else
	{//#FF00ff
		for (int i = 0; i < strlen(colorbuff); ++i)
		{
			if (colorbuff[i] == '$')
			{
				colorbuff[i] = '#';
			}
		}
		if (strlen(colorbuff) == 4)
		{
			char temp[32];
			memcpy_s(temp, 32, colorbuff, 32);
			memset(colorbuff, 0, 32);
			colorbuff[0] = '#';
			for (int i = 1; i < 4;++i)
			{
				colorbuff[2*i-1] += temp[i];
				colorbuff[2*i] += temp[i];
			}

		}
		sscanf_s(colorbuff, "#%x",&color);
		color = TRGBA(GetBValue(color), GetGValue(color), GetRValue(color),255);
	}
	return color;
}

std::string ColorToStr(const COLORREF &color)
{
	int r = GetRValue(color);
	int g = GetGValue(color);
	int b = GetBValue(color);

	std::string str;
	str = "#";
	char szBuffer[8];
	wsprintf(szBuffer,"%X",r);
	if(r <= 16)
		str += "0";
	str += szBuffer;
	wsprintf(szBuffer,"%X",g);
	if(g <= 16)
		str += "0";
	str += szBuffer;
	wsprintf(szBuffer,"%X",b);
	if(b <= 16)
		str += "0";
	str += szBuffer;
	return str;
}


void EncodeCharUTF8( int nUChar, char* pszUTF8, int& nUTF8Len )
{
	if ( ! (nUChar & ~0x0000007f) ) // < 0x80
	{
		if ( pszUTF8 )
			pszUTF8[nUTF8Len++] = (char)nUChar;
		else
			++nUTF8Len;
	}
	else if ( ! (nUChar & ~0x000007ff) ) // < 0x800
	{
		if ( pszUTF8 )
		{
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0x7c0)>>6)|0xc0);
			pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
		}
		else
			nUTF8Len += 2;
	}
	else if ( ! (nUChar & ~0x0000ffff) ) // < 0x10000
	{
		if ( pszUTF8 )
		{
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0xf000)>>12)|0xe0);
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0xfc0)>>6)|0x80);
			pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
		}
		else
			nUTF8Len += 3;
	}
	else // < 0x110000
	{
		if ( pszUTF8 )
		{
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0x1c0000)>>18)|0xf0);
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0x3f000)>>12)|0x80);
			pszUTF8[nUTF8Len++] = (char)(((nUChar&0xfc0)>>6)|0x80);
			pszUTF8[nUTF8Len++] = (char)((nUChar&0x3f)|0x80);
		}
		else
			nUTF8Len += 4;
	}
}
void AToUTF8( std::string &strbuff)
{
	std::string strUTF8;
	int nLen = strbuff.length();
	int nBufferLen = nLen * 2 + 4;
	int nUChar, nCharLen;
	wchar_t wcChar;
	char szUTF8Char[4];
	const char *pANSI = strbuff.c_str();
	while ( *pANSI )
	{		
		nCharLen = mbtowc( &wcChar, pANSI, 5 );
		if ( nCharLen < 1 )
		{
			nCharLen = 1;
			wcChar = (wchar_t)' ';
		}
		pANSI += nCharLen;
		nUChar = (int)wcChar;
		nCharLen = 0;
		EncodeCharUTF8( nUChar, szUTF8Char, nCharLen );
		strUTF8.append(szUTF8Char,nCharLen);
	}
	strbuff = strUTF8;
}

int inline DecodeCharUTF8( const char*& pszUTF8 )
{
	if(pszUTF8 && *pszUTF8 == 0)
		return 0;
	int nUChar = (unsigned char)*pszUTF8;
	++pszUTF8;
	if ( nUChar & 0x80 )
	{
		int nExtraChars;
		if ( ! (nUChar & 0x20) )
		{
			nExtraChars = 1;
			nUChar &= 0x1f;
		}
		else if ( ! (nUChar & 0x10) )
		{
			nExtraChars = 2;
			nUChar &= 0x0f;
		}
		else if ( ! (nUChar & 0x08) )
		{
			nExtraChars = 3;
			nUChar &= 0x07;
		}
		else
			return -1;
		while ( nExtraChars-- && *pszUTF8)
		{
			if ( (*pszUTF8 & 0x80) )
			{
				nUChar = nUChar<<6;
				nUChar |= *pszUTF8 & 0x3f;
			}
			else
				return -1;
			++pszUTF8;
		}
	}
	return nUChar;
}

void UTF8ToA(const char * pszUTF8,int nLen,std::string &strANSI)
{
	strANSI = "";
	int nBufferLen = nLen + 4;
	strANSI.reserve(nBufferLen);
	int nUChar, nCharLen;
	char szANSI[2];
	const char* pUTF8 = pszUTF8;
	while ( *pUTF8 )
	{
		nUChar = DecodeCharUTF8( pUTF8 );
		if ( nUChar & ~0xffff )
			nCharLen = -1;
		else
			wctomb_s(&nCharLen,szANSI,MB_CUR_MAX,(wchar_t)nUChar);
		if ( nCharLen == -1 )
		{
		}
		else
		{
			strANSI.append(szANSI,nCharLen);
		}
	}
}

std::string trimRight(const std::string& str)
{
	if (str.begin() == str.end()) 
	{
		return str;
	}
	std::string t = str;
	for (std::string::iterator i = t.end() - 1; i != t.begin(); i--) {
		if (!isspace(*i)) {
			t.erase(i + 1, t.end());
			break;
		}
	}
	return t;
}
char buff[128];
const char* ftostring(float fvalue)
{
	memset(buff,0,128);
	if(abs(fvalue - (int)fvalue) <= 0.0001)
	{
		sprintf_s(buff,128,"%d",(int)fvalue);
	}
	else
	{
		sprintf_s(buff,128,"%.3f",fvalue);
	}
	return buff;
}

std::string UrlStringGetId(const std::string & str)
{
	std::string restr;
	int npos1 = str.find('#');
	npos1 ++;
	int npos2 = str.find(')');
	if(npos2 > npos1)
	{
		const int nSize = npos2-npos1+1;
		char *pChar = new char[nSize];
		memset(pChar,0,nSize);
		if(pChar)
		{
			const char *pstr = str.c_str();
			if((int)str.length() > (npos2-npos1))
			{
				pstr += npos1;
				memcpy_s(pChar,(npos2-npos1),pstr,(npos2-npos1));
			}
			restr = pChar;
			delete pChar;
		}
	}
	return restr;
}

void AddToVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs)
{
	if(toIndexs.size() == 0)
	{
		CopyVector(toIndexs,fromindexs);
		return;
	}

	bool bExist = false;
	for (int i = 0; i < (int)fromindexs.size(); ++i)
	{
		for (int j = 0; j < (int)toIndexs.size(); ++j)
		{
			if(fromindexs[i] == toIndexs[j])
			{
				bExist = true;
				break;
			}
		}
		if(bExist == false)
			toIndexs.push_back(fromindexs[i]);		
		bExist = false;
	}
}

void CopyVector(std::vector<long>&toIndexs,std::vector<long>&fromindexs)
{
	int nfromsize = fromindexs.size();
	for (int i = 0; i < nfromsize; ++i)
	{
		toIndexs.push_back(fromindexs[i]);		
	}
}

bool IsVectorSame(std::vector<long>&Index1,std::vector<long>&Index2)
{
	//个数不同,肯定不同
	if(Index1.size() != Index2.size())
		return false;
	//采用都加到一个数组,看看个数是否还是不变
	std::vector<long>newIndex;
	CopyVector(newIndex,Index1);
	AddToVector(newIndex,Index2);
	if(newIndex.size() == Index1.size())
		return true;

	return false;
}

std::vector<std::string> g_tokenize(const std::string str,const std::string delimiters)
{
	std::vector<std::string> tokens;

	// skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// found a token, add it to the std::vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));

		// skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}

std::string GetRandStr()
{
	DWORD dw = GetTickCount();		
	char elementid[64] = {0};
	_ultoa_s(dw,elementid,16);
	int nrand = rand();
	char dstbuf[64] = {0};
	_ltoa_s(nrand,dstbuf,16);
	std::string str = elementid;
	str += dstbuf;
	return str;
}

int GetQuadrant(POINTF2 ptStart,POINTF2 ptEnd)
{
	//右边坐标轴属于自己象限
	int nType = 0;
	//第1限从0到上
	if ((  ptEnd.y-ptStart.y  ) >= 0 && (ptEnd.x - ptStart.x) > 0)
	{
		nType = 1;
	}
	//第2限从0到上
	else if (( ptEnd.y-ptStart.y ) > 0 && (ptEnd.x - ptStart.x) <= 0)
	{
		nType = 2;
	}
	//第3限从0到上
	else if(( ptEnd.y - ptStart.y) <= 0 && (ptEnd.x - ptStart.x) < 0)
	{
		nType = 3;		
	}
	//第4限从0到上
	else if((ptEnd.y-ptStart.y ) < 0 && (ptEnd.x - ptStart.x) >= 0)
	{
		nType = 4;
	}
	return nType;
}

POINTF2 GetParaPostion(POINTF2 ptStart,POINTF2 ptEnd,double dDistance,BOOL bStart)
{
	POINTF2 GetPos;
	POINTF2 ptPos;
	if(bStart)
		ptPos = ptStart;
	else
		ptPos = ptEnd;

	double digree = atan(fabs(ptEnd.y - ptStart.y)/fabs(ptEnd.x - ptStart.x));
	int nType = GetQuadrant(ptStart,ptEnd);
	switch(nType)
	{
	case 1:
		{
			GetPos.x = float(ptPos.x + dDistance*sin(digree));
			GetPos.y = float(ptPos.y - dDistance*cos(digree));
		}
		break;
	case 2:
		{
			GetPos.x = float(ptPos.x + dDistance*sin(digree));
			GetPos.y = float(ptPos.y + dDistance*cos(digree));
		}
		break;
	case 3:
		{
			GetPos.x = float(ptPos.x - dDistance*sin(digree));
			GetPos.y = float(ptPos.y + dDistance*cos(digree));
		}
		break;
	case 4:
		{
			GetPos.x = float(ptPos.x - dDistance*sin(digree));
			GetPos.y = float(ptPos.y - dDistance*cos(digree));
		}
		break;
	default:
		break;
	}
	return GetPos;
}

void GetparallelLine(POINTF2 ptStart,POINTF2 ptEnd,POINTF2 &GetPtStart,POINTF2 &GetPtEnd,double dDistance)
{
	GetPtStart = GetParaPostion(ptStart,ptEnd,dDistance,TRUE);
	GetPtEnd  = GetParaPostion(ptStart,ptEnd,dDistance,FALSE);
}

CSVGPoint GetPosInLine(CSVGPoint ptStart,CSVGPoint ptEnd,double dDistance)
{
	double digree = atan(fabs(ptEnd.y - ptStart.y)/fabs(ptEnd.x - ptStart.x));
	int nType = GetQuadrant(ptStart,ptEnd);
	CSVGPoint GetPos;
	switch(nType)
	{
	case 1:
		{
			GetPos.x = float(ptEnd.x - dDistance*cos(digree));
			GetPos.y = float(ptEnd.y - dDistance*sin(digree));
		}
		break;
	case 2:
		{
			GetPos.x = float(ptEnd.x + dDistance*cos(digree));
			GetPos.y = float(ptEnd.y - dDistance*sin(digree));
		}
		break;
	case 3:
		{
			GetPos.x = float(ptEnd.x + dDistance*cos(digree));
			GetPos.y = float(ptEnd.y + dDistance*sin(digree));
		}
		break;
	case 4:
		{
			GetPos.x = float(ptEnd.x - dDistance*cos(digree));
			GetPos.y = float(ptEnd.y + dDistance*sin(digree));
		}
		break;
	default:
		break;
	}
	return GetPos;

}

void stl_replace_string( std::string& context, std::string oldref, std::string newref )
{
	std::string::size_type indexCh = std::string::npos;
	std::string::size_type nlength = oldref.length();
	std::string::size_type pos = newref.length();
	do
	{
		indexCh = context.find(oldref);

		if ( indexCh != std::string::npos )
		{
			context.replace(indexCh, nlength, newref);
		}

	} while ( (indexCh = context.find(oldref, indexCh+pos)) != std::string::npos );
}




void GetparallelLine( CSVGPoint ptStart,CSVGPoint ptEnd,CSVGPoint &GetptStart
					 , CSVGPoint &GetptEnd,float dDistance )
{
	GetptStart = GetParaPostion(ptStart,ptEnd,dDistance,TRUE);
	GetptEnd  = GetParaPostion(ptStart,ptEnd,dDistance,FALSE);
}

int GetQuadrant(CSVGPoint ptStart,CSVGPoint ptEnd)
{
	//右边坐标轴属于自己象限
	int nType = 0;
	//第1限从0到上
	if ((  ptEnd.y-ptStart.y  ) >= 0 && (ptEnd.x - ptStart.x) > 0)
	{
		nType = 1;
	}
	//第2限从0到上
	else if (( ptEnd.y-ptStart.y ) > 0 && (ptEnd.x - ptStart.x) <= 0)
	{
		nType = 2;
	}
	//第3限从0到上
	else if(( ptEnd.y - ptStart.y) <= 0 && (ptEnd.x - ptStart.x) < 0)
	{
		nType = 3;		
	}
	//第4限从0到上
	else if((ptEnd.y-ptStart.y ) < 0 && (ptEnd.x - ptStart.x) >= 0)
	{
		nType = 4;
	}
	return nType;
}

CSVGPoint GetParaPostion(CSVGPoint ptStart,CSVGPoint ptEnd,float dDistance,BOOL bStart)
{
	CSVGPoint GetPos;
	CSVGPoint ptPos;
	if(bStart)
		ptPos = ptStart;
	else
		ptPos = ptEnd;

	double digree = atan(fabs(ptEnd.y - ptStart.y)/fabs(ptEnd.x - ptStart.x));
	int nType = GetQuadrant(ptStart,ptEnd);
	switch(nType)
	{
	case 1:
		{
			GetPos.x = (float)(ptPos.x + dDistance*sin(digree));
			GetPos.y = (float)(ptPos.y - dDistance*cos(digree));
		}
		break;
	case 2:
		{
			GetPos.x = (float)(ptPos.x + dDistance*sin(digree));
			GetPos.y = (float)(ptPos.y + dDistance*cos(digree));
		}
		break;
	case 3:
		{
			GetPos.x = (float)(ptPos.x - dDistance*sin(digree));
			GetPos.y = (float)(ptPos.y + dDistance*cos(digree));
		}
		break;
	case 4:
		{
			GetPos.x =(float) (ptPos.x - dDistance*sin(digree));
			GetPos.y = (float)(ptPos.y - dDistance*cos(digree));
		}
		break;
	default:
		break;
	}
	return GetPos;
}

int quad(CSVGPoint po,CSVGPoint pt)//求某顶点相对于所判点所处象限
{
	int re;
	if(pt.x < po.x)
	{
		if(pt.y < po.y)
			re = 2;
		else
			re = 1;
	}
	else
	{
		if(pt.y < po.y)
			re = 3;
		else
			re = 0;
	}
	return re;
}

//判断一个点是否在区域内
bool PtInPolygonEx(const std::vector<CSVGPoint>& polygon, CSVGPoint& pt)
{
	size_t n = (int)polygon.size();
	if(n == 0)
		return false;
	int oldquad,newquad;
	size_t a,b,wind = 0;;
	size_t lastpt = n-1;
	oldquad = quad(pt,polygon[lastpt]);
	for(size_t i = 0; i < n; i++)
	{
		newquad = quad(pt,polygon[i]);
		if(newquad != oldquad)
		{
			if(((oldquad+1)&3)==newquad)
				wind++;
			else if(((newquad+1)&3)==oldquad)
				wind--;
			else
			{
				a = (int)polygon[lastpt].y - (int)polygon[i].y;
				a *= (int)(pt.x - polygon[lastpt].x);
				b = (int)polygon[lastpt].x - (int)polygon[i].x;
				a += (int)polygon[lastpt].y * b;
				b *= (int)pt.y;
				if(a > b)
					wind += 2;
				else
					wind -= 2;
			}
		}
		lastpt = i;
		oldquad = newquad;
	}
	if(wind)
		return true;
	return false;
}

bool PtInPolygonEx(const std::list<CSVGPoint>& polygon, CSVGPoint& pt)
{
	std::list<CSVGPoint>::const_iterator iter = polygon.begin();
	std::list<CSVGPoint>::const_iterator iterend = polygon.end();
	std::vector<CSVGPoint>vcpoints;
	for (; iter != iterend;++iter)
	{
		vcpoints.push_back(*iter);
	}
	return PtInPolygonEx(vcpoints,pt);
}

bool GetInterPostion(CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint ptStart2,CSVGPoint ptEnd2,CSVGPoint& pInter)
{
	//如果两直线都平行于x轴或y轴
	if(abs(ptStart1.x - ptEnd1.x)<WUCHA && abs(ptStart2.x - ptEnd2.x) < WUCHA 
		|| (abs(ptStart1.y - ptEnd1.y)<WUCHA && abs(ptStart2.y - ptEnd2.y)<WUCHA))
		return false;
	//两直线平行(斜率相等)
	double dKline1 = 0;
	double dKline2 = 0;
	if(abs(ptStart1.x - ptEnd1.x) > WUCHA)
		dKline1 = (ptStart1.y-ptEnd1.y)/(ptStart1.x - ptEnd1.x);
	if(abs(ptStart2.x - ptEnd2.x) > WUCHA)
		dKline2 = (ptStart2.y - ptEnd2.y)/(ptStart2.x - ptEnd2.x);

	//2个斜率相等，且必须大于0
	if(abs(dKline1 - dKline2) < WUCHA && abs(dKline1) > WUCHA)
		return false;

	//直线1平行y轴
	if(abs(ptStart1.x - ptEnd1.x) < WUCHA)
	{
		pInter.x = ptStart1.x;
		pInter.y = (ptStart2.y-ptEnd2.y)*(ptStart1.x-ptStart2.x)/(ptStart2.x-ptEnd2.x) +ptStart2.y;
	}
	//直线2平行y轴
	else if (abs(ptStart2.x - ptEnd2.x) < WUCHA)
	{
		pInter.x = ptStart2.x;
		pInter.y = (ptStart1.y-ptEnd1.y)*(ptStart2.x-ptStart1.x)/(ptStart1.x-ptEnd1.x) +ptStart1.y;
	}
	//2直线都不平行y坐标轴
	else
	{
		pInter.x = (ptStart2.y-ptStart1.y + ptStart1.x*(ptStart1.y-ptEnd1.y)/(ptStart1.x-ptEnd1.x)	- ptStart2.x*(ptStart2.y-ptEnd2.y)/(ptStart2.x-ptEnd2.x))/((ptStart1.y-ptEnd1.y)/(ptStart1.x-ptEnd1.x)-(ptStart2.y-ptEnd2.y)/(ptStart2.x-ptEnd2.x));
		pInter.y = (ptStart1.y-ptEnd1.y)*(pInter.x-ptStart1.x)/(ptStart1.x-ptEnd1.x)+ptStart1.y;
	}
	return true;
}

bool IsPosInLineRight(CSVGPoint ptStart,CSVGPoint ptEnd,CSVGPoint pos)
{
	int nType = GetQuadrant(ptStart,ptEnd);
	if(abs(ptStart.x - ptEnd.x)< 0.00001)
	{
		switch(nType)
		{
		case 2:
			{
				if(pos.x >= ptStart.x)
					return TRUE;
				else
					return FALSE;
			}
			break;
		case 4:
			{
				if(pos.x <= ptStart.x)
					return TRUE;
				else
					return FALSE;
			}
			break;
		default:
			break;
		}
	}
	double dyValue = (ptStart.y - ptEnd.y)*(pos.x-ptStart.x)/(ptStart.x-ptEnd.x) + ptStart.y;
	switch(nType)
	{
	case 1:
	case 4:
		if(pos.y <= dyValue)
			return true;
		break;
	case 2:
	case 3:
		if(pos.y >= dyValue)
			return true;
		break;
	default:
		break;
	}
	return false;
}

void GetPolygonFillLines(std::vector<CSVGPoint>& vcPoint,std::vector<LINE>&Lines)
{
	std::vector<void*>tempvc;
	int nCount = (int)vcPoint.size();
	// 边结构数据类型
	typedef struct Edge{
		int ymax;  // 边的最大y坐标
		float x; // 与当前扫描线的交点x坐标
		float dx; // 边所在直线斜率的倒数
		struct Edge * pNext; // 指向下一条边
	}Edge, * LPEdge;

	int i=0,j=0,k=0;
	int y0=0,y1=0;  // 扫描线的最大和最小y坐标
	LPEdge pAET=NULL; // 活化边表头指针
	LPEdge * pET=NULL;  // 边表头指针

	pAET=new Edge; // 初始化表头指针，第一个元素不用
	tempvc.push_back(pAET);

	pAET->pNext=NULL;

	// 获取y方向扫描线边界
	y0=y1=(int)vcPoint[0].y;
	for(i=1;i<nCount;i++)
	{
		if(vcPoint[i].y<y0)
			y0=(int)vcPoint[i].y;
		else if(vcPoint[i].y>y1)
			y1=(int)vcPoint[i].y;
	}
	if(y0>=y1) return;

	// 初始化边表，第一个元素不用
	pET=new LPEdge[y1-y0+1];
	tempvc.push_back(pET);
	for(i=0;i<=y1-y0;i++)
	{
		pET[i]= new Edge;
		tempvc.push_back(pET[i]);
		pET[i]->pNext=NULL;
	}

	for(i=0;i<nCount;i++)
	{
		j=(i+1)%nCount; // 组成边的下一点
		if(vcPoint[i].y != vcPoint[j].y)// 如果该边不是水平的则加入边表
		{
			LPEdge peg;   // 指向该边的指针
			LPEdge ppeg;  // 指向边指针的指针

			// 构造边
			peg =new Edge;
			tempvc.push_back(peg);
			k=(vcPoint[i].y>vcPoint[j].y)?i:j;
			peg->ymax=(int)vcPoint[k].y; // 该边最大y坐标
			k=(k==j)?i:j;  
			peg->x=(float)vcPoint[k].x; // 该边与扫描线焦点x坐标
			if(vcPoint[i].y != vcPoint[j].y) 
				peg->dx=(float)(vcPoint[i].x-vcPoint[j].x)/(vcPoint[i].y-vcPoint[j].y);// 该边斜率的倒数
			peg->pNext=NULL;

			// 插入边
			ppeg=pET[(int)vcPoint[k].y-y0]; 
			while(ppeg->pNext)
				ppeg=ppeg->pNext;
			ppeg->pNext=peg;
		}// end if
	}// end for i

	// 扫描
	for(i=y0;i<=y1;i++)
	{
		LPEdge peg0=pET[i-y0]->pNext;
		LPEdge peg1=pET[i-y0];
		if(peg0)// 有新边加入
		{
			while(peg1->pNext)
				peg1=peg1->pNext;
			peg1->pNext=pAET->pNext;
			pAET->pNext=peg0;
		}

		// 按照x递增排序pAET
		peg0=pAET;
		while(peg0->pNext)
		{
			LPEdge pegmax=peg0;
			LPEdge peg1=peg0;
			LPEdge pegi=NULL;

			while(peg1->pNext)
			{
				if(peg1->pNext->x>pegmax->pNext->x)
					pegmax=peg1;
				peg1=peg1->pNext;
			}
			pegi=pegmax->pNext;
			pegmax->pNext=pegi->pNext;
			pegi->pNext=pAET->pNext;
			pAET->pNext=pegi;
			if(peg0 == pAET)
				peg0=pegi;
		}

		// 遍历活边表，画线
		peg0=pAET;
		while(peg0->pNext)
		{
			if(peg0->pNext->pNext)
			{
				LINE line;
				line.pt1.x = (int)peg0->pNext->x;
				line.pt1.y = i;
				line.pt2.x = (int)peg0->pNext->pNext->x;
				line.pt2.y = i;
				Lines.push_back(line);
				peg0=peg0->pNext->pNext;
			}
			else
				break;
		}

		// 把ymax=i的节点从活边表删除并把每个节点的x值递增dx
		peg0=pAET;
		while(peg0->pNext)
		{
			if(peg0->pNext->ymax < i+2)
			{
				peg1=peg0->pNext;
				peg0->pNext=peg0->pNext->pNext; //删除
				continue;
			}
			peg0->pNext->x+=peg0->pNext->dx; //把每个节点的x值递增dx
			peg0=peg0->pNext;
		}
	}

	for (size_t i = 0; i  <tempvc.size();++i)
	{
		delete tempvc[i];
	}
	tempvc.clear();
}

bool IsLineInter( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint ptStart2,CSVGPoint ptEnd2 )
{
	//头尾相交的时候不算.
	if(ptStart1 == ptStart2 || ptStart1 == ptEnd2
		||ptEnd1 == ptStart2 || ptEnd1 == ptEnd2)
		return false;
	CSVGPoint pInter;
	if(GetInterPostion(ptStart1,ptEnd1,ptStart2,ptEnd2,pInter))
	{
		if(PtInLine(ptStart1,ptEnd1,pInter))
			return true;
	}
	return false;
}

bool PtInLine( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter )
{
	float x0 = ptStart1.x;
	float y0 = ptStart1.y;
	float x1 = ptEnd1.x;
	float y1 = ptEnd1.y;
	if(DBEQUAL(x0,x1))
	{
		if(DBEQUAL(pInter.x,x0) && pInter.y >= min(y0,y1) && pInter.y <= max(y0,y1))
			return true;
	}
	else if(DBEQUAL(y0,y1))
	{
		if(DBEQUAL(pInter.y,y0))
			if(pInter.x >= min(x0,x1) && pInter.x <= max(x0,x1))
				return true;
	}
	float k1 = (y0-y1)/(x0-x1);
	float y = (pInter.x-x0)*k1+y0;
	if(abs(y-pInter.y) <= 1)
	{
		if(min(x0,x1) <= pInter.x && max(x0,x1) >= pInter.x
			&&min(y0,y1) <= pInter.y && max(y0,y1) >= pInter.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

float PtInLinePos( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter )
{
	float fret = 0;
	float x0 = ptStart1.x;
	float y0 = ptStart1.y;
	float x1 = ptEnd1.x;
	float y1 = ptEnd1.y;
	float fdis = ptEnd1.y-ptStart1.y;
	if(!(DBEQUAL(fdis,0)))
	{
		float fval = pInter.y-ptStart1.y;
		fret = fval/fdis;
	}
	fdis = ptEnd1.x-ptStart1.x;
	if(!(DBEQUAL(fdis,0)))
	{
		float fval = pInter.x-ptStart1.x;
		fret = fval/fdis;
	}
	if(fret < 0)
		fret = 0;
	if(fret > 1)
		fret = 1;
	return fret;
}

float PtInLinePos2( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pInter )
{
	float fret = 0;
	float x0 = ptStart1.x;
	float y0 = ptStart1.y;
	float x1 = ptEnd1.x;
	float y1 = ptEnd1.y;
	float fdis = ptEnd1.y-ptStart1.y;
	if(!(DBEQUAL(fdis,0)))
	{
		float fval = pInter.y-ptStart1.y;
		fret = fval/fdis;
	}
	fdis = ptEnd1.x-ptStart1.x;
	if(!(DBEQUAL(fdis,0)))
	{
		float fval = pInter.x-ptStart1.x;
		fret = fval/fdis;
	}
	return fret;
}

CSVGPoint GetVerPoint( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt )
{
	CSVGPoint retpt;
	float   abx   =   ptEnd1.x   -   ptStart1.x; 
	float   aby   =   ptEnd1.y   -   ptStart1.y; 
	float   acx   =   pt.x   -   ptStart1.x; 
	float   acy   =   pt.y   -   ptStart1.y;
	float k = abx*abx+aby*aby;
	if(DBEQUAL(k,0))
		return retpt;
	float   f   =   (abx*acx+aby*acy)/k;//注意ab必须是直线上的两个不同点 
	retpt.x   =   ptStart1.x   +   f*abx; 
	retpt.y   =   ptStart1.y   +   f*aby;
	return retpt;
}

CSVGPoint GetInterPoint( CSVGPoint ptStart1,CSVGPoint ptEnd1,CSVGPoint pt,float k )
{
	//y=(y1-y2)*(x-x1)/(x1-x2)+y1
	//y= disypt12*(x-x1)/disxpt12+y1;
	//y = kx+b
	//b = kx1-y1;
	//y = kx + (kx1-y1)

	float b = pt.y-k*pt.x;

	float disypt12 = ptStart1.y - ptEnd1.y;
	float disxpt12 = ptStart1.x - ptEnd1.x;

	//	k*x + b = disypt12 * (x-ptStart1.x)/disxpt12 + ptStart1.y;
	//(k*x + b - ptStart1.y )*disxpt12 = (x-ptStart1.x)*disypt12;
	//k*x*disxpt12 + b*disxpt12 - ptStart1.y * disxpt12 = x*disypt12 - ptStart1.x * disypt12;
	//k*x*disxpt12-x*disypt12 = ptStart1.y*disxpt12-b*disxpt12-ptStart1.x*disypt12;
	//x(k*disxpt12-disypt12) = ;
	float fdiv = k*disxpt12-disypt12;
	if(abs(fdiv) <= 0.001)
		return CSVGPoint(0,0);

	float x = (ptStart1.y*disxpt12-b*disxpt12-ptStart1.x*disypt12)/fdiv;
	float y = k*x + b ;
	CSVGPoint retpt(x,y);
	return retpt;
}


void GetArrow(CSVGPoint startpt,CSVGPoint endpt,float halfwidth,float height,std::vector<CSVGPoint>&Arrowpt)
{
	CSVGPoint ptPaStart;
	CSVGPoint ptPaEnd;
	GetparallelLine(startpt,endpt,ptPaStart,ptPaEnd,halfwidth);
	Arrowpt.push_back(ptPaEnd);
	GetparallelLine(startpt,endpt,ptPaStart,ptPaEnd,-halfwidth);
	Arrowpt.push_back(ptPaEnd);
	CSVGPoint ptEnd = GetPosInLine(startpt,endpt,-height);
	Arrowpt.push_back(ptEnd);
}

void GetArrowPoint(CSVGPoint pt1, CSVGPoint pt2, float width, float height, std::vector<CSVGPoint>& ArrowPts)
{
	if ( DB_EQUAL( pt1.x, pt2.x) )
	{
		int sign = ( pt1.y - pt2.y  > 0 ) ? 1 : -1;

		CSVGPoint pt( pt1.x, pt1.y + sign * height );
		ArrowPts.push_back( pt );
		CSVGPoint pt_1( pt1.x - width / 2, pt1.y );
		ArrowPts.push_back( pt_1 );
		CSVGPoint pt_2( pt1.x + width / 2, pt1.y );
		ArrowPts.push_back( pt_2 );

		CSVGPoint pt_3( pt2.x, pt2.y + (-1) * sign * height );
		ArrowPts.push_back( pt_3 );
		CSVGPoint pt_4( pt2.x - width / 2, pt2.y );
		ArrowPts.push_back( pt_4 );
		CSVGPoint pt_5( pt2.x + width / 2, pt2.y );
		ArrowPts.push_back( pt_5 );

		return;
	}
	else if ( DB_EQUAL( pt1.y, pt2.y ) )
	{
		int sign = ( pt1.x - pt2.x  > 0 ) ? 1 : -1;

		CSVGPoint pt( pt1.x + sign * height, pt1.y );
		ArrowPts.push_back( pt );
		CSVGPoint pt_1( pt1.x, pt1.y  - width / 2 );
		ArrowPts.push_back( pt_1 );
		CSVGPoint pt_2( pt1.x, pt1.y  + width / 2 );
		ArrowPts.push_back( pt_2 );

		CSVGPoint pt_3( pt2.x  - sign * height, pt2.y );
		ArrowPts.push_back( pt_3 );
		CSVGPoint pt_4( pt2.x, pt2.y  - width / 2 );
		ArrowPts.push_back( pt_4 );
		CSVGPoint pt_5( pt2.x, pt2.y  + width / 2 );
		ArrowPts.push_back( pt_5 );

		return;
	}

	float K = ( pt1.y - pt2.y ) / ( pt1.x - pt2.x );
	float Q = -( pt1.x - pt2.x ) / ( pt1.y - pt2.y );
	float SQRT_K = sqrt( 1 + pow( K, 2 ) );
	float SQRT_Q = sqrt( 1 + pow( Q, 2 ) );

	if ( pt1.x > pt2.x && pt1.y > pt2.y )	// 第一象限
	{
		CSVGPoint pt;
		pt.x = pt1.x + height / SQRT_K;
		pt.y = pt1.y + height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt1.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt1.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

		// wei dian
		pt.x = pt2.x - height / SQRT_K;
		pt.y = pt2.y - height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt2.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt2.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

	}
	else if ( pt1.x > pt2.x && pt1.y < pt2.y )	// 第四象限
	{
		CSVGPoint pt;
		pt.x = pt1.x + height / SQRT_K;
		pt.y = pt1.y + height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt1.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt1.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

		// wei dian
		pt.x = pt2.x - height / SQRT_K;
		pt.y = pt2.y - height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt2.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt2.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

	}
	else if ( pt1.x < pt2.x && pt1.y > pt2.y )	// 第二象限
	{
		CSVGPoint pt;
		pt.x = pt1.x + height / SQRT_K;
		pt.y = pt1.y + height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt1.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt1.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

		// wei dian
		pt.x = pt2.x + height / SQRT_K;
		pt.y = pt2.y + height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt2.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt2.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
	}
	else if ( pt1.x < pt2.x && pt1.y < pt2.y )	// 第三象限
	{
		CSVGPoint pt;
		pt.x = pt1.x - height / SQRT_K;
		pt.y = pt1.y - height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt1.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt1.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt1.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

		// wei dian
		pt.x = pt2.x + height / SQRT_K;
		pt.y = pt2.y + height * K / SQRT_K;
		ArrowPts.push_back( pt );
		pt.x = pt2.x + ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = pt2.x - ( width / 2 ) / SQRT_Q;
		pt.y = pt2.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

	}
}

void GetBroLineArrow( CSVGPoint pt1, CSVGPoint pt2, float width, std::vector<CSVGPoint>& ArrowPts )
{
	const float height = 8.0;

	if ( DB_EQUAL( pt1.x, pt2.x) )
	{
		int sign = ( pt1.y - pt2.y  > 0 ) ? 1 : -1;

		ArrowPts.push_back( pt1 );
		CSVGPoint pt_1( pt1.x - width / 2, pt1.y - sign * height );
		ArrowPts.push_back( pt_1 );
		CSVGPoint pt_2( pt1.x + width / 2, pt1.y - sign * height );
		ArrowPts.push_back( pt_2 );

		return;
	}
	else if ( DB_EQUAL( pt1.y, pt2.y ) )
	{
		int sign = ( pt1.x - pt2.x  > 0 ) ? 1 : -1;

		ArrowPts.push_back( pt1 );
		CSVGPoint pt_1( pt1.x - sign * height, pt1.y  - width / 2 );
		ArrowPts.push_back( pt_1 );
		CSVGPoint pt_2( pt1.x - sign * height, pt1.y  + width / 2 );
		ArrowPts.push_back( pt_2 );

		return;
	}

	float K = ( pt1.y - pt2.y ) / ( pt1.x - pt2.x );
	float Q = -( pt1.x - pt2.x ) / ( pt1.y - pt2.y );
	float SQRT_K = sqrt( 1 + pow( K, 2 ) );
	float SQRT_Q = sqrt( 1 + pow( Q, 2 ) );

	if ( pt1.x > pt2.x && pt1.y > pt2.y )	// 第一象限
	{
		CSVGPoint temp;
		temp.x = pt1.x - height / SQRT_K;
		temp.y = pt1.y - height * K / SQRT_K;

		ArrowPts.push_back( pt1 );
		CSVGPoint pt;
		pt.x = temp.x + ( width / 2 ) / SQRT_Q;
		pt.y = temp.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = temp.x - ( width / 2 ) / SQRT_Q;
		pt.y = temp.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
	}
	else if ( pt1.x > pt2.x && pt1.y < pt2.y )	// 第四象限
	{
		CSVGPoint temp;
		temp.x = pt1.x - height / SQRT_K;
		temp.y = pt1.y - height * K / SQRT_K;
		ArrowPts.push_back( pt1 );

		CSVGPoint pt;
		pt.x = temp.x + ( width / 2 ) / SQRT_Q;
		pt.y = temp.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = temp.x - ( width / 2 ) / SQRT_Q;
		pt.y = temp.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

	}
	else if ( pt1.x < pt2.x && pt1.y > pt2.y )	// 第二象限
	{
		CSVGPoint temp;
		temp.x = pt1.x + height / SQRT_K;
		temp.y = pt1.y + height * K / SQRT_K;
		ArrowPts.push_back( pt1 );
		CSVGPoint pt;
		pt.x = temp.x + ( width / 2 ) / SQRT_Q;
		pt.y = temp.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = temp.x - ( width / 2 ) / SQRT_Q;
		pt.y = temp.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );

	}
	else if ( pt1.x < pt2.x && pt1.y < pt2.y )	// 第三象限
	{
		CSVGPoint temp;
		temp.x = pt1.x + height / SQRT_K;
		temp.y = pt1.y + height * K / SQRT_K;
		CSVGPoint pt;
		ArrowPts.push_back( pt1 );
		pt.x = temp.x + ( width / 2 ) / SQRT_Q;
		pt.y = temp.y + ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
		pt.x = temp.x - ( width / 2 ) / SQRT_Q;
		pt.y = temp.y - ( width / 2 ) * Q / SQRT_Q;
		ArrowPts.push_back( pt );
	}
}


bool PtInRectF(CSVGRect rect,float x,float y )
{
	std::vector<CSVGPoint>Polygons;
	CSVGPoint pt1(rect.x,rect.y);
	Polygons.push_back(pt1);
	CSVGPoint pt2(rect.x+rect.width,rect.y);
	Polygons.push_back(pt2);
	CSVGPoint pt3(rect.x+rect.width,rect.y+rect.height);
	Polygons.push_back(pt3);
	CSVGPoint pt4(rect.x,rect.y+rect.height);
	Polygons.push_back(pt4);
	CSVGPoint pt(x,y);
	return ::PtInPolygonEx(Polygons,pt);
}

static const char *codes =   
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";  
  
static const unsigned char map[256] = {  
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
  
int base64_encode(const  char *src, int srclen, char *out,int ndeslen)  
{  
   unsigned long i, len2, leven;  
   char *p;  
   len2 = 4 * ((srclen + 2) / 3);  
   p = out; 
   leven = 3*(srclen / 3);  
   for (i = 0; i < leven; i += 3) {  
       *p++ = codes[src[0] >> 2];  
       *p++ = codes[((src[0] & 3) << 4) + (src[1] >> 4)];  
       *p++ = codes[((src[1] & 0xf) << 2) + (src[2] >> 6)];  
       *p++ = codes[src[2] & 0x3f];  
       src += 3;  
   }  
   if (i < (unsigned long)srclen) {  
       unsigned a = src[0];  
       unsigned b = (i+1 < (unsigned long)srclen) ? src[1] : 0;  
       unsigned c = 0;  
  
       *p++ = codes[a >> 2];  
       *p++ = codes[((a & 3) << 4) + (b >> 4)];  
       *p++ = (i+1 < (unsigned long)srclen) ? codes[((b & 0xf) << 2) + (c >> 6)] : '=';  
       *p++ = '=';  
   }  
   *p = '\0';  
     return p - out; 
}  
  
int base64_decode(const  char *src, int srclen, char *out,int ndeslen)  
{
	if(!src || srclen == 0 || !out || ndeslen == 0)
		return 0;
	unsigned long t = 0;
	unsigned long x = 0;
	unsigned long y = 0;
	unsigned long z = 0;
	unsigned char c;  
	int g = 3;  
	for (int i = 0; i < srclen; ++i)
	{  
		c = (unsigned char)map[src[i]];  
		if (c == 255) 
			return 0;  
		if (c == 253) 
			continue;
		if (c == 254) 
		{
			c = 0; 
			g--; 
		}  
		t = (t<<6)|c;  
		if (++y == 4) 
		{
			if(z > ((unsigned long)ndeslen-1))
				return 0;
			out[z++] = (unsigned char)((t>>16)&255);  
			if (g > 1) out[z++] = (unsigned char)((t>>8)&255);  
			if (g > 2) out[z++] = (unsigned char)(t&255);  
			y = t = 0;  
		}
	}  
	return z;  
}

std::string GetImageTempPath()
{
	std::string strPath;
	char DataPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL,DataPath,CSIDL_APPDATA,0); 
	strPath += DataPath;
	strPath += "\\SVGMaker";
	if(_access(strPath.c_str(),0) == -1)
	{
		_mkdir(strPath.c_str());
	}
	return strPath;
}

bool DbEqual(float f1, float f2)
{
	 return DB_EQUAL(f1, f2); 
}


#define BASE64_VALUE_SZ 256
int base64_value[BASE64_VALUE_SZ];
const unsigned char alphabet[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

Base64Utility::Base64Utility() {
	base_64_init();
}
void Base64Utility::base_64_init(void) {
	int i;
	for (i = 0; i < BASE64_VALUE_SZ; i++) base64_value[i] = -1;
	for (i = 0; i < 64; i++) base64_value[(int)alphabet[i]] = i;
	base64_value['='] = 0;
}
int Base64Utility::base64_encode(char *src, int srclen, char *dst, int tail) {
	int bits, char_count, len;
	char *o_char, *lim, *o_lim;
	unsigned char c;
	if (!src || !dst) return 0;
	len = srclen;
	lim = src + len;
	o_char = dst;
	o_lim = dst + (len * 4) / 3 + 1;
	char_count = 0;
	bits = 0;
	while ((src < lim) && (o_char < o_lim)) {
		c = *(src++);
		bits += c;
		char_count++;
		if (char_count == 3) {
			*(o_char++) = alphabet[bits >> 18];
			*(o_char++) = alphabet[(bits >> 12) & 0x3f];
			*(o_char++) = alphabet[(bits >> 6) & 0x3f];
			*(o_char++) = alphabet[bits & 0x3f];
			bits = 0;
			char_count = 0;
		}
		else {
			bits <<= 8;
		}
	}
	if (char_count != 0) {
		bits <<= 16 - (8 * char_count);
		*(o_char++) = alphabet[bits >> 18];
		*(o_char++) = alphabet[(bits >> 12) & 0x3f];
		if (char_count == 1) {
			if (tail) {
				*(o_char++) = '=';
				*(o_char++) = '=';
			}
		}
		else {
			*(o_char++) = alphabet[(bits >> 6) & 0x3f];
			if (tail) {
				*(o_char++) = '=';
			}
		}
	}
	*(o_char) = 0;
	return strlen(dst);
}
int Base64Utility::base64_decode(char *src, int srclen, char *dst) {
	int j;
	unsigned int k;
	int c, base_result_sz;
	long val;
	if (!src || !dst) return 0;
	base_result_sz = srclen;
	val = c = 0;
	for (j = 0; *src; src++) {
		k = (int)*src % BASE64_VALUE_SZ;
		if (base64_value[k] < 0) continue;
		val <<= 6;
		val += base64_value[k];
		if (++c < 4) continue;
		dst[j++] = (char)(val >> 16);
		dst[j++] = (val >> 8) & 0xff;
		dst[j++] = val & 0xff;
		val = c = 0;
	}
	switch (c) {
	case 2://xxxxxx xx0000
		dst[j++] = (val >> 4) & 0xff;
		break;
	case 3://XXXXXX XXxxxx xxxx00
		dst[j++] = (char)(val >> 10);
		dst[j++] = (val >> 2) & 0xff;
		break;
	}
	return j;
}


//////////////////////////////////////////////////////////////////////////
#define  MAX_TIMER_COUNT 32
double Timer::dbfFreq = 0;
bool Timer::bInit = false;
Timer::TInterval Timer::timerGroup[MAX_TIMER_COUNT] = { 0 };
void Timer::InitUpdate(void)
{
	if(!bInit)
	{
		bInit = true;
		LARGE_INTEGER litmp;
		memset(timerGroup,0,MAX_TIMER_COUNT*sizeof(TInterval));
		QueryPerformanceFrequency(&litmp);
		dbfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	}
}
void Timer::Start(int index,const char* lpszdesc)
{
	InitUpdate();
	LARGE_INTEGER litmp;
	QueryPerformanceCounter(&litmp);
	sprintf_s(timerGroup[index].szDesc,128,"%s",lpszdesc);
	timerGroup[index].llStart = litmp.QuadPart;// 获得初始值
}
void Timer::Stop(int index)
{
	LARGE_INTEGER litmp;
	QueryPerformanceCounter(&litmp);
	LONGLONG QPart = litmp.QuadPart;//获得中止值
	double dfMinus = (double)(QPart-timerGroup[index].llStart);
	timerGroup[index].dblInv += (dfMinus / dbfFreq);// 获得对应的时间值，单位为秒
}
void Timer::Print(int index,char* lpszBuf,int maxlen)
{
	sprintf_s(lpszBuf,maxlen-1,"Timer<%d-%s> : %.8f\n",index,timerGroup[index].szDesc,timerGroup[index].dblInv);
}
void Timer::Print(char* lpszBuf,int maxlen)
{
	int nLen = 0;
	for(int i=0;i<MAX_TIMER_COUNT;i++)
	{
		if(timerGroup[i].llStart>0)
			nLen += sprintf_s(lpszBuf+nLen,maxlen-nLen-1,"Timer<%d-%s> : %.8f\n",i,timerGroup[i].szDesc,timerGroup[i].dblInv);
	}
}

void Timer::ReSet(int index)
{
	timerGroup[index].dblInv = 0;
}

std::string Timer::GetTime(int index)
{
	char lpszBuf[256]= {0};
	sprintf_s(lpszBuf,255,"%.2f秒",timerGroup[index].dblInv/100);
	std::string str = lpszBuf;
	return str;
}

void Timer::OutPut()
{
	char msg[256]= {0};
	Timer::Print(msg,255);
	::OutputDebugString(msg);
}


void AddAttrTextValue(const char *pattr, float val, std::string &strtext)
{
	strtext += " ";
	strtext += pattr;
	strtext += "=\"";
	strtext += ftostring(val);
	strtext += "\"";
}

void AddAttrTextValue(const char *pattr, const char *val, std::string &strtext)
{
	if (pattr && val && strlen(val) > 0)
	{
		strtext += " ";
		strtext += pattr;
		strtext += "=\"";
		strtext += val;
		strtext += "\"";
	}	
}