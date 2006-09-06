#ifndef AFHACKS_H_
#define AFHACKS_H_

typedef struct tagPALETTEENTRY { 
 	BYTE peRed; 
 	BYTE peGreen; 
 	BYTE peBlue; 
 	BYTE peFlags; 
} PALETTEENTRY; 

typedef struct tagLOGPALETTE { 
 	WORD         palVersion; 
 	WORD         palNumEntries; 
 	PALETTEENTRY palPalEntry[1]; 
} LOGPALETTE;

int GetDeviceCaps(HDC hdc, int nIndex);
HPALETTE CreatePalette(LOGPALETTE *lplgpl);
UINT RealizePalette(HDC hdc);
HPALETTE SelectPalette(HDC hdc, HPALETTE hpal, BOOL bForceBackground);
BOOL DeleteObject(HGDIOBJ hObject);

UINT SysStringLen(BSTR bstr);
int _strnicmp(const char *string1, const char *string2, size_t count);
int _stricmp(const char *string1, const char *string2);

class AfApp
{
public:
	AfApp();
	~AfApp();
	static AfApp * Papp();
	static bool LaunchHL(HWND hwnd, LPCTSTR pszOperation, LPCTSTR pszFile,
		LPCTSTR pszParameters, LPCTSTR pszDirectory, int nShowCmd);
};

class AfDeFeInt
{
public:
	AfDeFeInt();
	~AfDeFeInt();
	virtual bool ValidKeyUp(UINT wp);
};

class AfGfx
{
public:
	AfGfx();
	~AfGfx();
	static void FillSolidRect(HDC hdc, const Rect &rc, COLORREF clr, bool fUsePalette = true);
	static COLORREF SetBkColor(HDC hdc, COLORREF clr);
	static COLORREF SetTextColor(HDC hdc, COLORREF clr);
};

class AfGdi
{
public:
	typedef enum
	{
		UNDEF=0, NEW=1, OLD=2, CLUDGE_OLD=3,
	} SelType;
	AfGdi();
	~AfGdi();
	static HDC CreateCompatibleDC(HDC hdc);
	static BOOL	DeleteDC(HDC hdc);
	static HDC GetDC(HWND hWnd);
	static int ReleaseDC(
		HWND hWnd, // handle to window
		HDC hdc);  // handle to DC
	static HFONT CreateFontIndirect(const LOGFONT * plf);
	static HFONT SelectObjectFont(HDC hdc, HGDIOBJ font, SelType fNew=NEW);
	static BOOL DeleteObjectFont(HGDIOBJ font);
	static HBITMAP CreateCompatibleBitmap(HDC hdc, int width, int height);
	static HBITMAP SelectObjectBitmap(HDC hdc, HGDIOBJ bitmap, SelType fNew=NEW);
	static BOOL DeleteObjectBitmap(HGDIOBJ bitmap);
	static HBRUSH CreateSolidBrush(COLORREF crColor);
};

namespace AfUtil
{
	bool GetResourceTss(int rid, int wsUser, ITsString ** pptss);
}

#endif /*AFHACKS_H_*/
