
#include "TextDraw.h"
#include <tchar.h>

#define	TOP			1
#define MIDDLE		2
#define BOTTOM		3
#define JUSTIFIED	4

static int GetCharSize(HDC hdc,LPCTSTR lpChar,LPRECT rc)
{
	RECT results = {0,0,0,0};

	// Clean and draw the char to the canvas
	if (rc)
		FillRect(hdc,rc,(HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawText(hdc,lpChar,_tcslen(lpChar),rc,DT_CENTER);

	// Create an array of pixels with the char
	BITMAP b;
	HBITMAP hBmp = (HBITMAP)GetCurrentObject(hdc,OBJ_BITMAP);
	GetObject(hBmp,sizeof(BITMAP),&b);
	int size = b.bmHeight * b.bmWidthBytes ;
	BYTE *lpBits = new BYTE[ size ];    
	GetBitmapBits(hBmp,size,lpBits);

	// Check for empty characters
	if (lpChar[0] == _T(' ')) {
		SetRect(&results,b.bmWidth/3,b.bmHeight/4,b.bmWidth,b.bmHeight - b.bmHeight/2);
		CopyRect(rc,&results);
		return results.bottom - results.top;
	}

	// Loop through and find the highest point
	bool bTop = false,
		bBot = false,
		bRgt = false,
		bLft = false;
	int x;
	int y;
	// Get the highest point
	for (y=0;y<b.bmHeight,!bTop;y++)
	{
		for (x=0;x<b.bmWidth;x++)
		{
			int offset = y * b.bmWidthBytes + x / 8;
			BYTE pixel = *(BYTE *)(lpBits + offset) >> (7 - x%8);
			if ((pixel & 0x01) == 0x00) {
				results.top = y-1;
				bTop = true;
				break;
			}
		}
	}
	// Get the lowest point
	for (y=b.bmHeight-1;y>=0,!bBot;y--)
	{
		for (x=0;x<b.bmWidth;x++)
		{
			int offset = y * b.bmWidthBytes + x / 8;
			BYTE pixel = *(BYTE *)(lpBits + offset) >> (7 - x%8);
			if ((pixel & 0x01) == 0x00) {
				results.bottom = y+1;
				bBot = true;
				break;
			}
		}
	}
	// Get the leftmost point
	for (x=0;x<b.bmWidth,!bLft;x++)
	{
		for (y=0;y<b.bmHeight;y++)
		{
			int offset = y * b.bmWidthBytes + x / 8;
			BYTE pixel = *(BYTE *)(lpBits + offset) >> (7 - x%8);
			if ((pixel & 0x01) == 0x00) {
				results.left = x-1;
				bLft = true;
				break;
			}
		}
	}
	// Get the rightmost point
	for (x=b.bmWidth-1;x>=0,!bRgt;x--)
	{
		for (y=0;y<b.bmHeight;y++)
		{
			int offset = y * b.bmWidthBytes + x / 8;
			BYTE pixel = *(BYTE *)(lpBits + offset) >> (7 - x%8);
			if ((pixel & 0x01) == 0x00) {
				results.right = x+1;
				bRgt = true;
				break;
			}
		}
	}

	// Calculate the size and clean up
	if (rc)
		CopyRect(rc,&results);

	delete[] lpBits;

	return results.right - results.left;
}

static int OffsetColumn(LPRECT lpRect, int iPos, int iMaxWidth, int iSpace, int iFlags)
{
	bool bFirstCol = iPos == 0 ? true : false;

	int iWidth = ((lpRect[iPos].right - lpRect[iPos].left) - iMaxWidth)/2;
	//	iWidth--;

	while (lpRect[iPos].bottom != 0) {
		lpRect[iPos].right -= iWidth;
		OffsetRect(&lpRect[iPos],-iWidth,0);
		//	if (!bFirstCol)
		lpRect[iPos].right += iSpace/2;
		iPos++;
	}
	return lpRect[iPos-1].right - iWidth;
}

static int SpaceColumns(LPRECT lpRect, int iMaxHeight, int iTextlength, int iFlags)
{
	int iPos = 0;
	while (iPos < iTextlength) {
		int charCount = 0;
		int ColHeight = 0;

		// count the chars in the row
		while (lpRect[iPos].bottom != 0) {
			charCount++;
			ColHeight = lpRect[iPos].bottom;
			iPos++;
		}
		int CurPos = iPos - charCount;
		double posY = 0;

		// adjust positions
		for (int i=0; i<charCount; i++) {
			if (iFlags == BOTTOM) {
				posY = iMaxHeight - ColHeight;
				OffsetRect(&lpRect[CurPos],0,(int)posY);
				CurPos++;
			}
			else if (iFlags == MIDDLE) {
				posY = (iMaxHeight/2) - (ColHeight/2);
				OffsetRect(&lpRect[CurPos],0,(int)posY);
				CurPos++;
			}
			else if (iFlags == JUSTIFIED) {
				CurPos++;
				posY += (iMaxHeight - ColHeight)/((double)charCount-1);
				OffsetRect(&lpRect[CurPos],0,(int)posY);
			}
		}
		iPos++;
	}
	return 0;
}

static int OffsetChars(LPRECT lpRect, int iRectCount, int x, int y)
{
	// batch offset
	for (int i=0; i<iRectCount; i++) {
		OffsetRect(&lpRect[i],x,y);
	}
	return 0;
}

static void CalcRect(LPRECT lpRect, LPRECT lprcDest, int iRectCount)
{
	RECT rec = {0,0,0,0};
	// join all the rects together
	for (int i=0; i<iRectCount; i++) {
		UnionRect(&rec,&rec,&lpRect[i]);
	}
	CopyRect(lprcDest,&rec);
}

int DoDrawVertText(HDC hdc,LPCTSTR lpString,LPRECT lpRect,UINT uFormat,UINT HcharSpacing /*= 0*/, UINT VcharSpacing /*= 0*/)
{
	RECT rec = {0,0,0,0};
	int iCharHeight = DrawText(hdc,_T("W"),1,&rec,DT_CALCRECT);
	int iCharWidth  = (rec.right-rec.left)*2;
	int iTextLength = _tcsclen(lpString);

	int iCharSpacingX	= HcharSpacing;
	int iCharSpacingY	= VcharSpacing;

	int iTextLines		= 0;
	int iLongestLine	= 0;

	RECT *CharacterPositions;

	bool bVCenter = false;
	bool bHCenter = false;
	bool bRight   = false;
	bool bBottom  = false;
	bool bTop     = false;
	bool bCalc	  = false;
	bool bBufBuf  = false;

	int  iCharPlacement = 0;

	if ((uFormat & DV_TEXTTOP) == DV_TEXTTOP)
		iCharPlacement = TOP;
	else if ((uFormat & DV_TEXTCENTER) == DV_TEXTCENTER)
		iCharPlacement = MIDDLE;
	else if ((uFormat & DV_TEXTBOTTOM) == DV_TEXTBOTTOM)
		iCharPlacement = BOTTOM;
	else if ((uFormat & DV_TEXTJUSTIFY) == DV_TEXTJUSTIFY)
		iCharPlacement = JUSTIFIED;

	if ((uFormat & DV_HCENTER) == DV_HCENTER)
		bHCenter = true;
	if ((uFormat & DV_VCENTER) == DV_VCENTER)
		bVCenter = true;
	if ((uFormat & DV_BOTTOM) == DV_BOTTOM)
		bBottom = true;
	if ((uFormat & DV_RIGHT) == DV_RIGHT)
		bRight = true;
	if ((uFormat & DV_CALCRECT) == DV_CALCRECT)
		bCalc = true;
	if ((uFormat & DV_DOUBLEBUFFER) == DV_DOUBLEBUFFER)
		bBufBuf = true;
	if (!bVCenter && !bBottom)
		bTop = true;

	CharacterPositions = new RECT [iTextLength + 1];
	memset(CharacterPositions,0,(iTextLength + 1)*sizeof(RECT));

	// Calc the longest line and number of lines
	int iTmpLength = 0;int i=0;int s = 0;
	for (; i<iTextLength; i++,s+=_tclen(&lpString[s]),iTmpLength++ )
	{
		TCHAR tc[3] = {0,0,0};
		_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
		if (tc[0] == _T('\n') || i == iTextLength-1) 
		{
			if (iTmpLength > iLongestLine)
				iLongestLine = iTmpLength;
			iTextLines++;
			iTmpLength =0;
		}
		else
			SetRect(&CharacterPositions[i],0,0,iCharWidth,iCharHeight);
	}
	SetRect(&CharacterPositions[i-1],0,0,iCharWidth,iCharHeight);

	RECT  rStrRec = { 0, 0, iCharWidth, iCharHeight };
	HFONT hFont  = (HFONT)GetCurrentObject(hdc,OBJ_FONT);

	// Create a canvas for drawing a single characters
	HDC hCharDC			= CreateCompatibleDC(hdc);
	HBITMAP hCharBmp	= CreateBitmap(iCharWidth,iCharHeight,1,1,NULL);
	HBITMAP hOldCharBmp	= (HBITMAP)SelectObject(hCharDC,hCharBmp);
	HFONT hOldCharFont	= (HFONT)SelectObject(hCharDC,hFont);
	FillRect(hCharDC,&rStrRec,(HBRUSH)GetStockObject(WHITE_BRUSH));
	// black text on white background
	SetTextColor(hCharDC,RGB(0,0,0));
	SetBkColor(hCharDC,RGB(255,255,255));

	int xPos			= 0;
	int yPos			= 0;
	int iTotalWidth		= 0;
	int iTotalHeight	= 0;
	int iMinWidth		= (iCharWidth/3)*2;
	int iMaxWidth		= iMinWidth;
	int iPos = 0;

	// Go through the characters getting their sizes
	for (i=0,s=0; i<iTextLength; i++,s+=_tclen(&lpString[s]) ) 
	{
		RECT rc = { 0, 0, iCharWidth, iCharHeight };

		TCHAR tc[3] = {0,0,0};
		_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );

		if (tc[0] != _T('\n')) 
		{
			GetCharSize(hCharDC,tc,&rc);
			CharacterPositions[i].bottom = rc.bottom;
			OffsetRect(&CharacterPositions[i],0,-rc.top);
			OffsetRect(&CharacterPositions[i],xPos,yPos);
			yPos += rc.bottom - rc.top + iCharSpacingY;
			if (rc.right - rc.left > iMaxWidth)
				iMaxWidth = rc.right - rc.left;
			if (i == iTextLength-1)
				goto COMPLETE;
		}
		else 
		{
COMPLETE:
			if (yPos > iTotalHeight)
				iTotalHeight = yPos - iCharSpacingY;

			xPos = OffsetColumn(CharacterPositions,iPos,iMaxWidth,iCharSpacingX,0);
			iPos = i+1;

			iTotalWidth = xPos;

			iMaxWidth = iMinWidth;
			yPos = 0;
		}
	}

	SpaceColumns(CharacterPositions, iTotalHeight, iTextLength, iCharPlacement);

	// Calc the screen position
	int ScrX = 0;
	int ScrY = 0;
	if (bHCenter)
		ScrX = (lpRect->right/2) - (iTotalWidth/2);
	else if (bRight)
		ScrX = lpRect->right - iTotalWidth;
	if (bVCenter)
		ScrY = ((lpRect->bottom-lpRect->top)/2) - (iTotalHeight/2);
	else if (bBottom)
		ScrY = lpRect->bottom - iTotalHeight;


	if (bCalc) 
	{
		CalcRect(CharacterPositions,lpRect,iTextLength);
	}
	else 
	{
		if (bBufBuf) {
			// Draw to the screen (Double buffer)
			HDC DBuf     = CreateCompatibleDC(hdc);
			HBITMAP BBuf = CreateCompatibleBitmap(hdc,iTotalWidth,iTotalHeight);
			HBITMAP oBuf = (HBITMAP)SelectObject(DBuf,BBuf);
			HFONT oFont	 = (HFONT)SelectObject(DBuf,hFont);

			COLORREF txtCol = GetTextColor(hdc);
			COLORREF bkCol  = GetBkColor(hdc);

			BitBlt(DBuf,0,0,iTotalWidth,iTotalHeight,hdc,ScrX,ScrY,SRCCOPY);

			int bkMode = GetBkMode(hdc);
			if (bkMode != OPAQUE) {
				RECT rc = { 0, 0, iTotalWidth, iTotalHeight };
				FillRect(DBuf, &rc, CreateSolidBrush(bkCol));
			}

			SetBkMode(DBuf,TRANSPARENT);
			SetTextColor(DBuf,txtCol);

			SetBkMode(hdc,TRANSPARENT);
			for (i=0,s=0; i<iTextLength; i++,s+=_tclen(&lpString[s]),iTmpLength++ ) {
				TCHAR tc[3] = {0,0,0};
				_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
				if (tc[0] != _T('\n')) {
					DrawText(DBuf,tc,_tclen(tc),&CharacterPositions[i],DT_CENTER);
				}
			}

			BitBlt(hdc,ScrX,ScrY,iTotalWidth,iTotalHeight,DBuf,0,0,SRCCOPY);

			BBuf = (HBITMAP)SelectObject(DBuf,oBuf);
			DeleteObject(BBuf);
			DeleteObject(oBuf);
			DeleteDC(DBuf);
		}
		else
		{
			OffsetChars(CharacterPositions,iTextLength,ScrX,ScrY);

			COLORREF bkCol  = GetBkColor(hdc);

			int bkMode = GetBkMode(hdc);
			if (bkMode != OPAQUE) {
				RECT rc = { ScrX, ScrY, ScrX+iTotalWidth, ScrY+iTotalHeight };
				FillRect(hdc, &rc, CreateSolidBrush(bkCol));
			}
			SetBkMode(hdc,TRANSPARENT);

			for (i=0,s=0; i<iTextLength; i++,s+=_tclen(&lpString[s]),iTmpLength++ ) {
				TCHAR tc[3] = {0,0,0};
				_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
				if (tc[0] != _T('\n')) {
					DrawText(hdc,tc,_tclen(tc),&CharacterPositions[i],DT_CENTER);
				}
			}

			SetBkMode(hdc,bkMode);
		}
	}

	// Clean up the GDI objects
	SelectObject(hCharDC,hOldCharFont);
	hCharBmp = (HBITMAP)SelectObject(hCharDC,hOldCharBmp);
	DeleteObject(hOldCharBmp);
	DeleteObject(hCharBmp);
	DeleteDC(hCharDC);

	delete[] CharacterPositions;

	return 0;
}

void DoDrawText(HDC hdc,LPCTSTR lpString,RECT *pCalRec,bool bVert)
{
	if(bVert)
	{
		DoDrawVertText(hdc,lpString,pCalRec);
	}
	else
	{
		DoDrawHorText(hdc,lpString,pCalRec);
	}
}

void DoDrawHorText(HDC hdc,LPCTSTR lpString,RECT *pCalRec)
{
	int iTextLength = _tcsclen(lpString);
	if(pCalRec)
	{
		int nCharWidth = 0;
		int nCharHeight = 0;
		int s = 0;
		for (int i = 0; i<iTextLength; i++,s+=_tclen(&lpString[s]) ) 
		{
			RECT rec = {0,0,0,0};
			int iCharHeight = DrawText(hdc,&lpString[s],_tclen(&lpString[s]),&rec,DT_CALCRECT);
			if(nCharHeight < iCharHeight)
				nCharHeight = iCharHeight;
			int iCharWidth  = (rec.right-rec.left);
			nCharWidth += iCharWidth;
		}
		pCalRec->left = 0;
		pCalRec->top = 0;
		pCalRec->right = nCharWidth;
		pCalRec->bottom = nCharHeight;
	}
	else
	{
		int s = 0;
		for (int i = 0; i<iTextLength; i++,s+=_tclen(&lpString[s]) ) 
		{
		}	
		::TextOut(hdc,0,0,lpString,s);
	}
}

void DoDrawVertText( HDC hdc,LPCTSTR lpString,RECT *pCalRec)
{
	RECT rec = {0,0,0,0};
	int iCharHeight = DrawText(hdc,_T("W"),1,&rec,DT_CALCRECT);
	int iCharWidth  = (rec.right-rec.left)*2;
	int iTextLength = _tcsclen(lpString);
	int iTextLines		= 0;
	int iLongestLine	= 0;
	int  iCharPlacement = 0;
	iCharPlacement = TOP;
	RECT *CharacterPositions = new RECT [iTextLength + 1];
	memset(CharacterPositions,0,(iTextLength + 1)*sizeof(RECT));
	int iTmpLength = 0;
	int i=0;
	int s = 0;
	for (; i<iTextLength; i++,s+=_tclen(&lpString[s]),iTmpLength++ ) 
	{
		TCHAR tc[3] = {0,0,0};
		_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
		if (tc[0] == _T('\n') || i == iTextLength-1) 
		{
			if (iTmpLength > iLongestLine)
				iLongestLine = iTmpLength;
			iTextLines++;
			iTmpLength =0;
		}
		else
		{
			SetRect(&CharacterPositions[i],0,0,iCharWidth,iCharHeight);
		}
	}
	SetRect(&CharacterPositions[i-1],0,0,iCharWidth,iCharHeight);

	RECT  rStrRec = { 0, 0, iCharWidth, iCharHeight };
	HFONT hFont  = (HFONT)GetCurrentObject(hdc,OBJ_FONT);

	// Create a canvas for drawing a single characters
	HDC hCharDC			= CreateCompatibleDC(hdc);
	HBITMAP hCharBmp	= CreateBitmap(iCharWidth,iCharHeight,1,1,NULL);
	HBITMAP hOldCharBmp	= (HBITMAP)SelectObject(hCharDC,hCharBmp);
	HFONT hOldCharFont	= (HFONT)SelectObject(hCharDC,hFont);
	FillRect(hCharDC,&rStrRec,(HBRUSH)GetStockObject(WHITE_BRUSH));
	// black text on white background
	SetTextColor(hCharDC,RGB(0,0,0));
	SetBkColor(hCharDC,RGB(255,255,255));

	int xPos			= 0;
	int yPos			= 0;
	int iTotalWidth		= 0;
	int iTotalHeight	= 0;
	int iMinWidth		= (iCharWidth/3)*2;
	int iMaxWidth		= iMinWidth;
	int iPos = 0;
	for (i=0,s=0; i<iTextLength; i++,s+=_tclen(&lpString[s]) ) 
	{
		RECT rc = { 0, 0, iCharWidth, iCharHeight };
		TCHAR tc[3] = {0,0,0};
		_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
		if (tc[0] != _T('\n')) 
		{
			GetCharSize(hCharDC,tc,&rc);
			CharacterPositions[i].bottom = rc.bottom;
			OffsetRect(&CharacterPositions[i],0,-rc.top);
			OffsetRect(&CharacterPositions[i],xPos,yPos);
			yPos += rc.bottom - rc.top;
			if (rc.right - rc.left > iMaxWidth)
				iMaxWidth = rc.right - rc.left;
			if (i == iTextLength-1)
				goto COMPLETE;
		}
		else 
		{
COMPLETE:
			if (yPos > iTotalHeight)
				iTotalHeight = yPos;
			xPos = OffsetColumn(CharacterPositions,iPos,iMaxWidth,0,0);
			iPos = i+1;

			iTotalWidth = xPos;

			iMaxWidth = iMinWidth;
			yPos = 0;
		}
	}
	if(pCalRec)
	{//¼ÆËãÎÄ×Ö°üÎ§¿ò
		pCalRec->left = 0;
		pCalRec->top = 0;
		pCalRec->right = iTotalWidth;
		pCalRec->bottom = iTotalHeight;
	}
	else
	{
		SpaceColumns(CharacterPositions, iTotalHeight, iTextLength, iCharPlacement);
		COLORREF bkCol  = GetBkColor(hdc);
		int bkMode = GetBkMode(hdc);
		if (bkMode != OPAQUE)
		{
			RECT rc = { 0, 0, iTotalWidth, iTotalHeight };
			FillRect(hdc, &rc, CreateSolidBrush(bkCol));
		}
		SetBkMode(hdc,TRANSPARENT);
		for (i=0,s=0; i<iTextLength; i++,s+=_tclen(&lpString[s]),iTmpLength++ ) 
		{
			TCHAR tc[3] = {0,0,0};
			_tcsncpy(tc,&lpString[s],_tclen(&lpString[s]) );
			if (tc[0] != _T('\n')) {
				DrawText(hdc,tc,_tclen(tc),&CharacterPositions[i],DT_CENTER);
			}
		}
		SetBkMode(hdc,bkMode);
	}
	// Clean up the GDI objects
	SelectObject(hCharDC,hOldCharFont);
	hCharBmp = (HBITMAP)SelectObject(hCharDC,hOldCharBmp);
	DeleteObject(hOldCharBmp);
	DeleteObject(hCharBmp);
	DeleteDC(hCharDC);
	delete[] CharacterPositions;
}