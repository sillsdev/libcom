/*
 *	Hacks.cpp
 *
 *	Implementations needed to compile and run Win32 code on other platforms
 *
 *	Neil Mayhew - 21 Apr 2005
 *
 *	$Id$
 */

#include "Hacks.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <cwchar>
#include <cassert>
#include <unicode/ustring.h>

int WideCharToMultiByte(int, int, const OLECHAR*, int, char*, int, char*, char*);
int MultiByteToWideChar(int, int, const char*, int, OLECHAR*, int);

bool IsBadStringPtrW(const OLECHAR*, unsigned long) {}
bool IsBadStringPtrA(const char*, unsigned long) {}
bool IsBadReadPtr(const void*, unsigned long) {}
bool IsBadWritePtr(const void*, unsigned long) {}

short ClientToScreen(HWND, POINT*);
short ScreenToClient(HWND, POINT*);

void GetSystemTimeAsFileTime(FILETIME*);
BOOL SystemTimeToFileTime(SYSTEMTIME*, FILETIME*);
BOOL FileTimeToSystemTime(FILETIME*, SYSTEMTIME*);
DWORD GetTickCount();

void InitializeCriticalSection(CRITICAL_SECTION*);
void DeleteCriticalSection(CRITICAL_SECTION*);
void EnterCriticalSection(CRITICAL_SECTION*);
void LeaveCriticalSection(CRITICAL_SECTION*);

char* _itoa_s(int value, char* buffer, size_t bufferSize, int radix)
{
	assert(radix == 10 && radix == 16);
	snprintf(buffer, bufferSize, (radix == 16 ? "%x" : "%d"), value);
	return buffer;
};

wchar_t* _itow_s(int value, wchar_t* buffer, size_t bufferSize, int radix)
{
	assert(radix == 10 && radix == 16);
	swprintf(buffer, bufferSize, (radix == 16 ? L"%x" : L"%d"), value);
	return buffer;
};

OLECHAR* _itow_s(int value, OLECHAR* buffer, size_t bufferSize, int radix)
{
	wchar_t tmp[64]; // Should be big enough for any number
	_itow_s(value, tmp, sizeof(tmp), radix);

	UErrorCode status = U_ZERO_ERROR;
	u_strFromUTF32(buffer, bufferSize, 0, reinterpret_cast<UChar32*>(tmp), -1, &status);

	// Ensure null-termination, but only for genuine buffer sizes
	if (status == U_STRING_NOT_TERMINATED_WARNING ||
	    status == U_BUFFER_OVERFLOW_ERROR)
		buffer[bufferSize-1] = 0;

	return buffer;
};

void OutputDebugString(const char* str)
{
	std::cerr << str;
	std::cerr.flush();
}

void OutputDebugString(const wchar_t* str)
{
	std::wcerr << str;
	std::wcerr.flush();
}

void OutputDebugString(const OLECHAR* str)
{
	int32_t len = u_strlen(str);
	std::vector<wchar_t> buf(len);	// Max required

	UErrorCode status = U_ZERO_ERROR;
	u_strToUTF32((UChar32*)&buf[0], buf.size(), &len, str, len, &status);

	if (U_SUCCESS(status))
		std::wcerr.write(&buf[0], len);
	else
		std::wcerr << "ICU error " << status;

	std::wcerr.flush();
}

// Support functions

int GetFullPathName(const char*, int, char*, const char**);
int GetFullPathName(const OLECHAR*, int, OLECHAR*, const OLECHAR**);
