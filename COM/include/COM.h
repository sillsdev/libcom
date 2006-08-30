/*
 *	$Id$
 *
 *	Definitions and declarations to support COM programming
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#ifndef _COM_H_
#define _COM_H_

#include "Types.h"

#include <cstring>

struct GUID
{
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
	
	// These are not in Win32, but we need them to make our emulation work
	GUID();
	GUID(const char*);
};

typedef const GUID& REFGUID;
typedef const GUID* LPGUID;

inline bool operator == (const GUID& guid1, const GUID& guid2)
{
	return std::memcmp(&guid1, &guid2, sizeof(guid1)) == 0;
}

inline bool operator != (const GUID& guid1, const GUID& guid2)
{
	return !(guid1 == guid2);
}

typedef GUID IID;
typedef REFGUID REFIID;
typedef LPGUID LPIID;
typedef GUID CLSID;
typedef REFGUID REFCLSID;
typedef LPGUID LPCLSID;

extern GUID GUID_NULL;
#define IID_NULL GUID_NULL
extern GUID GUID_ITextStoreACPSink;
#define IID_ITextStoreACPSink GUID_ITextStoreACPSink
extern GUID GUID_ITextStoreACPServices;
#define IID_ITextStoreACPServices GUID_ITextStoreACPServices
extern GUID GUID_ITextStoreACP;
#define IID_ITextStoreACP GUID_ITextStoreACP
extern GUID GUID_ITfRangeACP;
#define IID_ITfRangeACP GUID_ITfRangeACP
extern GUID GUID_ITfContextOwnerCompositionServices;
#define IID_ITfContextOwnerCompositionServices GUID_ITfContextOwnerCompositionServices
extern GUID GUID_ITfContextOwnerCompositionSink;
#define IID_ITfContextOwnerCompositionSink GUID_ITfContextOwnerCompositionSink
extern GUID GUID_ITfMouseTrackerACP;
#define IID_ITfMouseTrackerACP GUID_ITfMouseTrackerACP
extern GUID GUID_PROP_ATTRIBUTE;
#define IID_PROP_ATTRIBUTE GUID_PROP_ATTRIBUTE

#define interface struct

template<class T>
class __interface_traits
{
public:
	static GUID uuid;
};

#define __uuidof(T) __interface_traits<T>::uuid

#define DECLSPEC_UUID(X) // Can't be done
#define MIDL_INTERFACE(X) DECLSPEC_UUID(X) interface

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

HRESULT CoCreateInstance(REFCLSID rclsid, void* pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv); 

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax);

HRESULT StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz);
HRESULT CLSIDFromString(const LPOLESTR lpsz, LPCLSID pclsid);
HRESULT StringFromIID(REFIID rclsid, LPOLESTR* lplpsz);
HRESULT IIDFromString(const LPOLESTR lpsz, LPIID lpiid);
HRESULT ProgIDFromCLSID(REFCLSID clsid, LPOLESTR* lplpszProgID);
HRESULT CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid);

void* CoTaskMemAlloc(size_t cb);
void* CoTaskMemRealloc(void* pv, size_t cb);
void  CoTaskMemFree(void* pv);

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch);
BSTR SysAllocString(const OLECHAR *sz); // Zero-terminated
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len);
void SysFreeString(BSTR bstr);

#endif //_COM_H_
