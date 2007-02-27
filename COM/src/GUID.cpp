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

/**
 * Convert a string representation of a classID to a binary classID GUID (in little endian?).
 * The spec says it could also return REGDB_E_CLASSNOTREG and REGDB_E_READREGDB, but we don't currently.
 * http://msdn2.microsoft.com/en-us/library/ms680589.aspx
 * @param inWideString input string to convert to a classID
 * @param outClassID output binary classID GUID based on inWideString
 * @return ERROR_SUCCESS on successful conversion, E_INVALIDARG if one or more invalid arguments, or CO_E_CLASSSTRING if the class string was not formatted correctly.
 */
#pragma export on
HRESULT WINAPI CLSIDFromString(LPCOLESTR inWideString, LPCLSID outClassID)
{
	const OLECHAR *p = inWideString;
	wchar_t wc;
	char	c;
	int		i, j, cd;
	union {
		PlainGUID clsid;
		char      cst[16];
	};
	char	*q = &cst[0];

	// Better test needed here?
	if (!inWideString || !outClassID) return E_INVALIDARG;

//TODO	// Consume open brace and any preceding characters
//	while ( (wc=*p++) != L'{') if (wc==L'\0') return CO_E_CLASSSTRING;

	// Convert data for each of the 16 bytes of the GUID
	for (i=0; i<16; i++)
	{
		for (j=0, cd=0; j<2; j++)
		{
			wc = *p++;
			c = char(wc);
			if ( wc == L'\0') return CO_E_CLASSSTRING;
			if ( wc != c ) return CO_E_CLASSSTRING;
//			if (std::iswxdigit(c))
			if (std::isxdigit(c))
			{
				cd <<= 4;
				if ('0' <= c && c <= '9')
					cd += c - '0';
				else
//					cd += std::towupper(c) - ('A' - 10);
					cd += std::toupper(c) - ('A' - 10);
    		}
			else return CO_E_CLASSSTRING;
		}
		*q++ = char(cd);
		
		if (i==3 || i==5 || i== 7 || i==9)
		{
			if ( (wc=*p++) != L'-') return CO_E_CLASSSTRING;
		}
	}
	
	*outClassID = clsid;
	return ERROR_SUCCESS;
}
#pragma export off

/**
 * Convert binary classID GUID to string representation.
 * http://msdn2.microsoft.com/en-us/library/ms683917.aspx
 * @param inClassID input binary classID GUID to convert
 * @param outWideString output string based on inClassID
 * @return S_OK on successful conversion, E_OUTOFMEMORY if we run out of memory while creating the string, or (despite the spec) E_INVALIDARG if an invalid argument was given.
 */
#pragma export on
HRESULT StringFromCLSID(const CLSID &inClassID, LPOLESTR *outWideString)
{
	// NOTE: The Windows function allocates memory and calls StringFromGUID2()
	// but we're not implementing StringFromGUID2().
	const unsigned char	*clsid = reinterpret_cast<const unsigned char*>(&inClassID);
	OLECHAR			*buf, *q;
	unsigned int	c, i;
	static wchar_t	hex_digits[] = L"0123456789ABCDEF";

	// Better test needed here?
	if (!outWideString) return E_INVALIDARG;
	
	// Allocate buffer for string
	if (!(buf = static_cast<OLECHAR*>(CoTaskMemAlloc(37 * sizeof(OLECHAR)))))
		return E_OUTOFMEMORY;
	q = buf;

	// Convert data for each of the 16 bytes of the GUID
	for (i=0; i<16; i++)
	{
		c = *clsid++;
		*q++ = hex_digits[c >> 4];
		*q++ = hex_digits[c & 0xF];
		
		if (i==3 || i==5 || i== 7 || i==9)
		{
			*q++ = L'-';
		}
	}
	*q = L'\0';
	*outWideString = buf;
	return S_OK;
}
#pragma export off
