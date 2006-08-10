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

inline bool operator == (const GUID& guid1, const GUID& guid2)
{
	return std::memcmp(&guid1, &guid2, sizeof(guid1)) == 0;
}

typedef GUID IID;
typedef REFGUID REFIID;
typedef GUID CLSID;
typedef REFGUID REFCLSID;

extern GUID IID_NULL;

#define interface struct

#define __uuidof(X) IID_##X

#define ATTACH_GUID_TO_CLASS(type, guid, cls) \
	static GUID __uuidof(cls)(#guid);

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

BSTR SysAllocStringLen(const OLECHAR* pch, unsigned int cch);
BSTR SysAllocString(const OLECHAR *sz); // Zero-terminated
BSTR SysAllocStringByteLen(LPCSTR psz, UINT len);
void SysFreeString(BSTR bstr);

#endif //_COM_H_
