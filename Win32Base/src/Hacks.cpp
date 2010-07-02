//
//	$Id$
//
//	Implementations needed to compile and run Win32 code on other platforms
//
//	Neil Mayhew - 2005-04-21
//
// Win32 Compatibility Library
// Copyright (C) 2007 SIL International
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://www.gnu.org/licenses/lgpl.html
// 

#include <wchar.h>
#include "Hacks.h"

#include <cwchar>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <unicode/ustring.h>
#include <stdarg.h>
#include <stdio.h>

// Begin copied code from Wine's uitools.c, Modified 2007-02-12 MarkS.

/*
 * User Interface Functions
 *
 * Copyright 1997 Dimitrie O. Paun
 * Copyright 1997 Bertho A. Stultiens
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/***********************************************************************
 *		OffsetRect (USER32.@)
 */
bool OffsetRect( LPRECT rect, INT x, INT y )
{
    if (!rect) return FALSE;
    rect->left   += x;
    rect->right  += x;
    rect->top    += y;
    rect->bottom += y;
    return TRUE;
}

/***********************************************************************
 *		SetRect (USER32.@)
 */
bool SetRect( LPRECT rect, INT left, INT top, INT right, INT bottom )
{
    if (!rect) return FALSE;
    rect->left   = left;
    rect->right  = right;
    rect->top    = top;
    rect->bottom = bottom;
    return TRUE;
}


// End copied code from Wine's uitools.c

int WideCharToMultiByte(int codePage, int flags,
	const OLECHAR* src, int srcLen, char* dst, int dstLen,
	char* defaultChar, char* defaultUsed)
{
	// PORT-TODO
	assert(codePage == CP_ACP || codePage == CP_UTF8);

	UErrorCode status = U_ZERO_ERROR;
	int32_t spaceRequiredForData;

	// if dstLen is 0, pre-flight.
	u_strToUTF8(dst, dstLen, &spaceRequiredForData, src, srcLen, &status);

	if (U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR)
		throw std::runtime_error("Unable to convert from UTF-16 to UTF-8");

	return spaceRequiredForData;
}

int MultiByteToWideChar(int codePage, int flags,
	const char* src, int srcLen, OLECHAR* dst, int dstLen)
{
	// PORT-TODO
	assert(codePage == CP_ACP || codePage == CP_UTF8);

	UErrorCode status = U_ZERO_ERROR;
	int32_t spaceRequiredForData;

	// if dstLen is 0, pre-flight.
	u_strFromUTF8(dst, dstLen, &spaceRequiredForData, src, srcLen, &status);

	if (U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR)
		throw std::runtime_error("Unable to convert from UTF-8 to UTF-16");

	return spaceRequiredForData;
}

#include <setjmp.h>
#include <signal.h>

static bool g_bPtrTestInstalled;
static jmp_buf g_PtrTestJmpBuf;

// signal handler to catch seg faults
// returns to the setjmp position.
void __cdecl PtrTestHandler(int nSig)
{
	if (g_bPtrTestInstalled)
		longjmp(g_PtrTestJmpBuf, 1);
}

// Attempt to read every byte in the lp array.
// If seg fault return true
// else return false.
bool __IsBadReadPtr(const void* lp, UINT cb)
{
	if (!cb)
		return false;
	if (!lp)
		return true;

	UINT i;
	BYTE b1;
	bool bRet = false;
	void (__cdecl* pfnPrevHandler)(int);
	g_bPtrTestInstalled	= true;
	if (setjmp(g_PtrTestJmpBuf))
	{
		bRet = true;
		goto Ret;
	}
	pfnPrevHandler = signal(SIGSEGV, PtrTestHandler);

	for (i = 0; i < cb; i++)
		b1 = ((BYTE*)lp)[i];
Ret:
	g_bPtrTestInstalled	= false;
	signal(SIGSEGV, pfnPrevHandler);

	return bRet;
}

// return true if lp is not valid for reading.
bool __IsBadCodePtr(const void* lp)
{
	return __IsBadReadPtr(lp, 1);
}

// attempt to |= 0 every byte in lp array
// if seg fault return true
// else return false.
bool __IsBadWritePtr(const void *lp, UINT cb)
{
	if (!cb)
		return false;
	if (!lp)
		return true;

	UINT i;
	bool bRet = false;
	void (__cdecl* pfnPrevHandler)(int);
	g_bPtrTestInstalled	= true;
	if (setjmp(g_PtrTestJmpBuf))
	{
		bRet = true;
		goto Ret;
	}
	pfnPrevHandler = signal(SIGSEGV, PtrTestHandler);

	for (i = 0; i < cb; i++)
		((BYTE*)lp)[i] |= 0;
Ret:
	g_bPtrTestInstalled	= false;
	signal(SIGSEGV, pfnPrevHandler);

	return bRet;
}

bool IsBadStringPtrW(const OLECHAR* ptr, unsigned long len) { return false; }
bool IsBadStringPtrA(const char* ptr, unsigned long len ) { return false; }
bool IsBadReadPtr(const void* ptr, unsigned long len) { return __IsBadReadPtr(ptr, len); }
bool IsBadWritePtr(const void* ptr, unsigned long len) { return __IsBadWritePtr(ptr,len); }
bool IsBadCodePtr(const void* ptr) { return __IsBadCodePtr(ptr); }



char* _itoa_s(int value, char* buffer, size_t sizeInCharacters, int radix)
{
	assert(radix == 10 || radix == 16);
	snprintf(buffer, sizeInCharacters, (radix == 16 ? "%x" : "%d"), value);
	return buffer;
};

wchar_t* _itow_s(int value, wchar_t* buffer, size_t sizeInCharacters, int radix)
{
	assert(radix == 10 || radix == 16);
	swprintf(buffer, sizeInCharacters, (radix == 16 ? L"%x" : L"%d"), value);
	return buffer;
};

OLECHAR* _itow_s(int value, OLECHAR* buffer, size_t sizeInCharacters, int radix)
{
	wchar_t tmp[64]; // Should be big enough for any number
	_itow_s(value, tmp, sizeof(tmp) / sizeof(wchar_t), radix);

	UErrorCode status = U_ZERO_ERROR;
	u_strFromWCS(buffer, sizeInCharacters, 0, tmp, -1, &status);

	// Ensure null-termination, but only for genuine buffer sizes
	if (status == U_STRING_NOT_TERMINATED_WARNING ||
	    status == U_BUFFER_OVERFLOW_ERROR)
		buffer[sizeInCharacters-1] = 0;

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
	std::vector<wchar_t> buf(len + 1);	// Max required

	UErrorCode status = U_ZERO_ERROR;
	u_strToWCS(&buf[0], buf.size(), &len, str, len, &status);

	if (U_SUCCESS(status))
		std::wcerr.write(&buf[0], len);
	else
		std::wcerr << "ICU error " << status;

	std::wcerr.flush();
}

// TODO-P4CL23677-Merge
// TODO - maybe add assert if char* longer that sizeOfBuffer?
int sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...)
{
	va_list argList;
	va_start(argList, format);
	return vsprintf(buffer, format, argList);
	va_end(argList);
}

int _snprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, ...)
{
	va_list argList;
	va_start(argList, format);
	if (count == (size_t)_TRUNCATE)
		count = sizeOfBuffer;
	return vsnprintf(buffer, count, format, argList);
	va_end(argList);
}

// TODO return EINVAL on error
// return 0 on success
int wcscpy_s(OLECHAR *dst, const int size, const OLECHAR *src)
{
	u_strcpy(dst, src);
	return 0;
}

// returns 0 on success
int wcsncpy_s(OLECHAR* dst, const size_t dsize,
			const OLECHAR* src, const size_t size)
{
	if (!dst || dsize == 0)
		return EINVAL;
	dst[0] = 0;
	if (!src)
		return EINVAL;

	int nToCopy = size;
	if (size == (size_t)_TRUNCATE)
		nToCopy = std::min(u_strlen(src), (int)dsize - 1);
	else if (dsize <= size)
		return EINVAL;

	u_strncpy(dst, src, nToCopy);
	dst[nToCopy] = 0;
	return 0;
}

void InitializeCriticalSection(CRITICAL_SECTION*)
{
	// TODO-Linux
}

void DeleteCriticalSection(CRITICAL_SECTION*)
{
	// TODO-Linux
}

void EnterCriticalSection(CRITICAL_SECTION*)
{
	// TODO-Linux
}

void LeaveCriticalSection(CRITICAL_SECTION*)
{
	// TODO-Linux
}

