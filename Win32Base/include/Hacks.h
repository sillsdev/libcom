/*
 *	$Id$
 *
 *	Definitions needed to compile and run Win32 code on other platforms
 *
 *	Neil Mayhew - 28 Apr 2004
 */

#ifndef HACKS_H
#define HACKS_H

#include "BasicTypes.h"
#include "ExtendedTypes.h"
#include "DateTime.h"
#include "WinError.h"
#include "winuser.h"
#include <errno.h>

#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <alloca.h>
#include <algorithm>

#include <unicode/ustring.h>

enum { CP_ACP, CP_UTF8 };

int WideCharToMultiByte(int, int, const OLECHAR*, int, char*, int, char*, char*);
int MultiByteToWideChar(int, int, const char*, int, OLECHAR*, int);

bool IsBadStringPtrW(const OLECHAR*, unsigned long);
bool IsBadStringPtrA(const char*, unsigned long);
bool IsBadReadPtr(const void*, unsigned long);
bool IsBadWritePtr(const void*, unsigned long);
bool IsBadCodePtr(const void*);

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
	return __sync_add_and_fetch(p, 1);
}
template<typename T>
T InterlockedDecrement(T* p)
{
	return __sync_sub_and_fetch(p, 1);
}

//long InterlockedIncrement(long*);
//long InterlockedDecrement(long*);

struct EXCEPTION_POINTERS;
struct CONTEXT;

#define MAX_PATH 260
#define MAX_COMPUTERNAME_LENGTH 15

int GetFullPathName(const char*, int, char*, const char**);
int GetFullPathName(const OLECHAR*, int, OLECHAR*, const OLECHAR**);

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

enum nIndex
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
#ifdef WIN32_KEY_VALUES
	VK_PRIOR = 33,
	VK_NEXT = 34,
	VK_END = 35,
	VK_HOME = 36,
	VK_LEFT = 37,
	VK_UP = 38,
	VK_RIGHT = 39,
	VK_DOWN = 40,
	VK_INSERT = 45,
	VK_DELETE = 46,
	VK_TAB = 9,
	VK_RETURN = 13,
	VK_F1 = 0x70,
	VK_F2 = 0x71,
	VK_F3 = 0x72,
	VK_F4 = 0x73,
	VK_F5 = 0x74,
	VK_F6 = 0x75,
	VK_F7 = 0x76,
	VK_F8 = 0x77, 
#else
	VK_PRIOR = 0,
	VK_NEXT = 1,
	VK_END = 2,
	VK_HOME = 3,
	VK_LEFT = 4,
	VK_UP = 5,
	VK_RIGHT = 6,
	VK_DOWN = 7,
	VK_INSERT = 8,
	VK_DELETE = 46,
	VK_TAB = 47,
	VK_RETURN = 48,
	VK_F1 = 112,
	VK_F2 = 113,
	VK_F3 = 114,
	VK_F4 = 115,
	VK_F5 = 116,
	VK_F6 = 117,
	VK_F7 = 118,
	VK_F8 = 119,
#endif
};		
HKL GetKeyboardLayout(DWORD);
#define LANGIDFROMLCID(localeIdentifier) ((WORD)localeIdentifier)

enum dwStyle
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

enum dwRop
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

DWORD GetModuleFileName(HMODULE, OLECHAR* buf, DWORD length);
DWORD GetModuleFileName(HMODULE, TCHAR*   buf, DWORD length);

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

enum
{
	_CRT_WARN = 0,
	_CRT_ERROR = 1,
	_CRT_ASSERT = 2,
};

int _CrtSetDbgFlag(int);

#define _alloca alloca

char*    _itoa_s(int value, char*    buffer, size_t sizeInCharacters, int radix);
wchar_t* _itow_s(int value, wchar_t* buffer, size_t sizeInCharacters, int radix);
OLECHAR* _itow_s(int value, OLECHAR* buffer, size_t sizeInCharacters, int radix);

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

#define UNREFERENCED_PARAMETER(P)          (P)

// FieldWorks-specific

#ifndef NO_ASM
#define NO_ASM 1
#endif


// TODO-P4CL23677-Merge
// These functions defines don't account for specified length
// TODO Replace these macros with multiple functions
#define strcpy_s(DST, NUM, SRC) strcpy(DST, SRC)
#define strncpy_s(DST, NUM, SRC, CNT) strncpy(DST, SRC, CNT)
#define strcat_s(DST, NUM, SRC) strcat(DST, SRC)
#define strncat_s(DST, NUM, SRC, CNT) strncat(DST, SRC, CNT)

#define wcscmp(LEFT, RIGHT) u_strcmp(LEFT, RIGHT)
#define wcsncmp(LEFT, RIGHT, NUM) u_strncmp(LEFT, RIGHT, NUM)
#define wcsncmp_s(LEFT, RIGHT, NUM) u_strncmp(LEFT, RIGHT, NUM)
#define _wcsicmp(LEFT, RIGHT) u_strcasecmp(LEFT, RIGHT, 0)

#define fopen_s(FH, FILE, MODE) (*FH = fopen(FILE, MODE)) == NULL

#define _TRUNCATE	-1

int wcslen(const OLECHAR *str);
int wcslen_s(const OLECHAR *str, const int size);

int wcscat_s(OLECHAR *dst, const int size, const OLECHAR *src);

OLECHAR * wcscpy(OLECHAR *dst, const OLECHAR *src);
int wcscpy_s(OLECHAR *dst, const int size, const OLECHAR *src);


OLECHAR * wcsncpy(OLECHAR* dst, const OLECHAR* src, const int size);
int wcsncpy_s(OLECHAR* dst, const size_t dsize, const OLECHAR* src, const size_t size);

int _stricmp(const char *string1, const char *string2);
int _strnicmp(const char *string1, const char *string2, size_t count);

// Not done as a macro because use of elipsis..
int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...);
int _snprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, ...);

#endif //HACKS_H
