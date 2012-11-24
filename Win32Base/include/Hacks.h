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

#include "MessageBox.h"

enum { CP_ACP, CP_UTF8 };

int WideCharToMultiByte(int, int, const OLECHAR*, int, char*, int, char*, char*);
int MultiByteToWideChar(int, int, const char*, int, OLECHAR*, int);

bool IsBadStringPtrW(const OLECHAR*, unsigned long);
bool IsBadStringPtrA(const char*, unsigned long);
bool IsBadReadPtr(const void*, unsigned long);
bool IsBadWritePtr(const void*, unsigned long);
bool IsBadCodePtr(const void*);

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

#define MAX_PATH 260
#define MAX_COMPUTERNAME_LENGTH 15

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))
#define GetRValue(rgb)      ((BYTE)(rgb))
#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

bool SetRect(RECT*, int, int, int, int);
bool OffsetRect(RECT*, int,int);

short ClientToScreen(HWND, POINT*);
short ScreenToClient(HWND, POINT*);

enum
{
	// These need to match the values sent by Mono in KeyEventArgs.KeyCode
	VK_TAB      = 0x09,
	VK_RETURN   = 0x0D,
	VK_PRIOR    = 0x21,
	VK_NEXT     = 0x22,
	VK_END      = 0x23,
	VK_HOME     = 0x24,
	VK_LEFT     = 0x25,
	VK_UP       = 0x26,
	VK_RIGHT    = 0x27,
	VK_DOWN     = 0x28,
//	VK_SELECT   = 0x29,
//	VK_PRINT    = 0x2A,
//	VK_EXECUTE  = 0x2B,
//	VK_SNAPSHOT = 0x2C,
	VK_INSERT   = 0x2D,
	VK_DELETE   = 0x2E,
//	VK_HELP     = 0x2F,
	VK_F1       = 0x70,
	VK_F2       = 0x71,
	VK_F3       = 0x72,
	VK_F4       = 0x73,
	VK_F5       = 0x74,
	VK_F6       = 0x75,
	VK_F7       = 0x76,
	VK_F8       = 0x77,
};

#define LANGIDFROMLCID(localeIdentifier) ((WORD)localeIdentifier)

DWORD GetModuleFileName(HMODULE, OLECHAR* buf, DWORD length);
DWORD GetModuleFileName(HMODULE, TCHAR*   buf, DWORD length);

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

// TODO-P4CL23677-Merge
// These functions defines don't account for specified length
// TODO Replace these macros with multiple functions
#define strcpy_s(DST, NUM, SRC) strcpy(DST, SRC)
#define strncpy_s(DST, NUM, SRC, CNT) strncpy(DST, SRC, CNT)
#define strcat_s(DST, NUM, SRC) strcat(DST, SRC)
#define strncat_s(DST, NUM, SRC, CNT) strncat(DST, SRC, CNT)

inline int wcscmp(const OLECHAR* s1, const OLECHAR* s2)
{
	return u_strcmp(s1, s2);
}

inline int wcsncmp(const OLECHAR* s1, const OLECHAR* s2, size_t n)
{
	return u_strncmp(s1, s2, n);
}

#define wcsncmp_s(LEFT, RIGHT, NUM) u_strncmp(LEFT, RIGHT, NUM)
#define _wcsicmp(LEFT, RIGHT) u_strcasecmp(LEFT, RIGHT, 0)

#define fopen_s(FH, FILE, MODE) ((*FH = fopen(FILE, MODE)) == NULL)

#define _TRUNCATE	-1

inline int wcslen(const OLECHAR *str)
{
	return u_strlen(str);
}

inline int wcslen_s(const OLECHAR *str, const int size)
{
	return u_strlen(str);
}

inline int wcscat_s(OLECHAR *dst, const int size, const OLECHAR *src)
{
	u_strcat(dst, src);
	return 0;
}

inline OLECHAR* wcscpy(OLECHAR *dst, const OLECHAR *src)
{
	return u_strcpy(dst, src);
}

int wcscpy_s(OLECHAR *dst, const int size, const OLECHAR *src);


inline OLECHAR* wcsncpy(OLECHAR* dst, const OLECHAR* src, const int size)
{
	return u_strncpy(dst, src, size);
}

int wcsncpy_s(OLECHAR* dst, const size_t dsize, const OLECHAR* src, const size_t size);

inline int _stricmp(const char *string1, const char *string2)
{
	return strcasecmp(string1, string2);
}
inline int _strnicmp(const char *string1, const char *string2, size_t count)
{
	return strncasecmp(string1, string2, count);
}
inline int _wcsnicmp(const OLECHAR* string1, const OLECHAR* string2, size_t count)
{
	return 	u_strncasecmp(string1, string2, count, 0);
}

// Not done as a macro because use of elipsis..
int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...);
int _snprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, ...);

#endif //HACKS_H
