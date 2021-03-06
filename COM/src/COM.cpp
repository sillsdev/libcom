/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - 2005-04-22
 *
 * COM Support Library
 * Copyright (C) 2007 SIL International
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "COM.h"
#include "COMError.h"
#include "COMSupportInternals.h"

#include <uuid/uuid.h>
#include <arpa/inet.h>	// For htonl etc.

#include <algorithm>
#include <cassert>

// PlainGUID class

/**
 * @brief Test whether the GUID value is null
 */
bool PlainGUID::isNull() const
{
	// Assume null value is independent of byte order
	return uuid_is_null(buf());
}

/**
 * @brief Convert the GUID value to to a string
 */
std::string PlainGUID::str() const
{
	PlainGUID temp(*this);
	// Convert from host byte order
	temp.Data1 = htonl(temp.Data1);
	temp.Data2 = htons(temp.Data2);
	temp.Data3 = htons(temp.Data3);
	char text[37];
	uuid_unparse(temp.buf(), text);
	return text;
}

/**
 * @brief Set the GUID value to null
 */
PlainGUID& PlainGUID::clear()
{
	uuid_clear(buf());
	return *this;
}

/**
 * @brief Set the GUID value to a newly generated one
 */
PlainGUID& PlainGUID::create()
{
	uuid_generate(buf());

	// Convert to host byte order
	Data1 = ntohl(Data1);
	Data2 = ntohs(Data2);
	Data3 = ntohs(Data3);

	return *this;
}

/**
 * @brief Create a SmartGUID from a text GUID string
 * @param text GUID string
 * @throw std::runtime_error if text could not be parsed
 */
PlainGUID& PlainGUID::initialize(const char* text)
{
	if (uuid_parse(text, buf()))
	{
		std::string s("Failed to parse GUID string: ");
		s+= text;
		throw std::runtime_error(s);
	}

	// Convert to host byte order
	Data1 = ntohl(Data1);
	Data2 = ntohs(Data2);
	Data3 = ntohs(Data3);

	return *this;
}

// SmartGUID class

/**
 * @brief Create a SmartGUID
 * @param creating Generate a new GUID instead of initializing to null
 */
SmartGUID::SmartGUID(bool creating)
{
	if (creating)
		create();
	else
		clear();
}

/**
 * @brief Create a SmartGUID from a text GUID string
 * @param text GUID string
 * @throw std::runtime_error if text could not be parsed
 */
SmartGUID::SmartGUID(const char* text)
{
	initialize(text);
}

// GUID values

PlainGUID PlainGUID::null;

HRESULT CoCreateGuid(GUID* pguid)
{
	pguid->create();
	return S_OK;
}

// Translation of IDs

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax)
{
	assert(lpsz);

	std::string text = rguid.str();

	if (cchMax <= int(text.size()))
		return 0;

	std::copy(text.begin(), text.end(), lpsz);
	lpsz[text.size()] = 0;

	return text.size() + 1; // Characters, including null terminator
}

HRESULT StringFromIID(REFIID rclsid, LPOLESTR* lplpsz)
{
	// PORT-TODO
	return E_NOTIMPL;
}

HRESULT IIDFromString(const LPOLESTR lpsz, LPIID lpiid)
{
	// PORT-TODO
	return E_NOTIMPL;
}

HRESULT ProgIDFromCLSID(REFCLSID clsid, LPOLESTR* lplpszProgID)
{
	// PORT-TODO
	return E_NOTIMPL;
}

HRESULT CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid)
{
	// PORT-TODO
	return E_NOTIMPL;
}

// Memory allocation
OLECHAR* Alloc(int nOleChars)
{
	// NOTE: we better use malloc/free for allocating the memory since this is what Mono does,
	// and this string might be freed by managed code.
	return (OLECHAR*)malloc(nOleChars * sizeof(OLECHAR));
}

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch)
{
	//	OLECHAR* buf = new OLECHAR[cch + 3];	// Space for length, data, terminator.
	//		// Note: this allocates 2 * (cch + 3) bytes if sizeof(OLECHAR) == 2
	OLECHAR* buf = Alloc(cch + 3); // Space for length, data, terminator.

	*(INT32*)buf = cch * sizeof(OLECHAR);	// Length (bytes)
	if (pch)
		memcpy(buf + 2, pch, *(INT32*)buf);	// Data
	else
		memset(buf + 2, 0, *(INT32*)buf);	// Data
	buf[2 + cch] = 0;						// Terminator

	return buf + 2;
}

/**
 * @brief Create an ANSI BSTR from an ANSI char*
 * Note that there is no ANSI to Unicode translation.
 * This implementation will work only for little-endian systems.
 * @param psz input null-terminated string. If NULL, the BSTR string will be uninitialized.
 * @param len Number of bytes to copy from input string to output string. Most likely should be the length of psz, not including null-terminator.
 * @return pointer to the BSTR string, or NULL if not enough memory. The BSTR string will have len bytes copied from psz as well as a null-terminator appended.
 */
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len)
{
	OLECHAR* buf = Alloc(len + 3);	// Space for length, data, terminator.
		// Note: this allocates 2 * (len + 3) bytes if sizeof(OLECHAR) == 2

	if (NULL == buf)
		return NULL; // insufficient memory

	if (psz != NULL)
	{
		*(INT32*)buf = len;
		memcpy(buf + 2, psz, len);
		((char*)(buf + 2))[len] = 0;
	}

	return buf + 2;
}

BSTR SysAllocString(const OLECHAR *sz) // Zero-terminated
{
	return SysAllocStringLen(sz, wcslen(sz));
}

void SysFreeString(BSTR bstr)
{
	if (bstr)
	{
		// NOTE: we better use malloc/free for allocating the memory since this is what Mono does,
		// and this string might have been allocated by managed code.
		//delete [] (bstr - 2);
		free(bstr - 2);
	}
}

int SysReAllocString(BSTR* pbstr, const OLECHAR* psz)
{
	BSTR tmp = SysAllocString(psz);
	if (!tmp)
		return false;
	std::swap(tmp, *pbstr);
	SysFreeString(tmp);
	return true;
}

int SysReAllocStringLen(BSTR* pbstr, const OLECHAR* pch, unsigned int cch)
{
	BSTR tmp = SysAllocStringLen(pch, cch);
	if (!tmp)
		return false;
	std::swap(tmp, *pbstr);
	SysFreeString(tmp);
	return true;
}

UINT SysStringLen(BSTR bstr)
{
	if (bstr == NULL)
		return 0;

	return SysStringByteLen(bstr) / sizeof(OLECHAR);
}

UINT SysStringByteLen(BSTR bstr)
{
	assert(bstr != NULL);
	return *(UINT32*)(bstr - 2);
}

// Class registration

HRESULT CoRegisterClassObject(REFCLSID rclsid, class IUnknown* pObj, DWORD dwClsContext, DWORD flags, LPDWORD lpdwRegister)
{
	// PORT-TODO
	return E_NOTIMPL;
}

HRESULT CoRevokeClassObject(DWORD dwRegister)
{
	// PORT-TODO
	return E_NOTIMPL;
}
