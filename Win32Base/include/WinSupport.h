/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: WinSupport.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$
			
Header file to provide substitutions and kludges when compiling FieldWorks for MacOS.
Contains definitions copied and (sometimes modified) from the Windows headers:

	WTYPES.H
	WINNT.H
	WINDEF.H
	WINERROR.H
	BASETYPS.H
	BASETSD.H
	WINBASE.H
	MAPIWIN.H

The copied portions are included here in sections named according to the above Windows
headers. It is suggested that further bits copied from Windows headers be kept in
these sections to facilitate later reading and debugging of the code.

Following the portions copied from Windows headers is an assortment of kludges and
other things needed for a port to MacOS.

Some of the implementation is provided in WinSupport.cpp.

Added GetLastError and some more error codes.
	2002-11-08, GDLC
Added ERROR_NO_MORE_FILES
	2002-11-07, GDLC
Added FILE_ATTRIBUTE #defines
	2002-11-06, GDLC
Added FindNextFile()
	2002-10-31, GDLC
Modified to compile with Apple Developer tools
    2002-09-26, GDLC
Used forward definition of _BUNDLE_INFO instead of void in HMODULE.
	2002-09-03, GDLC
Changed HMODULE to allow for the BUNDLE_INFO structure defined in WinSupportInternals.h
	2002-08-27, GDLC
Moved #pragma export directives to WinSupport.cpp
	2002-08-21, GDLC
Changed definition of HMODULE from file refNum to CFURLRef
	2002-07-12, GDLC
Removed FormatMessge()
	2002-06-25, GDLC
Reversed order of DWORDs in FILETIME.
	2002-04-23, GDLC
Added LPVOIC & LPCVOID. Reordered adaptations from WINBASE.H.
	2002-03-27, GDLC
Changed HINSTANCE and HMODULE to short for MacOS file RefNum. Changed HGLOBAL back to
HANDLE. Changed _MAC to MAC (since MAC is defined in common.h).
	2002-03-21, GDLC
Removed OutputDebugStringA/W and GetModuleFileNameA/W as they are not used in
Windows FieldWorks code. Added GetModuleHandle().
	2002-03-20, GDLC
Added HINSTANCE.
	2002-03-15, GDLC
Added CALLBACK, FARPROC, NEARPROC, PROC. Changed IsBad...() functions to return BOOL
rather than bool.
	2002-03-14, GDLC
Added SYSTEMTIME, TIME_ZONE_INFORMATION and GetTimeZoneInformation().
	2002-03-12, GDLC
Changed definitions of enums, structs, and unions to suit C++ (this file is not intended
for compilation as a C source)
	2002-03-08, GDLC
Changed LARGE_INTEGER and ULARGE_INTEGER according to WINNT.H
	2002-02-28, GDLC
Items from WTYPES.H moved from COMSUpport.h into this header.
	2002-02-25, GDLC
Added STDMETHODCALLTYPE, STDMETHODVCALLTYPE, STDAPICALLTYPE, STDAPIVCALLTYPE,
STDAPI_(type), STDMETHODIMP, and STDMETHODIMP_(type) from BASETYPS.H. Added CLSCTX
enum from WTYPES.H.
	2002-02-22, GDLC
Added Win32 critical section dummies. Added definition of __stdcall
	2002-02-13, GDLC
Added GetModuleFileName() & HMODULE
	2002-01-23, GDLC
Changed to #pragma once
	2002-01-17, GDLC
More parts of the CodeGen WinSupport copied into this file
	2002-01-16, GDLC
Written by extracting all non-COM items from ComSupport.h
	2001-07-17, GDLC
Added IsBadStringPtrA/W, IsBadReadPtr, IsBadWritePtr, UINT_PTR, LPCWSTR et al
	2001-08-28,	GDLC
Added E_FAIL
	2001-11-12, GDLC
----------------------------------------------------------------------------------------------*/

#pragma once

// Copied, with adaptations, from the Windows header  WTYPES.H

typedef void				VOID;
typedef	void				*PVOID;
typedef	void				*LPVOID;

typedef unsigned short		WORD;

typedef char				CHAR;
typedef CHAR				*LPSTR;
typedef const CHAR			*LPCSTR;

typedef unsigned int		UINT;

typedef int					INT;

typedef long				BOOL;	// But we started with this as a bool in CodeGen

typedef long				LONG;

typedef unsigned long		DWORD;
typedef	DWORD				ULONG;

typedef	long long			__int64;
typedef	unsigned long long	__uint64;

typedef __int64				LONGLONG;

typedef __uint64			ULONGLONG;

typedef LONGLONG			*PLONGLONG;

typedef ULONGLONG			*PULONGLONG;

//	FILETIME holds the number of 100-nanosecond intervals since 12:00AM on
//	January 1, 1601. It is signed.
//
//	Windows documentation recommends that programmers calculate relative times
//	by using 64-bit subtraction/addition on the contents of FILETIME structures.
//	In order for this to work in MacFieldWorks we need to reverse the order of
//	the two DWORDs in FILETIME.
//
//	TODO: Check whether this introduces any problems.
typedef struct
{
    DWORD dwHighDateTime;
    DWORD dwLowDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

typedef struct
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}	SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef long				HRESULT;

//	The Windows types HANDLE and HRSRC are defined as MacOS Handles
typedef Handle				HANDLE;
typedef Handle				HRSRC;

//	HMODULE is defined in Windows headers as a pointer to a void.
//	For applications ported to MacOS and presented to the user packaged in a
//	CFBundle, it is reasonable to take HMODULE as a pointer to the BUNDLE_INFO
//	structure held by a per-process static variable. The details of the BUNDLE_INFO
//	structure are kept private to the WinSuppportLib.
typedef struct BUNDLE_INFO	*HMODULE;

typedef short				HINSTANCE;

typedef wchar_t				WCHAR;

typedef WCHAR				TCHAR;

typedef WCHAR				*LPWSTR;

typedef TCHAR				*LPTSTR;

typedef const WCHAR			*LPCWSTR;

typedef const TCHAR			*LPCTSTR;

//	The following 4 definitions are for the case of defined(_WIN32) && !defined(OLE2ANSI)
//	in order to correspond to using Unicode characters in BSTR strings. This makes
//	this implementation of COM incompatible with the implementation used by
//	Microsoft Office 98 and Microsoft Office 2001; this in turn means that Mac
//	FieldWorks will not be able to engage in cross-process COM with either version
//	of Mac Office -- but cross-process COM was been stricken from the list of
//	requirements before this porting task was started! But the benefit is that the
//	Windows FieldWorks code that uses BSTRs can easily be compiled for MacOS.

typedef WCHAR				OLECHAR;
typedef OLECHAR				*LPOLESTR;
typedef const OLECHAR		*LPCOLESTR;
#define OLESTR(str)			L##str

typedef OLECHAR				*BSTR;

typedef BSTR				*LPBSTR;

/* 0 == FALSE, -1 == TRUE */
typedef short				VARIANT_BOOL;

#define VARIANT_TRUE ((VARIANT_BOOL)-1)
#define VARIANT_FALSE ((VARIANT_BOOL)0)

typedef enum
    {
    CLSCTX_INPROC_SERVER		= 0x1,
	CLSCTX_INPROC_HANDLER		= 0x2,
	CLSCTX_LOCAL_SERVER			= 0x4,
	CLSCTX_INPROC_SERVER16		= 0x8,
	CLSCTX_REMOTE_SERVER		= 0x10,
	CLSCTX_INPROC_HANDLER16		= 0x20,
	CLSCTX_INPROC_SERVERX86		= 0x40,
	CLSCTX_INPROC_HANDLERX86	= 0x80,
	CLSCTX_ESERVER_HANDLER		= 0x100,
	CLSCTX_RESERVED				= 0x200,
	CLSCTX_NO_CODE_DOWNLOAD		= 0x400,
	CLSCTX_NO_WX86_TRANSLATION	= 0x800,
	CLSCTX_NO_CUSTOM_MARSHAL	= 0x1000,
	CLSCTX_ENABLE_CODE_DOWNLOAD	= 0x2000,
	CLSCTX_NO_FAILURE_LOG		= 0x4000
    }	CLSCTX;


typedef enum
	{
		STATFLAG_DEFAULT	= 0,
		STATFLAG_NONAME		= 1,
		STATFLAG_NOOPEN		= 2
	} STATFLAG;

//--end copy from WTYPES.H--------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  WINNT.H
// Both of these unions are rather strange, having a named part
// and an unnamed part that are identical!
//Line 421
typedef union
{
    struct {
        DWORD LowPart;
        LONG HighPart;
    };
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef LARGE_INTEGER *PLARGE_INTEGER;

typedef union
{
	struct {
		DWORD	LowPart;
		DWORD	HighPart;
	};
	struct {
		DWORD	LowPart;
		DWORD	HighPart;
	} u;
    ULONGLONG QuadPart;
} ULARGE_INTEGER;

typedef ULARGE_INTEGER *PULARGE_INTEGER;

//Line 4524
#define TIME_ZONE_ID_UNKNOWN	0
#define TIME_ZONE_ID_STANDARD	1
#define TIME_ZONE_ID_DAYLIGHT	2

//Line 4685
#define FILE_ATTRIBUTE_READONLY					0x00000001
#define FILE_ATTRIBUTE_HIDDEN					0x00000002
#define FILE_ATTRIBUTE_SYSTEM					0x00000004

#define FILE_ATTRIBUTE_DIRECTORY				0x00000010
#define FILE_ATTRIBUTE_ARCHIVE					0x00000020
//#define FILE_ATTRIBUTE_DEVICE					0x00000040
#define FILE_ATTRIBUTE_NORMAL					0x00000080
#define FILE_ATTRIBUTE_TEMPORARY				0x00000100
//#define FILE_ATTRIBUTE_SPARSE_FILE			0x00000200
//#define FILE_ATTRIBUTE_REPARSE_POINT			0x00000400
#define FILE_ATTRIBUTE_COMPRESSED				0x00000800
#define FILE_ATTRIBUTE_OFFLINE					0x00001000
//#define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED	0x00002000
//#define FILE_ATTRIBUTE_ENCRYPTED				0x00004000

//--end copy from WINNT.H--------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  WINDEF.H

#define MAX_PATH 260

#define	CALLBACK
#define	WINAPI

typedef void				*LPVOID;
typedef const void			*LPCVOID;

typedef int					INT;
typedef unsigned int		UINT;
typedef unsigned int		*PUINT;

typedef BOOL				*LPBOOL;

typedef struct
	{
		long x;
		long y;
	}	POINT;

typedef struct POINT		*PPOINT;

typedef struct POINT		*LPPOINT;

typedef struct
    {
    long left;
    long top;
    long right;
    long bottom;
    }	RECT;

typedef struct RECT			*PRECT;

typedef struct RECT			*LPRECT;

typedef const RECT			*LPCRECT;

typedef HANDLE				HGLOBAL;

typedef void	(CALLBACK *FARPROC)();
typedef void	(CALLBACK *NEARPROC)();
typedef void	(CALLBACK *PROC)();

//--end copy from WINDEF.H--------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  WINERROR.H

//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
//#define FACILITY_WINDOWS				8
//#define FACILITY_URT					19
//#define FACILITY_STORAGE				3
//#define FACILITY_SSPI					9
//#define FACILITY_SCARD				16
//#define FACILITY_SETUPAPI				15
//#define FACILITY_SECURITY				9
//#define FACILITY_RPC					1
#define FACILITY_WIN32					7
//#define FACILITY_CONTROL				10
//#define FACILITY_NULL					0
//#define FACILITY_MSMQ					14
//#define FACILITY_MEDIASERVER			13
//#define FACILITY_INTERNET				12
#define FACILITY_ITF					4
//#define FACILITY_DISPATCH				2
//#define FACILITY_COMPLUS				17
//#define FACILITY_CERT					11
//#define FACILITY_ACS					20
//#define FACILITY_AAF					18

#define ERROR_SUCCESS					0L

//  The system cannot find the file specified.
#define ERROR_FILE_NOT_FOUND			2L

//	Invalid drive
#define ERROR_INVALID_DRIVE				15L

//	There are no more files
#define ERROR_NO_MORE_FILES				18L

//  This function is not supported on this system.
#define ERROR_CALL_NOT_IMPLEMENTED		120L

//	The specified module could not be found
#define ERROR_MOD_NOT_FOUND				126L

//  The specified path is invalid.
#define ERROR_BAD_PATHNAME				161L

#define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)

// Generic test for success on any status value (non-negative numbers
// indicate success).
#define SUCCEEDED(Status) ((HRESULT)(Status) >= 0)

// and the inverse

#define FAILED(Status) ((HRESULT)(Status)<0)

#define S_OK							_HRESULT_TYPEDEF_(0x00000000L)

//  Catastrophic failure
#define E_UNEXPECTED					_HRESULT_TYPEDEF_(0x8000FFFFL)

//  Ran out of memory
#define E_OUTOFMEMORY					_HRESULT_TYPEDEF_(0x8007000EL)

//  One or more arguments are invalid
#define E_INVALIDARG					_HRESULT_TYPEDEF_(0x80070057L)

//  Unspecified error
#define E_FAIL							_HRESULT_TYPEDEF_(0x80004005L)

//  Class not registered
#define REGDB_E_CLASSNOTREG				_HRESULT_TYPEDEF_(0x80040154L)

//  Invalid class string
#define CO_E_CLASSSTRING				_HRESULT_TYPEDEF_(0x800401F3L)

//  A disk error occurred during a write operation.
#define STG_E_WRITEFAULT				_HRESULT_TYPEDEF_(0x8003001DL)

//  A disk error occurred during a read operation.
#define STG_E_READFAULT					_HRESULT_TYPEDEF_(0x8003001EL)


//--end copy from WINERROR.H------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  BASETYPS.H

#define STDMETHODCALLTYPE
#define STDMETHODVCALLTYPE

#define STDAPICALLTYPE
#define STDAPIVCALLTYPE

#define STDAPI					HRESULT STDAPICALLTYPE
#define STDAPI_(type)           type STDAPICALLTYPE

#define STDMETHODIMP            HRESULT STDMETHODCALLTYPE
#define STDMETHODIMP_(type)     type STDMETHODCALLTYPE

//--end copy from BASETYPS.H------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  BASETSD.H

// The INT_PTR is guaranteed to be the same size as a pointer.  Its
// size will change with pointer size (32/64).  It should be used
// anywhere that a pointer is cast to an integer type. UINT_PTR is
// the unsigned variation.

typedef int					INT_PTR, *PINT_PTR;
typedef unsigned int		UINT_PTR, *PUINT_PTR;

typedef long				LONG_PTR, *PLONG_PTR;
typedef unsigned long		ULONG_PTR, *PULONG_PTR;


typedef	unsigned long		SIZE_T, *PSIZE_T;

//--end copy from BASETSD.H-------------------------------------------------------------------

// Copied, with adaptations, from the Windows header WINBASE.H
//
// The items in this section are in the same order as in WINBASE.H
// Approximate line numbers in WINBASE.H are given as comments.

//Line 56
#define INVALID_HANDLE_VALUE ((HANDLE)-1)

//Line 64
#define TIME_ZONE_ID_INVALID ((DWORD)0xFFFFFFFF)

//	For further comments about LockResource(), LoadResource() and
//	SizeOfResource(), see FindResource() [//Line 4675]

//Line 1206					
inline void*	LockResource(HRSRC hres)
					{ HLock(hres); return *hres; }
//Line 2178
DWORD			GetLastError(VOID);

//Line 2475
inline HGLOBAL	LoadResource(void*, HRSRC hres)
					{ return hres; }

//Line 2483
inline long		SizeofResource(void*, HRSRC hres)
					{ return GetHandleSize(hres); }

//Line 2735
BOOL WINAPI		FindClose(
    /*IN OUT*/ HANDLE hFindFile
    );

//Line 3085
void			GetSystemTimeAsFileTime(
				LPFILETIME lpSystemTimeAsFileTime);

//Line 3124
typedef struct
{
    LONG		Bias;
    WCHAR		StandardName[ 32 ];
    SYSTEMTIME	StandardDate;
    LONG		StandardBias;
    WCHAR		DaylightName[ 32 ];
    SYSTEMTIME	DaylightDate;
    LONG		DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION, *LPTIME_ZONE_INFORMATION;

//Line 3146
DWORD			GetTimeZoneInformation(
				LPTIME_ZONE_INFORMATION lpTimeZoneInformation);

//Line 3223
DWORD			GetTickCount(void);

//Line 3248
//#pragma export on
//DWORD FormatMessage(
//	/*IN*/ DWORD	dwFlags,
//	/*IN*/ LPCVOID	lpSource,
//	/*IN*/ DWORD	dwMessageId,
//	/*IN*/ DWORD	dwLanguageId,
//	/*OUT*/ LPSTR	lpBuffer,
//	/*IN*/ DWORD	nSize,
//	/*IN*/ va_list	*Arguments
//	);
//#pragma export off
//
//#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
//#define FORMAT_MESSAGE_IGNORE_INSERTS  0x00000200
//#define FORMAT_MESSAGE_FROM_STRING     0x00000400
//#define FORMAT_MESSAGE_FROM_HMODULE    0x00000800
//#define FORMAT_MESSAGE_FROM_SYSTEM     0x00001000
//#define FORMAT_MESSAGE_ARGUMENT_ARRAY  0x00002000
//#define FORMAT_MESSAGE_MAX_WIDTH_MASK  0x000000FF

//Line 4057
typedef struct
{
	DWORD		dwFileAttributes;
	FILETIME	ftCreationTime;
	FILETIME	ftLastAccessTime;
	FILETIME	ftLastWriteTime;
	DWORD		nFileSizeHigh;
	DWORD		nFileSizeLow;
	DWORD		dwReserved0;
	DWORD		dwReserved1;
	CHAR		cFileName[ MAX_PATH ];
	CHAR		cAlternateFileName[ 14 ];
#ifdef MAC
	DWORD		dwFileType;
	DWORD		dwCreatorType;
	WORD		wFinderFlags;
#endif
} WIN32_FIND_DATA, *PWIN32_FIND_DATA, *LPWIN32_FIND_DATA;

//Line 4438
//	Only GetModuleFilename() is used by Windows FW code; and only 8-bit character
//	filenames are relevant on MacOS. Despite its name it returns the full pathname.
DWORD			GetModuleFileName(
				/*IN*/ HMODULE hModule,
				/*OUT*/ LPSTR lpFilename,
				/*IN*/ DWORD nSize);

//Line 4468
//	Only GetModuleHandle() is used by Windows FW code; and only 8-bit character
//	filenames are relevant on MacOS. The only parameter implemented is NULL
//	which indicates the application's resource fork; other modules are probably
//	not relevant on MacOS.
//	TODO: Be on the lookout for relevance of other parameters and extend this if necessary.
HMODULE			GetModuleHandle(/*IN*/ LPCSTR lpModuleName);

//Line 4657
//	Only OutputDebugString() is used by Windows FW code; and only 8-bit character
//	output is relevant to MacOS debugging.
//	OutputDebugString() generates action only for the Debug and Test targets; the
//	Final target has zero code for OutputDebugString().
#ifdef	OutputDebugString
#undef	OutputDebugString
#endif
#ifdef	_DEBUG
#define			OutputDebugString(s) OutputDebugStringMac(s)
VOID			OutputDebugStringMac(/*IN*/ LPCSTR lpOutputString);
#else
#define			OutputDebugString(s) ((void) 0)
#endif

// For this Mac port there is no need to have a handle to an info block
// about the resource, so we make FindResource() not only locate the resource
// but also load it into the heap. Thus our kludge for LoadResource() 
// will merely return the handle it is given, and HRSRC and HGLOBAL are
// both merely Mac Handles.

//Line 4675
HRSRC			FindResource(void*, int rid, const char* type);

//Line 5864
HANDLE			FindFirstFile(
				LPCSTR				lpFileName,
				LPWIN32_FIND_DATA	lpFindFileData);

//Line 5881
BOOL			FindNextFile(
				HANDLE hFindFile,
				LPWIN32_FIND_DATA lpFindFileData);

//Line 5904
// Copied from WINBASE.H and modified by removing the final 'A'
// and WINBASEAPI and WINAPI
DWORD			SearchPath(
				LPCSTR	lpPath,
				LPCSTR	lpFileName,
				LPCSTR	lpExtension,
				DWORD	nBufferLength,
				LPSTR	lpBuffer,
				LPSTR	*lpFilePart);

//Line 7587
inline BOOL		IsBadCodePtr(FARPROC /*lpfn*/) { return false; };

//Line 7825
inline BOOL		IsBadReadPtr(const void */*lp*/, UINT /*ucb*/) { return false; }

//Line 7833
inline BOOL		IsBadWritePtr(LPVOID /*lp*/, UINT /*ucb*/) { return false; }

//Line 7864
//	No good way of testing these conditions on MacOS
//	Both IsBadStringPtrA and IsBadStringPtrW are used in FM code
inline BOOL		IsBadStringPtrA(LPCSTR /*lpsz*/, UINT_PTR /*ucchMax*/) { return false; }
inline BOOL		IsBadStringPtrW(LPCWSTR /*lpsz*/, UINT_PTR /*ucchMax*/) { return false; }
#ifdef UNICODE
#define			IsBadStringPtr  IsBadStringPtrW
#else
#define			IsBadStringPtr  IsBadStringPtrA
#endif // !UNICODE

//--end copy from WINBASE.H------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  WINDEF.H

//	Windows uses a #define:
//	#define MAKEINTRESOURCE(i) (LPSTR)((ULONG_PTR)((WORD)(i)))
//	For MacOS an inline function works well.
inline int MAKEINTRESOURCE(int id) { return id; }

/*
 * Predefined Windows Resource Types
 */
#define RT_CURSOR           MAKEINTRESOURCE(1)
#define RT_BITMAP           MAKEINTRESOURCE(2)
#define RT_ICON             MAKEINTRESOURCE(3)
#define RT_MENU             MAKEINTRESOURCE(4)
#define RT_DIALOG           MAKEINTRESOURCE(5)
#define RT_STRING           MAKEINTRESOURCE(6)
#define RT_FONTDIR          MAKEINTRESOURCE(7)
#define RT_FONT             MAKEINTRESOURCE(8)
#define RT_ACCELERATOR      MAKEINTRESOURCE(9)
#define RT_RCDATA           MAKEINTRESOURCE(10)
#define RT_MESSAGETABLE     MAKEINTRESOURCE(11)

#define DIFFERENCE          11
#define RT_GROUP_CURSOR 	MAKEINTRESOURCE((ULONG_PTR)RT_CURSOR + DIFFERENCE)
#define RT_GROUP_ICON   	MAKEINTRESOURCE((ULONG_PTR)RT_ICON + DIFFERENCE)
#define RT_VERSION      	MAKEINTRESOURCE(16)
#define RT_DLGINCLUDE   	MAKEINTRESOURCE(17)
//#if(WINVER >= 0x0400)
#define RT_PLUGPLAY     	MAKEINTRESOURCE(19)
#define RT_VXD          	MAKEINTRESOURCE(20)
#define RT_ANICURSOR    	MAKEINTRESOURCE(21)
#define RT_ANIICON      	MAKEINTRESOURCE(22)
//#endif /* WINVER >= 0x0400 */
#define RT_HTML         	MAKEINTRESOURCE(23)

//--end copy from WINDEF.H------------------------------------------------------------------

// Copied, with adaptations, from the Windows header  MAPIWIN.H

/* Synchronization */
//	TODO: Reconsider this for MacOS X
//	Classic MacOS uses cooperative multitasking, so it is not possible for
//	control to be wrested from functions in the middle of their execution.
//	Thus we can simply program the increment/decrement in a standard manner.
//	When WinSupport.h is compiled for MacOS X this simple case will no longer
//	apply - hopefully there are system calls in MacOS X to handle this.
#define InterlockedIncrement(plong) (++(*(plong)))
#define InterlockedDecrement(plong) (--(*(plong)))

#define CRITICAL_SECTION            ULONG
#define InitializeCriticalSection(_pcs) ((void)0)
#define DeleteCriticalSection(_pcs)     ((void)0)
#define EnterCriticalSection(_pcs)      ((void)0)
#define LeaveCriticalSection(_pcs)      ((void)0)

//--end copy from MAPIWIN.H------------------------------------------------------------------

// Kludges and other things needed for a port to MacOS.

#define __cdecl
#define __stdcall

#define min(x, y) (x < y ? x : y)

// String/Memory comparison
int _strnicmp(const char *p, const char *q, std::size_t n);
int _memicmp(const void *, const void *, std::size_t);
