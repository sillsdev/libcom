/*
 *	$Id$
 *
 *	COM Support Library GUID manipulation
 *
 *	MarkS - 2007-02-26
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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

#include "Types.h"
#include "COM.h"
#include "WinError.h"
#include <vector>
#include <unicode/ustring.h>

/**
 * Convert a string representation of a classID to a binary classID GUID (in host byte order).
 * The string value should not be enclosed with {...}.
 * The spec says it could also return REGDB_E_CLASSNOTREG and REGDB_E_READREGDB, but we don't currently.
 * http://msdn2.microsoft.com/en-us/library/ms680589.aspx
 * @param inWideString input string to convert to a classID
 * @param outClassID output binary classID GUID based on inWideString
 * @return ERROR_SUCCESS on successful conversion, E_INVALIDARG if one or more invalid arguments, or CO_E_CLASSSTRING if the class string was not formatted correctly.
 */
#pragma export on
HRESULT WINAPI CLSIDFromString(LPCOLESTR inWideString, LPCLSID outClassID)
{
	if (!inWideString || !outClassID)
		return E_INVALIDARG;

	std::vector<char> text(inWideString, inWideString + u_strlen(inWideString) + 1);
	try
	{
		GUID clsid(&text[0]);
		*outClassID = clsid;
		return ERROR_SUCCESS;
	}
	catch (...)
	{
	}
	return CO_E_CLASSSTRING;
}
#pragma export off

/**
 * Convert binary classID GUID to string representation.
 * The string value will not be enclosed with {...}.
 * http://msdn2.microsoft.com/en-us/library/ms683917.aspx
 * @param inClassID input binary classID GUID to convert
 * @param outWideString output string based on inClassID
 * @return S_OK on successful conversion, E_OUTOFMEMORY if we run out of memory while creating the string, or (despite the spec) E_INVALIDARG if an invalid argument was given.
 */
#pragma export on
HRESULT StringFromCLSID(const CLSID &inClassID, LPOLESTR *outWideString)
{
	if (!outWideString) return E_INVALIDARG;
	
	std::string result = inClassID.str();

	// Allocate buffer for return value
	OLECHAR* buf = static_cast<OLECHAR*>(CoTaskMemAlloc((result.size() + 1) * sizeof(OLECHAR)));
	if (!buf)
		return E_OUTOFMEMORY;

	std::copy(result.begin(), result.end(), buf);

	*outWideString = buf;
	return S_OK;
}
#pragma export off
