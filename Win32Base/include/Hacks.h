/*
 *	$Id$
 *
 *	Definitions needed to compile and run Win32 code on other platforms
 *
 *	Neil Mayhew - 28 Apr 2004
 */

#ifndef HACKS_H
#define HACKS_H

#include "Types.h"
#include "COM.h"
#include "COMClasses.h"

#include "WinError.h"
#include "OleDbErr.h"
#include "OleCtl.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <alloca.h>
#include <algorithm>

enum { CP_ACP, CP_UTF8 };

int WideCharToMultiByte(int, int, const OLECHAR*, int, char*, int, char*, char*);
int MultiByteToWideChar(int, int, const char*, int, OLECHAR*, int);

bool IsBadStringPtrW(const OLECHAR*, unsigned long);
bool IsBadStringPtrA(const char*, unsigned long);
bool IsBadReadPtr(const void*, unsigned long);
bool IsBadWritePtr(const void*, unsigned long);

short ClientToScreen(HWND, POINT*);
short ScreenToClient(HWND, POINT*);

struct DVTARGETDEVICE;

struct FORMATETC
{
    CLIPFORMAT cfFormat;
    DVTARGETDEVICE *ptd;
    DWORD dwAspect;
    LONG lindex;
    DWORD tymed;
};

enum DVASPECT
{
	DVASPECT_CONTENT	= 1,
	DVASPECT_THUMBNAIL	= 2,
	DVASPECT_ICON		= 4,
	DVASPECT_DOCPRINT	= 8
};

// Clipboard Formats

enum
{
	CF_TEXT = 1,
	CF_BITMAP,
	CF_METAFILEPICT,
	CF_SYLK,
	CF_DIF,
	CF_TIFF,
	CF_OEMTEXT,
	CF_DIB,
	CF_PALETTE,
	CF_PENDATA,
	CF_RIFF,
	CF_WAVE,
	CF_UNICODETEXT,
	CF_ENHMETAFILE,
	CF_HDROP,
	CF_LOCALE,
	CF_DIBV5,
	CF_MAX
};

enum
{
	TYMED_HGLOBAL	= 1,
	TYMED_FILE		= 2,
	TYMED_ISTREAM	= 4,
	TYMED_ISTORAGE	= 8,
	TYMED_GDI		= 16,
	TYMED_MFPICT	= 32,
	TYMED_ENHMF		= 64,
	TYMED_NULL		= 0
};

enum				// File positions
{
	FILE_BEGIN,
	FILE_CURRENT,
	FILE_END,
};

DWORD SetFilePointer(HANDLE, LONG, PLONG, DWORD);

typedef struct tagCRITICAL_SECTION* CRITICAL_SECTION;

void InitializeCriticalSection(CRITICAL_SECTION*);
void DeleteCriticalSection(CRITICAL_SECTION*);
void EnterCriticalSection(CRITICAL_SECTION*);
void LeaveCriticalSection(CRITICAL_SECTION*);

template<typename T>
T InterlockedIncrement(T* p)
{
	return ++*p;
}
template<typename T>
T InterlockedDecrement(T* p)
{
	return --*p;
}

//long InterlockedIncrement(long*);
//long InterlockedDecrement(long*);

struct EXCEPTION_POINTERS;
struct CONTEXT;

int GetFullPathName(const char*, int, char*, const char**);
int GetFullPathName(const OLECHAR*, int, OLECHAR*, const OLECHAR**);

#define __RPC_USER
#define __RPC_STUB
#define __RPC_FAR
typedef int PRPC_MESSAGE;
typedef void* RPC_IF_HANDLE;
typedef unsigned char boolean;

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

bool SetRect(RECT*, int, int, int, int);
bool OffsetRect(RECT*, int,int);

/*typedef struct
{
	int		cbSize;
	LPCTSTR	lpszDocName;
	LPCTSTR lpszOutput;
	LPCTSTR lpszDatatype;
	DWORD	fwType;
}DOCINFO;
*/
enum
{
	LOGPIXELSY,
};
enum
{
	DEFAULT_CHARSET,
};
enum
{
	OUT_TT_ONLY_PRECIS,
};
enum
{
	CLIP_DEFAULT_PRECIS,
};
enum
{
	DRAFT_QUALITY,
};
typedef struct tagLOGFONT
{
	LONG	lfHeight;
	LONG	lfWidth;
	LONG	lfEscapement;
	LONG	lfOrientation;
	LONG	lfWeight;
	BYTE	lfItalic;
	BYTE	lfUnderline;
	BYTE	lfStrikeOut;
	BYTE	lfCharSet;
	BYTE	lfOutPrecision;
	BYTE	lfClipPrecision;
	BYTE	lfQuality;
	BYTE	lfPitchAndFamily;
	WCHAR	lfFaceName[];
}LOGFONT, *PLOGFONT;

typedef struct tagTEXTMETRIC
{ 
  LONG tmHeight; 
  LONG tmAscent; 
  LONG tmDescent; 
  LONG tmInternalLeading; 
  LONG tmExternalLeading; 
  LONG tmAveCharWidth; 
  LONG tmMaxCharWidth; 
  LONG tmWeight; 
  LONG tmOverhang; 
  LONG tmDigitizedAspectX; 
  LONG tmDigitizedAspectY; 
  TCHAR tmFirstChar; 
  TCHAR tmLastChar; 
  TCHAR tmDefaultChar; 
  TCHAR tmBreakChar; 
  BYTE tmItalic; 
  BYTE tmUnderlined; 
  BYTE tmStruckOut; 
  BYTE tmPitchAndFamily; 
  BYTE tmCharSet; 
} TEXTMETRIC, *PTEXTMETRIC;
bool GetTextMetrics(HDC, PTEXTMETRIC);

typedef enum nIndex
{
	COLOR_3DDKSHADOW	= 21,
	COLOR_3DFACE		= 15,
	COLOR_3DHILIGHT		= 20,
	COLOR_3DLIGHT		= 22,
	COLOR_3DSHADOW		= 16,
	COLOR_ACTIVEBORDER	= 10,
	COLOR_ACTIVECAPTION	= 2,
	COLOR_APPWORKSPACE	= 12,
	COLOR_BACKGROUND	= 1,
	COLOR_BTNFACE		= 15,
	COLOR_BTNHIGHLIGHT	= 20,
	COLOR_BTNHILIGHT	= 20,
	COLOR_BTNSHADOW		= 16,
	COLOR_BTNTEXT		= 18,
	COLOR_CAPTIONTEXT	= 9,
	COLOR_DESKTOP		= 1,
	COLOR_GRADIENTACTIVECAPTION = 27,
	COLOR_GRAYTEXT		= 17,
	COLOR_HIGHLIGHT		= 13,
	COLOR_HIGHLIGHTTEXT	= 14,
	COLOR_HOTLIGHT		= 26,
	COLOR_INACTIVEBORDER = 11,
	COLOR_INACTIVECAPTION = 3,
	COLOR_INACTIVECAPTIONTEXT = 19,
	COLOR_INFOBK		= 24,
	COLOR_INFOTEXT		= 23,
	COLOR_MENU			= 4,
	COLOR_MENUHILIGHT	= 29,
	COLOR_MENUBAR		= 30,
	COLOR_MENUTEXT		= 7,
	COLOR_SCROLLBAR		= 0,
	COLOR_WINDOW		= 5,
	COLOR_WINDOWFRAME	= 6,
	COLOR_WINDOWTEXT	= 8,
	RC_PALETTE,
	RASTERCAPS,
	SM_CMONITORS,	
};
DWORD GetSysColor(int);
int GetDeviceCaps(HDC, int);

typedef struct
{
	HWND	hwnd;
	UINT	message;
	WPARAM	wParam;
	LPARAM	lParam;
	DWORD	time;
	POINT	pt;
}MSG, *PMSG;

enum
{
	PM_NOREMOVE,
	PM_REMOVE,
	PM_NOYIELD,
	PM_QS_INPUT,
	PM_QS_PAINT,
	PM_QS_POSTMESSAGE,
	PM_QS_SENDMESSAGE,
};	
bool PeekMessage(PMSG, HWND, UINT,	UINT, UINT);

enum
{
	MB_ICONASTERISK		= 0x00000040L,
	MB_ICONEXCLAMATION	= 0x00000030L,
	MB_ICONHAND			= 0x00000010L,
	MB_ICONQUESTION		= 0x00000020L,
	MB_OK				= 0x00000000L,
	MB_YESNO			= ~0,
	MB_DEFBUTTON2		= ~0,
	MB_TASKMODAL		= ~0,
};
const int IDYES = ~0;

bool MessageBeep(unsigned int);
int MessageBox(HWND, LPCTSTR, LPCTSTR, UINT);
int MessageBoxA(HWND, LPCTSTR, LPCTSTR, UINT);
bool TranslateMessage(const MSG*);
LRESULT DispatchMessage(const MSG*);

enum
{
	SP_ERROR,
};
//int StartPage(HDC);
//int EndPage(HDC);
//int StartDoc(HDC, const DOCINFO*);
//int EndDoc(HDC);
//int AbortDoc(HDC);

enum
{ 	
	VK_PRIOR,
	VK_NEXT,
	VK_END,
	VK_HOME,
	VK_LEFT,
	VK_UP,
	VK_RIGHT,
	VK_DOWN,
	VK_INSERT,
	VK_DELETE = 46,
	VK_TAB,
	VK_RETURN,
};		
HKL GetKeyboardLayout(DWORD);
WORD LANGIDFROMLCID(LCID);

typedef enum dwStyle
{
	CFS_DEFAULT,
	CFS_FORCE_POSITION,
	CFS_POINT,
	CFS_RECT,
};
typedef struct tagCOMPOSITIONFORM
{
  DWORD  dwStyle;       
  POINT  ptCurrentPos;  
  RECT   rcArea;        
} COMPOSITIONFORM, *PCOMPOSITIONFORM;
bool ImmIsIME(HKL);
bool ImmGetCompositionWindow(HIMC, PCOMPOSITIONFORM);
HIMC ImmGetContext(HWND);
bool ImmSetCompositionWindow(HIMC, PCOMPOSITIONFORM);

typedef enum dwRop
{
	BLACKNESS,
	CAPTUREBLT,
	DSTINVERT,
	MERGECOPY,
	MERGEPAINT,
	NOMIRRORBITMAP,
	NOTSRCCOPY,
	NOTSRCERASE,
	PATCOPY,
	PATINVERT,
	PATPAINT,
	SRCAND,
	SRCCOPY,
	SRCERASE,
	SRCINVERT,
	SRCPAINT,
	WHITENESS,
};
bool BitBlt(HDC, int, int, int, int, HDC, int, int, DWORD);
HWND WindowFromDC(HDC);

int GetModuleFileName(HMODULE, OLECHAR* buf, int length);

enum
{
	DLL_PROCESS_ATTACH,
	DLL_PROCESS_DETACH,
	DLL_THREAD_ATTACH,
	DLL_THREAD_DETACH,
};

int _set_sbh_threshold(int);

enum
{
	_CRTDBG_REPORT_FLAG,
	_CRTDBG_LEAK_CHECK_DF,
};

int _CrtSetDbgFlag(int);

const int MAX_PATH = 260;

#define _alloca alloca

char*    _itoa_s(int value, char*    buffer, size_t bufferSize, int radix);
wchar_t* _itow_s(int value, wchar_t* buffer, size_t bufferSize, int radix);
OLECHAR* _itow_s(int value, OLECHAR* buffer, size_t bufferSize, int radix);

#define	_itoa(V, B, R) _itoa_s((V), (B), ~0, (R))	// Unchecked version, don't use in new code
#define	_itow(V, B, R) _itow_s((V), (B), ~0, (R))	// Unchecked version, don't use in new code

void OutputDebugString(const char* str);
void OutputDebugString(const wchar_t* str);
void OutputDebugString(const OLECHAR* str);

inline void OutputDebugStringA(const char* str)
	{ OutputDebugString(str); }
inline void OutputDebugStringW(const wchar_t* str)
	{ OutputDebugString(str); }
inline void OutputDebugStringW(const OLECHAR* str)
	{ OutputDebugString(str); }

inline void OutputDebugStr(const char* str)
	{ OutputDebugString(str); }
inline void OutputDebugStr(const wchar_t* str)
	{ OutputDebugString(str); }
inline void OutputDebugStr(const OLECHAR* str)
	{ OutputDebugString(str); }

// FieldWorks-specific

#define NO_ASM 1

#endif //HACKS_H
