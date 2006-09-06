/*
 *	$Id$
 *
 *	Main type names used in Win32 code
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <unicode/utypes.h>

// Compiler features

#define __cdecl //__attribute__((cdecl))
#define __stdcall //__attribute__((stdcall))
#define __declspec(X)		// No easy way to emulate this

// Has to be #define, not typedef, because of "unsigned __int64"
#define __int64 long long

// Win32SDK types and defines

#define FAR
#define NEAR
#define PASCAL
#define CONST const

#define CDECL __cdecl

typedef          void      VOID;

typedef          char      CHAR;
typedef unsigned char      UCHAR;
typedef          short     SHORT;
typedef unsigned short     USHORT;
typedef          int       INT;
typedef unsigned int       UINT;
typedef          long      LONG;
typedef unsigned long      ULONG;
typedef          long long LONGLONG;
typedef unsigned long long ULONGLONG;

typedef          float     FLOAT;
typedef          double    DOUBLE;

typedef USHORT	WORD;
typedef ULONG	DWORD;
typedef UCHAR	BYTE;

typedef LONG		BOOL;

#define __DECLARE_POINTER_TYPES(TYPE) \
	typedef       TYPE NEAR *P##TYPE; \
	typedef       TYPE FAR  *LP##TYPE; \
	typedef CONST TYPE NEAR *PC##TYPE; \
	typedef CONST TYPE FAR	*LPC##TYPE;

__DECLARE_POINTER_TYPES(VOID)
__DECLARE_POINTER_TYPES(CHAR)
__DECLARE_POINTER_TYPES(UCHAR)
__DECLARE_POINTER_TYPES(SHORT)
__DECLARE_POINTER_TYPES(USHORT)
__DECLARE_POINTER_TYPES(INT)
__DECLARE_POINTER_TYPES(UINT)
__DECLARE_POINTER_TYPES(LONG)
__DECLARE_POINTER_TYPES(ULONG)
__DECLARE_POINTER_TYPES(LONGLONG)
__DECLARE_POINTER_TYPES(ULONGLONG)

__DECLARE_POINTER_TYPES(FLOAT)
__DECLARE_POINTER_TYPES(DOUBLE)

__DECLARE_POINTER_TYPES(WORD)
__DECLARE_POINTER_TYPES(DWORD)
__DECLARE_POINTER_TYPES(BYTE)

__DECLARE_POINTER_TYPES(BOOL)

// Wide characters - 16-bit

typedef UChar				OLECHAR;
typedef UChar				WCHAR;
typedef wchar_t				TCHAR; // Assume we always want Unicode

typedef OLECHAR*			BSTR;

#include <cctype>	// So _T won't get defined again if ctype is included later
#undef  _T			// In <ctype.h>
#define _T(X) L##X

#define __DECLARE_STRING_TYPES(TYPE, NAME) \
	typedef       TYPE NEAR *P##NAME; \
	typedef       TYPE FAR  *LP##NAME; \
	typedef CONST TYPE NEAR *PC##NAME; \
	typedef CONST TYPE FAR	*LPC##NAME;

__DECLARE_POINTER_TYPES(OLECHAR)
__DECLARE_POINTER_TYPES(CHAR)
__DECLARE_POINTER_TYPES(WCHAR)
__DECLARE_POINTER_TYPES(TCHAR)
__DECLARE_STRING_TYPES(OLECHAR, OLESTR)
__DECLARE_STRING_TYPES(CHAR, STR)
__DECLARE_STRING_TYPES(WCHAR, WSTR)
__DECLARE_STRING_TYPES(TCHAR, TSTR)

typedef DWORD				WPARAM;
typedef LONG*				LPARAM;
typedef LONG*				LRESULT;

union LARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	};
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
};

union ULARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	};
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
};

__DECLARE_POINTER_TYPES(LARGE_INTEGER)
__DECLARE_POINTER_TYPES(ULARGE_INTEGER)

union CY
{
    struct
	{
        unsigned long   Lo;
        long			Hi;
    };
    LONGLONG int64;
};

__DECLARE_POINTER_TYPES(CY)

struct FIXED
{
	short          value;
	unsigned short fract;
};

__DECLARE_POINTER_TYPES(FIXED)

struct POINT
{
	long x, y;
};

__DECLARE_POINTER_TYPES(POINT)

struct RECT
{
	long left, right, top, bottom;
};

__DECLARE_POINTER_TYPES(RECT)


typedef long SCODE;
typedef long HRESULT;

typedef void*		HANDLE;
typedef HANDLE		HGLOBAL;

typedef HGLOBAL		HINSTANCE;
typedef HINSTANCE   HMODULE;
typedef HGLOBAL		HFILE;
typedef HGLOBAL		HBITMAP;
typedef HGLOBAL		HMETAFILEPICT;
typedef HGLOBAL		HENHMETAFILE;
typedef HGLOBAL 	HRGN;
typedef HGLOBAL		HDC;
typedef HGLOBAL		HFONT;
typedef HGLOBAL 	HTASK;
typedef HGLOBAL 	HKEY;
typedef HGLOBAL 	HDESK;
typedef HGLOBAL 	HPEN;
typedef HGLOBAL 	HRSRC;
typedef HGLOBAL 	HSTR;
typedef HGLOBAL 	HWINSTA;
typedef HGLOBAL 	HKL;
typedef HGLOBAL 	HGDIOBJ;
typedef HGLOBAL 	HDWP;
typedef HGLOBAL 	HWND;
typedef HGLOBAL		HBRUSH;
typedef HGLOBAL		HPALETTE;
typedef HGLOBAL		HIMC;
typedef HGLOBAL		HGDIOBJ;

typedef HMETAFILEPICT 	HMF;
typedef HENHMETAFILE 	HEMF;

typedef WORD	CLIPFORMAT;
typedef DWORD	COLORREF;

typedef char	CCHAR;          
typedef DWORD	LCID;         
typedef WORD	LANGID;      

typedef unsigned char byte;

// API declaration macros

#define WINAPI  	FAR PASCAL
#define CALLBACK	FAR PASCAL

#define STDMETHODCALLTYPE __stdcall
#define STDMETHODVCALLTYPE __cdecl
#define STDMETHODIMP_(TYPE) TYPE STDMETHODCALLTYPE
#define STDMETHODIMP STDMETHODIMP_(HRESULT)
#define STDMETHOD_(RET, NAME) virtual RET STDMETHODCALLTYPE NAME
#define STDMETHOD(NAME) STDMETHOD_(HRESULT, NAME)
#define STDMETHODV_(RET, NAME) virtual RET STDMETHODVCALLTYPE NAME
#define STDMETHODV(NAME) STDMETHOD_(HRESULT, NAME)

#define STDAPICALLTYPE  __stdcall
#define STDAPIVCALLTYPE __cdecl
#define EXTERN_C extern "C"

#define STDAPI EXTERN_C HRESULT STDAPICALLTYPE

#endif //_TYPES_H_
