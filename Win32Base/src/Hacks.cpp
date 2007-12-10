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

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cwchar>
#include <cassert>
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

bool IsBadStringPtrW(const OLECHAR*, unsigned long) { return false; }
bool IsBadStringPtrA(const char*, unsigned long) { return false; }
bool IsBadReadPtr(const void*, unsigned long) { return false; }
bool IsBadWritePtr(const void*, unsigned long) { return false; }

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
	u_strFromWCS(buffer, bufferSize, 0, tmp, -1, &status);

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
	std::vector<wchar_t> buf(len + 1);	// Max required

	UErrorCode status = U_ZERO_ERROR;
	u_strToWCS(&buf[0], buf.size(), &len, str, len, &status);

	if (U_SUCCESS(status))
		std::wcerr.write(&buf[0], len);
	else
		std::wcerr << "ICU error " << status;

	std::wcerr.flush();
}

// Support functions

// Temporary, until we find a way of doing this properly on Unix

#include <cwchar>
#include <vector>

static wchar_t module_name[] = L"Unknown Module";

unsigned long GetModuleFileName(HMODULE, wchar_t* buf, unsigned long max)
{
	std::wcsncpy(buf, module_name, max);
	buf[max - 1] = 0;
	return std::wcslen(buf);
}

unsigned long GetModuleFileName(HMODULE h, OLECHAR* buf, unsigned long max)
{
	std::vector<wchar_t> tmp(max);
	unsigned long count = GetModuleFileName(h, &tmp[0], max);
	std::copy(tmp.begin(), tmp.end(), buf);
	return count;
}
