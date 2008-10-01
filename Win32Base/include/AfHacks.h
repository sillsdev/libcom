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

inline int _stricmp(const char *string1, const char *string2)
{
	return strcasecmp(string1, string2);
}
inline int _strnicmp(const char *string1, const char *string2, size_t count)
{
	return strncasecmp(string1, string2, count);
}

class AfApp
{
public:
	AfApp();
	~AfApp();
	AfApp * Papp();
	bool LaunchHL(HWND hwnd, LPCTSTR pszOperation, LPCTSTR pszFile,
		LPCTSTR pszParameters, LPCTSTR pszDirectory, int nShowCmd);
};

class AfDeFeInt
{
public:
	AfDeFeInt();
	~AfDeFeInt();
	virtual bool ValidKeyUp(UINT wp);
};

COLORREF SetBkColor(HDC hdc, COLORREF clr);
COLORREF SetTextColor(HDC hdc, COLORREF clr);

typedef struct tagLOGPEN { 
  UINT     lopnStyle; 
  POINT    lopnWidth; 
  COLORREF lopnColor; 
} LOGPEN, *PLOGPEN;

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

typedef struct tagRGBQUAD {
  BYTE    rgbBlue; 
  BYTE    rgbGreen; 
  BYTE    rgbRed; 
  BYTE    rgbReserved; 
} RGBQUAD;

typedef struct tagBITMAPINFO { 
  BITMAPINFOHEADER bmiHeader; 
  RGBQUAD          bmiColors[1]; 
} BITMAPINFO, *PBITMAPINFO;

typedef struct tagLOGBRUSH { 
  UINT     lbStyle; 
  COLORREF lbColor; 
  LONG     lbHatch; 
} LOGBRUSH, *PLOGBRUSH;

enum fnObject
{
	DEFAULT_GUI_FONT,
};
 
const int CCHDEVICENAME = 0;
const int CCHFORMNAME = 0;
const int ERROR = 0;
const int HGDI_ERROR = 0;

typedef struct _devicemode { 
  BCHAR  dmDeviceName[CCHDEVICENAME]; 
  WORD   dmSpecVersion; 
  WORD   dmDriverVersion; 
  WORD   dmSize; 
  WORD   dmDriverExtra; 
  DWORD  dmFields; 
  union {
    struct {
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
      short dmScale; 
      short dmCopies; 
      short dmDefaultSource; 
      short dmPrintQuality; 
    };
    POINT dmPosition;
    DWORD  dmDisplayOrientation;
    DWORD  dmDisplayFixedOutput;
  };

  short  dmColor; 
  short  dmDuplex; 
  short  dmYResolution; 
  short  dmTTOption; 
  short  dmCollate; 
  BYTE  dmFormName[CCHFORMNAME]; 
  WORD  dmLogPixels; 
  DWORD  dmBitsPerPel; 
  DWORD  dmPelsWidth; 
  DWORD  dmPelsHeight; 
  union {
    DWORD  dmDisplayFlags; 
    DWORD  dmNup;
  };
  DWORD  dmDisplayFrequency; 
#if(WINVER >= 0x0400) 
  DWORD  dmICMMethod;
  DWORD  dmICMIntent;
  DWORD  dmMediaType;
  DWORD  dmDitherType;
  DWORD  dmReserved1;
  DWORD  dmReserved2;
#if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
  DWORD  dmPanningWidth;
  DWORD  dmPanningHeight;
#endif
#endif /* WINVER >= 0x0400 */
} DEVMODE;

HDC 	CreateCompatibleDC(HDC hdc);
BOOL	DeleteDC(HDC hdc);
HDC		GetDC(HWND hWnd);
int		ReleaseDC(HWND hWnd, HDC hdc);
HFONT	CreateFontIndirect(CONST LOGFONT * plf);
HBITMAP	CreateCompatibleBitmap(HDC hdc, int width, int height);
HBRUSH	CreateSolidBrush(COLORREF crColor);

LPTSTR	MAKEINTRESOURCE(WORD wInteger);

BOOL	InvertRect(HDC hDC, CONST RECT *lprc);
BOOL	RestoreDC(HDC hdc, int nSavedDC);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj);
HPEN	CreatePen(int fnPenStyle, int nWidth, COLORREF crColor);
HPEN	CreatePenIndirect(CONST LOGPEN *lplgpn);
HRGN	CreateRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
int		GetClipRgn(HDC hdc, HRGN hrgn);
int		SelectClipRgn(HDC hdc, HRGN hrgn);

const LPCTSTR RT_BITMAP = NULL;

LPVOID	LockResource(HGLOBAL hResData);
HRSRC	FindResource(HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType);
HGLOBAL LoadResource(HMODULE hModule, HRSRC hResInfo);

enum iUsage {
	DIB_PAL_COLORS,
	DIB_RGB_COLORS,
};

int 	StretchDIBits(HDC hdc, int XDest, int YDest, int nDestWidth, int nDestHeight,
						int XSrc, int YSrc, int nSrcWidth, int nSrcHeight,
						CONST VOID *lpBits, CONST BITMAPINFO *lpBitsInfo,
						UINT iUsage, DWORD dwRop);

enum fuOptions {
	ETO_OPAQUE,
};

BOOL 	ExtTextOut(HDC hdc, int X, int Y, UINT fuOptions, CONST RECT* lprc,
					LPCTSTR lpString, UINT cbCount, CONST INT* lpDx);
int 	GetSystemMetrics(int nIndex);
BOOL 	SystemParametersInfo(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni);
HDC 	CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, CONST DEVMODE* lpInitData);
HFONT	CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
			DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
			DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality, DWORD fdwPitchAndFamily,
			LPCTSTR lpszFace);
HGDIOBJ	GetStockObject(int fnObject);
HBITMAP	CreateBitmap(int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, CONST VOID* lpvBits);
HANDLE	LoadImage(HINSTANCE hinst, LPCTSTR lpszName, UINT uType, int cxDesired, int cyDesired, UINT fuLoad);
HBRUSH	CreateBrushIndirect(CONST LOGBRUSH *lplb);
HBRUSH	CreatePatternBrush(HBITMAP hbmp);
HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName);
HIMAGELIST ImageList_Create(int cx, int cy, UINT flags, int cInitial, int cGrow);
HIMAGELIST ImageList_LoadImage(HINSTANCE hi, LPCTSTR lpbmp, int cx, int cGrow, COLORREF crMask,
							UINT uType, UINT uFlags);
BOOL ImageList_Destroy(HIMAGELIST himl);
HIMAGELIST TreeView_CreateDragImage(HWND hwndTV, HTREEITEM hitem);

typedef struct tagMONITORINFO {
DWORD cbSize;
RECT rcMonitor;
RECT rcWork;
DWORD dwFlags;
} MONITORINFO, *LPMONITORINFO;

const DWORD MONITOR_DEFAULTTONULL = 0;

BOOL	GetMonitorInfo(HMONITOR hMonitor, LPMONITORINFO lpmi);
HMONITOR MonitorFromPoint(POINT pt, DWORD dwFlags);

enum uiAction {
	SPI_GETWORKAREA,
};

namespace AfUtil
{
	bool GetResourceTss(int rid, int wsUser, class ITsString ** pptss);
}

#endif /*AFHACKS_H_*/
