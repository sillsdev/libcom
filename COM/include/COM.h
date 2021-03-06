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

#ifndef _COM_H_
#define _COM_H_

#include "BasicTypes.h"

#include <cstring>		// For memcmp
#include <string>		// For GUID conversion
#include <stdexcept>

// GUID class

struct PlainGUID
{
	UINT32 			Data1; // 32 bit
	UINT16 			Data2; // 16 bit
	UINT16 			Data3; // 16 bit
	unsigned char   Data4[8];

	// No explicit constructors, to allow static initialization

	// Comparison

	int compare(const PlainGUID& other) const
	{
		return std::memcmp(this, &other, sizeof(other));
	}

	bool operator == (const PlainGUID& other) const
	{
		return compare(other) == 0;
	}

	bool operator != (const PlainGUID& other) const
	{
		return compare(other) != 0;
	}

	bool operator < (const PlainGUID& other) const
	{
		return compare(other) < 0;
	}

	bool operator > (const PlainGUID& other) const
	{
		return compare(other) > 0;
	}

	bool operator <= (const PlainGUID& other) const
	{
		return compare(other) <= 0;
	}

	bool operator >= (const PlainGUID& other) const
	{
		return compare(other) >= 0;
	}

	// Testing and conversion

	bool isNull() const;
	std::string str() const;

	// Creation and initialization

	PlainGUID& clear();
	PlainGUID& create();
	PlainGUID& initialize(const char*);

	// Allow easier use of libuuid functions

	unsigned char* buf()
	{
		return reinterpret_cast<unsigned char *>(&Data1);
	}
	const unsigned char* buf() const
	{
		return reinterpret_cast<const unsigned char *>(&Data1);
	}

	// Null value

	static PlainGUID null;
};

struct SmartGUID : public PlainGUID
{
	explicit SmartGUID(bool create = false);
	explicit SmartGUID(const char*);

	SmartGUID(const PlainGUID& p) : PlainGUID(p)
	{
	}

	SmartGUID& operator = (const PlainGUID& p)
	{
		PlainGUID::operator = (p);
		return *this;
	}
};

typedef PlainGUID GUID;

typedef const GUID& REFGUID;
typedef       GUID* LPGUID;

typedef GUID IID;
typedef REFGUID REFIID;
typedef LPGUID LPIID;
typedef GUID CLSID;
typedef REFGUID REFCLSID;
typedef LPGUID LPCLSID;

#define GUID_NULL GUID::null
#define IID_NULL GUID_NULL
#define CLSID_NULL GUID_NULL

HRESULT CoCreateGuid(GUID* pguid);

#define interface struct

template<class T>
class __interface_traits
{
public:
	static const GUID uuid;
};

#define __uuidof(T) __interface_traits<T>::uuid

#define DEFINE_UUIDOF(NAME, L, W1, W2, B1, B2, B3, B4, B5, B6, B7, B8) \
	class NAME; \
	template<> const GUID __uuidof(NAME) = {L, W1, W2, {B1, B2, B3, B4, B5, B6, B7, B8}}

#ifdef INITGUID
#define DEFINE_GUID(NAME, L, W1, W2, B1, B2, B3, B4, B5, B6, B7, B8) \
	extern const GUID NAME = {L, W1, W2, {B1, B2, B3, B4, B5, B6, B7, B8}}
#else
#define DEFINE_GUID(NAME, L, W1, W2, B1, B2, B3, B4, B5, B6, B7, B8) \
	extern const GUID NAME
#endif // INITGUID

#define DECLSPEC_UUID(X) // Can't be done
#define MIDL_INTERFACE(X) DECLSPEC_UUID(X) interface

// Translation of IDs

int StringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cchMax);

HRESULT StringFromCLSID(REFCLSID rclsid, LPOLESTR* lplpsz);
HRESULT CLSIDFromString(LPCOLESTR lpsz, LPCLSID pclsid);
HRESULT StringFromIID(REFIID rclsid, LPOLESTR* lplpsz);
HRESULT IIDFromString(LPCOLESTR lpsz, LPIID lpiid);
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
UINT SysStringLen(BSTR bstr);
UINT SysStringByteLen(BSTR bstr);

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

// These must be extern "C" because Mono needs to access them using dlsym
extern "C" HRESULT CoInitialize(LPVOID pvReserved);
extern "C" HRESULT CoCreateInstance(REFCLSID rclsid, class IUnknown* pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID FAR* ppv);

// Closes the COM Library on the current apartment.
extern "C" HRESULT CoUninitialize();

// Initizlizes the COM library from the current Apartment and sets the concurrency model as single-threaded apartment
extern "C" HRESULT OleInitialize(LPVOID pvReserved);

// Closes the COM library on the apartment.
extern "C" void	OleUninitialize();

extern "C" void CoFreeUnusedLibraries();

HRESULT CoRegisterClassObject(REFCLSID rclsid, class IUnknown* pObj, DWORD dwClsContext, DWORD flags, LPDWORD lpdwRegister);
HRESULT CoRevokeClassObject(DWORD dwRegister);

/**
 * @brief Get access to a class from the implementing DLL, such as a class's class factory
 * @param requestedClassID class ID of your desired class
 * @param requestedInterfaceID an interface ID that your desired class implements (such as IID_IClassFactory)
 * @param objectInterface interface to your desired class
 * @return S_OK upon success, E_OUTOFMEMORY upon running out of memory, E_NOINTERFACE if the class does not support the requested interface, CLASS_E_CLASSNOTAVAILABLE if this DLL doesn't support the requestedClassID
 *
 * http://msdn2.microsoft.com/en-us/library/ms680760.aspx
 */
EXTERN_C HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface);

#endif //_COM_H_
