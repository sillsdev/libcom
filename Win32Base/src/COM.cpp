/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#include "COM.h"

#include <uuid/uuid_types.h>
#include <uuid/uuid.h>

#include <algorithm>

GUID::GUID()
{
	memset(this, 0, sizeof(*this));
}

GUID::GUID(const char* text)
{
	uuid_parse(text, reinterpret_cast<unsigned char *>(&Data1));
}

GUID IID_NULL;

HRESULT StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz)
{
	// ToDo
	return S_OK;
}

HRESULT CLSIDFromString(const LPOLESTR lpsz, LPCLSID pclsid)
{
	// ToDo
	return S_OK;
}

HRESULT StringFromIID(REFIID rclsid, LPOLESTR* lplpsz)
{
	// ToDo
	return S_OK;
}

HRESULT IIDFromString(const LPOLESTR lpsz, LPIID lpiid)
{
	// ToDo
	return S_OK;
}

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch)
{
	OLECHAR* buf = new OLECHAR[cch + 3]; // Space for length, data, terminator
	
	*(int*)buf = cch * sizeof(OLECHAR); // Length (bytes)
	memcpy(buf + 2, pch, *(int*)buf);   // Data
	buf[2 + cch] = 0;					// Terminator
	
	return buf + 2;
}

BSTR SysAllocString(const OLECHAR *sz) // Zero-terminated
{
	const OLECHAR* end = std::find(sz, sz + 99999, 0);
	
	return SysAllocStringLen(sz, end - sz);
}

void SysFreeString(BSTR bstr)
{
	delete [] (bstr - 2);
}
