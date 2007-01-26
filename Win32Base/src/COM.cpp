/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - Apr 22, 2005
 *
 *	Most of this functionality will eventually come from COMSupportLibrary
 */

#include "COM.h"
#include "WinError.h"

#include <uuid/uuid.h>

#include <algorithm>

// SmartGUID class

SmartGUID::SmartGUID(bool create)
{
	if (create)
		uuid_generate(buf());
	else
		uuid_clear(buf());
}

SmartGUID::SmartGUID(const char* text)
{
	uuid_parse(text, buf());
}

std::string SmartGUID::str() const
{
	char text[37];
	uuid_unparse(buf(), text);
	return text;
}

bool SmartGUID::isNull() const
{
	return !uuid_is_null(buf());
}

GUID GUID_NULL;

// Translation of IDs

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax)
{
	// PORT-TODO
	return 0; // Bytes used, including null terminator
}

HRESULT StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz)
{
	// PORT-TODO
	return E_NOTIMPL;
}

HRESULT CLSIDFromString(const LPOLESTR lpsz, LPCLSID pclsid)
{
	// PORT-TODO
	return E_NOTIMPL;
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
	OLECHAR* buf = new OLECHAR[cch + 3];	// Space for length, data, terminator
	
	*(int*)buf = cch * sizeof(OLECHAR);	// Length (bytes)
	if (pch)
		memcpy(buf + 2, pch, *(int*)buf);	// Data
	else
		memset(buf + 2, 0, *(int*)buf);	// Data
	buf[2 + cch] = 0;						// Terminator
	
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

// Class registration and object creation

HRESULT CoCreateGuid(GUID* pguid)
{
	*pguid = GUID(true);
	return S_OK;
}

HRESULT CoCreateInstance(REFCLSID rclsid, void* pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv)
{
	// PORT-TODO
	return E_NOTIMPL;
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
