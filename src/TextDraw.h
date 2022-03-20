// cVert.h: interface for the cVert class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CVERT_H__84327F0D_9298_42B2_92E1_F8FDA858109F__INCLUDED_)
#define AFX_CVERT_H__84327F0D_9298_42B2_92E1_F8FDA858109F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Position flags
#define DV_HCENTER		0x0001
#define DV_RIGHT		0x0002
#define DV_VCENTER		0x0004
#define DV_BOTTOM		0x0008
#define DV_CENTERALL	0x0005
#define DV_CALCRECT		0x0010
#define DV_DOUBLEBUFFER 0x0020

// Alignment flags
#define DV_TEXTTOP		0x0100
#define DV_TEXTCENTER	0x0200
#define DV_TEXTBOTTOM	0x0400
#define	DV_TEXTJUSTIFY	0x0800

int DrawVertText(HDC hdc,LPCTSTR lpString,LPRECT lpRect
				 ,UINT uFormat,
				 UINT HcharSpacing = 0, 
				 UINT VcharSpacing = 0);
void DoDrawText(HDC hdc,LPCTSTR lpString,RECT *pCalRec = NULL,bool bVert=false);

void DoDrawVertText(HDC hdc,LPCTSTR lpString,RECT *pCalRec = NULL);
void DoDrawHorText(HDC hdc,LPCTSTR lpString,RECT *pCalRec = NULL);

#endif // !defined(AFX_CVERT_H__84327F0D_9298_42B2_92E1_F8FDA858109F__INCLUDED_)
