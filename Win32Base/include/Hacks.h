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

COLORREF RGB(BYTE, BYTE, BYTE);
typedef struct tagRGBQUAD {
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
}RGBQUAD;

typedef struct
{
	HWND	hwnd;
	UINT	message;
	WPARAM	wParam;
	LPARAM	lParam;
	DWORD	time;
	POINT	pt;
}MSG, *PMSG, *LPMSG;

bool PeekMessage(LPMSG, HWND, UINT,	UINT, UINT);
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
bool MessageBeep(unsigned int);
enum
{
	MB_ICONASTERISK		= 0x00000040L,
	MB_ICONEXCLAMATION	= 0x00000030L,
	MB_ICONHAND			= 0x00000010L,
	MB_ICONQUESTION		= 0x00000020L,
	MB_OK				= 0x00000000L,
};
bool TranslateMessage(const MSG*);
LRESULT DispatchMessage(const MSG*);

int StartPage(HDC);
int EndPage(HDC);
int StartDoc(HDC, const DOCINFO*);
int EndDoc(HDC);
enum
{
	SP_ERROR,
};
int AbortDoc(HDC);

#define _alloca alloca
// FieldWorks-specific

#define NO_ASM 1

#endif //HACKS_H
