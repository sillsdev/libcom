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
#include "WinError.h"

#include <uuid/uuid.h>
#include <arpa/inet.h>	// For htonl etc.

#include <algorithm>

// SmartGUID class

SmartGUID::SmartGUID(bool create)
{
	if (create)
	{
		uuid_generate(buf());

		Data1 = ntohl(Data1);
		Data2 = ntohs(Data2);
		Data3 = ntohs(Data3);
	}
	else
		uuid_clear(buf());
}

/**
 * @brief Create a SmartGUID from a text GUID string
 * @param text GUID string
 * @throw std::runtime_error if text could not be parsed
 */
SmartGUID::SmartGUID(const char* text) 
	throw (std::runtime_error)
{
	if (uuid_parse(text, buf()))
	{
		throw std::runtime_error("Failed to parse GUID string");
	}

	Data1 = ntohl(Data1);
	Data2 = ntohs(Data2);
	Data3 = ntohs(Data3);
}

std::string SmartGUID::str() const
{
	PlainGUID temp;
	temp.Data1 = htonl(Data1);
	temp.Data2 = htons(Data2);
	temp.Data3 = htons(Data3);
	memcpy(temp.Data4, Data4, sizeof(Data4));
	char text[37];
	uuid_unparse(reinterpret_cast<const unsigned char *>(&temp.Data1), text);
	return text;
}

bool SmartGUID::isNull() const
{
	// Assume null value is independent of byte order
	return !uuid_is_null(buf());
}

GUID GUID_NULL;

// Translation of IDs

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax)
{
	// PORT-TODO
	return 0; // Bytes used, including null terminator
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

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch)
{
	OLECHAR* buf = new OLECHAR[cch + 3];	// Space for length, data, terminator
	
	*(int*)buf = cch * sizeof(OLECHAR);	// Length (bytes)
	if (pch)
		memcpy(buf + 2, pch, *(int*)buf);	// Data
	else
		memset(buf + 2, 0, *(int*)buf);	// Data
	buf[2 + cch] = 0;						// Terminator
	
	return buf + 2;
}

// This function takes an ANSI string as input, and returns a BSTR that contains
//  an ANSI string. This function does not perform any ANSI to Unicode translation.
// If psz is NULL will returns uninitialized string.
// Allocates a new string of len bytes, copies len bytes from the passed string
//  into it, and then appends a null character. Valid only for 32-bit systems.
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len)
{
	OLECHAR* buf = new OLECHAR[len + 3];
	if (psz != NULL)
	{
	 	*(int*)buf = len;	
		memcpy(buf + 2, psz, len);
		buf[len] = NULL;
	}

	return buf + 2;
}

BSTR SysAllocString(const OLECHAR *sz) // Zero-terminated
{
	const OLECHAR* end = std::find(sz, sz + 99999, 0);
	
	return SysAllocStringLen(sz, end - sz);
}

void SysFreeString(BSTR bstr)
{
	if (bstr)
		delete [] (bstr - 2);
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

unsigned int SysStringLen(BSTR bstr)
{
	return *(unsigned int*)(bstr - 2);
}

// Class registration and object creation

HRESULT CoCreateGuid(GUID* pguid)
{
	*pguid = GUID(true);
	return S_OK;
}

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
