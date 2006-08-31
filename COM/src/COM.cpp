/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#include "COM.h"
#include "WinError.h"

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

GUID GUID_NULL;
GUID GUID_ITextStoreACPSink;
GUID GUID_ITextStoreACPServices;
GUID GUID_ITextStoreACP;
GUID GUID_ITfRangeACP;
GUID GUID_ITfContextOwnerCompositionServices;
GUID GUID_ITfContextOwnerCompositionSink;
GUID GUID_ITfMouseTrackerACP;
GUID GUID_PROP_ATTRIBUTE;

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax)
{
	// ToDo
	return 0; // Bytes used, including null terminator
}

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

HRESULT ProgIDFromCLSID(REFCLSID clsid, LPOLESTR* lplpszProgID)
{
	// ToDo
	return S_OK;
}

HRESULT CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid)
{
	// ToDo
	return S_OK;
}

void* CoTaskMemAlloc(size_t cb)
{
	return malloc(cb);
}

void* CoTaskMemRealloc(void* pv, size_t cb)
{
	realloc(pv, cb);
}

void  CoTaskMemFree(void* pv)
{
	free(pv);
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
