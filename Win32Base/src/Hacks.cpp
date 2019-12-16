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

#include "Hacks.h"
#include "WinError.h"

#include <cwchar>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <stdarg.h>

#include <unicode/ustring.h>

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
	u_strToUTF8(dst, dstLen, &spaceRequiredForData, (UChar*)src, srcLen, &status);

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
	u_strFromUTF8((UChar*)dst, dstLen, &spaceRequiredForData, src, srcLen, &status);

	if (U_FAILURE(status) && status != U_BUFFER_OVERFLOW_ERROR)
		throw std::runtime_error("Unable to convert from UTF-8 to UTF-16");

	return spaceRequiredForData;
}

static bool g_ptrChecking = getenv("DEBUG_ENABLE_PTR_VALIDATION") != NULL;

// Attempt to read every byte in the lp array.
// will seg fault if bad.
// catching segv with signal interferes with mono
bool __IsBadReadPtr(const void* lp, UINT cb)
{
	if (!g_ptrChecking)
		return false;

	if (!cb)
		return false;
	if (!lp)
		return true;

	UINT i;
	volatile BYTE b1 __attribute((unused));

	for (i = 0; i < cb; i++)
		b1 = ((BYTE*)lp)[i];

	return false;
}

// return true if lp is not valid for reading.
bool __IsBadCodePtr(const void* lp)
{
	if (!g_ptrChecking)
		return false;

	return __IsBadReadPtr(lp, 1);
}

// attempt to |= 0 every byte in lp array
// will seg fault if bad.
// catching segv with signal interferes with mono
bool __IsBadWritePtr(const void *lp, UINT cb)
{
	if (!g_ptrChecking)
		return false;

	if (!cb)
		return false;
	if (!lp)
		return true;

	UINT i;

	for (i = 0; i < cb; i++)
		((BYTE*)lp)[i] |= 0;

	return false;
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
	u_strFromWCS((UChar*)buffer, sizeInCharacters, 0, tmp, -1, &status);

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
	int32_t len = wcslen(str);
	std::vector<OLECHAR> buf(len*2 + 1); // Max required (surrogates == 2)

	UErrorCode status = U_ZERO_ERROR;
	u_strFromWCS((UChar*)&buf[0], buf.size(), &len, str, len, &status);

	if (U_SUCCESS(status))
		OutputDebugString(&buf[0]);
	else
	{
		std::cerr << "ICU error " << status;
		std::cerr.flush();
	}
}

void OutputDebugString(const OLECHAR* str)
{
	int32_t len = u_strlen((UChar*)str);
	std::vector<char> buf(len*3 + 1); // Max required (BMP <= 3, surrogates <= 6)

	UErrorCode status = U_ZERO_ERROR;
	u_strToUTF8(&buf[0], buf.size(), &len, (UChar*)str, len, &status);

	if (U_SUCCESS(status))
		std::cerr.write(&buf[0], len);
	else
		std::cerr << "ICU error " << status;

	std::cerr.flush();
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
	u_strcpy((UChar*)dst, (UChar*)src);
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
		nToCopy = std::min(u_strlen((UChar*)src), (int)dsize - 1);
	else if (dsize <= size)
		return EINVAL;

	u_strncpy((UChar*)dst, (UChar*)src, nToCopy);
	dst[nToCopy] = 0;
	return 0;
}
