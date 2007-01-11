/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - Apr 22, 2005
 *
 *	Most of this functionality will eventually come from COMSupportLibrary
 */

#ifndef _COM_H_
#define _COM_H_

#include "Types.h"

#include <cstring>	// For memcmp

// GUID class

struct GUID
{
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
	
	// These are not in Win32, but we need them to make our emulation work
	explicit GUID(bool create = false);
	explicit GUID(const char*);
	
	// Casting functions to allow easier use of libuuid functions
	operator unsigned char* ()
	{
		return reinterpret_cast<unsigned char *>(&Data1);
	}
	operator const unsigned char* () const
	{
		return reinterpret_cast<const unsigned char *>(&Data1);
	}
	
	operator bool () const;
};

inline bool operator == (const GUID& guid1, const GUID& guid2)
{
	return std::memcmp(&guid1, &guid2, sizeof(guid1)) == 0;
}

inline bool operator != (const GUID& guid1, const GUID& guid2)
{
	return !(guid1 == guid2);
}

typedef const GUID& REFGUID;
typedef const GUID* LPGUID;

typedef GUID IID;
typedef REFGUID REFIID;
typedef LPGUID LPIID;
typedef GUID CLSID;
typedef REFGUID REFCLSID;
typedef LPGUID LPCLSID;

extern GUID GUID_NULL;
#define IID_NULL GUID_NULL

inline HRESULT CoCreateGuid(GUID* pguid)
{
	*pguid = GUID(true);
}

#define interface struct

template<class T>
class __interface_traits
{
public:
	static const GUID uuid;
};

#define __uuidof(T) __interface_traits<T>::uuid

#define DECLSPEC_UUID(X) // Can't be done
#define MIDL_INTERFACE(X) DECLSPEC_UUID(X) interface

// Translation of IDs

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax);

HRESULT StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz);
HRESULT CLSIDFromString(const LPOLESTR lpsz, LPCLSID pclsid);
HRESULT StringFromIID(REFIID rclsid, LPOLESTR* lplpsz);
HRESULT IIDFromString(const LPOLESTR lpsz, LPIID lpiid);
HRESULT ProgIDFromCLSID(REFCLSID clsid, LPOLESTR* lplpszProgID);
HRESULT CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid);

// Memory allocation

void* CoTaskMemAlloc(size_t cb);
void* CoTaskMemRealloc(void* pv, size_t cb);
void  CoTaskMemFree(void* pv);

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch);
BSTR SysAllocString(const OLECHAR *sz); // Zero-terminated
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len);
void SysFreeString(BSTR bstr);
int SysReAllocString(BSTR* pbstr, const OLECHAR* psz);
int SysReAllocStringLen(BSTR* pbstr, const OLECHAR* pch, unsigned int cch);

// Class registration and object creation

enum CLSCTX
{
	CLSCTX_INPROC_SERVER  = 1,
	CLSCTX_INPROC_HANDLER = 2,
	CLSCTX_LOCAL_SERVER   = 4,
	CLSCTX_REMOTE_SERVER  = 8,
};

#define CLSCTX_INPROC (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)
#define CLSCTX_SERVER (CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)
#define CLSCTX_ALL    (CLSCTX_INPROC|CLSCTX_SERVER)

enum REGCLS
{
	REGCLS_SINGLEUSE      = 0,
	REGCLS_MULTIPLEUSE    = 1,
	REGCLS_MULTI_SEPARATE = 2,
	REGCLS_SUSPENDED      = 4,
	REGCLS_SURROGATE      = 8,
};

HRESULT CoCreateInstance(REFCLSID rclsid, void* pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv); 
HRESULT CoRegisterClassObject(REFCLSID rclsid, class IUnknown* pObj, DWORD dwClsContext, DWORD flags, LPDWORD lpdwRegister);
HRESULT CoRevokeClassObject(DWORD dwRegister);

#endif //_COM_H_
