/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupport.cp
Responsibility: Graeme Costin
Last reviewed: 2001-05-24, Neil Mayhew

	Support for COM in FieldWorks on MacOS.

Adjusted for GUID definitions from GUIDDEF.H
	2002-02-13, GDLC
Moved #include "debug.h" from COMSupport.h to this file
	2002-01-16, GDLC
Removed general items to WinSupport.cpp
	2001-07-17, GDLC
----------------------------------------------------------------------------------------------*/

#include "WinSupport.h"
#include "COMSupport.h"

#include "COMSupportInternals.h"

#include <cstdlib>
#include <cstring>
#include <cwctype>
#include <ostream>

#include "debug.h"

// No use for pguid in CoCreateGuid
#pragma warn_unusedarg off
//------------------------------------------------------------------
//	CoCreateGuid
//
//	Creates a new GUID.
//
//	At present this merely raises an assertion because it is
//	unlikely that FieldWorks will need to create GUIDs on the fly.
//------------------------------------------------------------------
HRESULT CoCreateGuid(GUID * pguid)
{
	Assert(false);
	return NULL;
}
#pragma warn_unusedarg reset

//------------------------------------------------------------------
//	CLSIDFromString
//
//	Takes a string representation of a Class ID and converts to the
//	binary GUID form. Places bytes in little endian positions.
//------------------------------------------------------------------
HRESULT CLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid)
{
	wchar_t	*p = lpsz;
	wchar_t	c;
	int		i, j, cd;
	union {
		GUID	clsid;
		char	cst[16];
	};
	char	*q = &cst[0];

	// Better test needed here?
	if (!lpsz || !pclsid) return E_INVALIDARG;

	// Consume open brace and any preceding characters
	while ( (c=*p++) != L'{') if (c==L'\0') return CO_E_CLASSSTRING;

	// Convert data for each of the 16 bytes of the GUID
	for (i=0; i<16; i++)
	{
		for (j=0, cd=0; j<2; j++)
		{
			if ( (c=*p++) == L'\0') return CO_E_CLASSSTRING;
			if (std::iswxdigit(c))
			{
				cd <<= 4;
				if (L'0' <= c && c <= L'9')
					cd += c - L'0';
				else
					cd += std::towupper(c) - ('A' - 10);
    		}
			else return CO_E_CLASSSTRING;
		}
		*q++ = cd;
		
		if (i==3 || i==5 || i== 7 || i==9)
		{
			if ( (c=*p++) != L'-') return CO_E_CLASSSTRING;
		}
	}
	if ( (c=*p) != L'}') return CO_E_CLASSSTRING;
	
	*pclsid = clsid;
	return ERROR_SUCCESS;
}

//------------------------------------------------------------------
//	StringFromCLSID
//
//	Takes a Class ID in binary GUID form and converts it to the string
//	representation.
//
//	NOTE: The Windows function allocates memory and calls StringFromGUID2()
//	but unless FieldWorks actually uses StringFromGUID2() we don't
//	need to implement it.
//------------------------------------------------------------------
HRESULT StringFromCLSID(CLSID &rclsid, LPOLESTR *ppsz)
{
	char	*clsid = reinterpret_cast<char*>(&rclsid);
	wchar_t	*buf, *q;
	int		c, i;
	static	wchar_t hex_digits[] = L"0123456789ABCDEF";

	// Better test needed here?
	if (!ppsz) return E_INVALIDARG;
	
	// Allocate buffer for string
	if ( !(buf=(static_cast <wchar_t *> (CoTaskMemAlloc(80))) )) return E_OUTOFMEMORY;
	q = buf;

	// Convert data for each of the 16 bytes of the GUID
	*q++ = L'{';
	for (i=0; i<16; i++)
	{
		c = *clsid++;
		*q++ = hex_digits[c >> 4];
		*q++ = hex_digits[c & 0xF];
		
		if (i==3 || i==5 || i== 7 || i==9)
		{
			*q++ = '-';
		}
	}
	*q++ = L'}'; *q = L'\0';
	*ppsz = buf;
	return S_OK;
}

//------------------------------------------------------------------
//	CoTaskMemAlloc
//
//	Allocates memory for COM objects.
//
//	TODO: This is defined as just malloc() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
void *	CoTaskMemAlloc(SIZE_T cb)
{
	return (std::malloc(cb));
}

//------------------------------------------------------------------
//	CoTaskMemRealloc
//
//	Reallocates memory for COM objects.
//
//	TODO: This is defined as just realloc() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
void *	CoTaskMemRealloc(LPVOID pv, SIZE_T cb)
{
	return (std::realloc(pv, cb));
}

//------------------------------------------------------------------
//	CoTaskMemFree
//
//	Frees memory from COM objects.
//
//	TODO: This is defined as just free() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
void	CoTaskMemFree(LPVOID pv)
{
	std::free(pv);
}

//------------------------------------------------------------------
//	GetMutableInstance
//
//	Returns a pointer to the registry.
//------------------------------------------------------------------
ComRegistry* ComRegistry::GetMutableInstance()
{
	static ComRegistry instance;
    return &instance;
}

//------------------------------------------------------------------
//	Register
//
//	Adds an entry to the registry for a Class and its factory pointer
//------------------------------------------------------------------
void ComRegistry::Register (const CLSID &Class, LPCLASSFACTORY Pointer)
{
	ComRegistry *r = ComRegistry::GetMutableInstance();
	(*r)[Class] = Pointer;
}

//------------------------------------------------------------------
//	GetFactoryPtr
//
//	Returns the factory pointer of the entry in the registry
//	for Class.
//------------------------------------------------------------------
HRESULT ComRegistry::GetFactoryPtr (const CLSID &Class, LPCLASSFACTORY* pIFactory) const
{
	const iterator where = ComRegistry::GetInstance()->find(Class);
	if (where != end())
	{
		*pIFactory = (*where).second;
		return S_OK;
	} else
	return REGDB_E_CLASSNOTREG;
}

// No use for dwClsContext yet in CoCreateInstance
#pragma warn_unusedarg off
//------------------------------------------------------------------
//	CoCreateInstance
//
//	Returns the factory pointer of the entry in the registry
//	for Class.
//------------------------------------------------------------------
static HRESULT CoCreateInstance (
	/*IN*/ CLSID		&rclsid,
	/*IN*/ LPUNKNOWN	pUnkOuter,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv)
{
	*ppv = NULL;

	IClassFactory* pIFactory = NULL;
	HRESULT hr = ComRegistry::GetInstance()->GetFactoryPtr (rclsid, &pIFactory);

	if (SUCCEEDED(hr))
	{
		// Create the component
		hr = pIFactory->CreateInstance(pUnkOuter, riid, ppv);

		// Release the class factory
		pIFactory->Release();
	}
	return hr;
}
#pragma warn_unusedarg reset

#ifdef DEBUG
//------------------------------------------------------------------
//	Dump
//
//	Dumps the contents of the registry onto the output stream out.
//------------------------------------------------------------------
void ComRegistry::Dump(std::ostream& out)
{
	ComRegistry* reg=ComRegistry::GetInstance();
	for (ComRegistry::const_iterator p = reg->begin(); p != reg->end(); ++p)
	{
		LPOLESTR c;
		
		GUID	g = p->first;
		HRESULT hr = StringFromCLSID(g, &c);
		
		if (SUCCEEDED(hr)) out << c;
		
		char ptrBuf[11];
		PtrToHex(p->second, &ptrBuf[0]);
		
		out << ptrBuf;
		
		CoTaskMemFree(c);
	}
}

//------------------------------------------------------------------
//	PtrToHex
//
//	Converts the Class Factory pointer Ptr to a hexadecimal
//	NUL terminated representation in the 11 byte buffer buf.
//------------------------------------------------------------------
static void PtrToHex(LPCLASSFACTORY Ptr, char *buf)
{
	static	wchar_t hex_digits[] = L"0123456789ABCDEF";
	char* p=buf;
	union {
		LPCLASSFACTORY	cp;
		unsigned long	ci;
	};
	
	cp = Ptr;
	
	*p++ = '0'; *p++ = 'x';
	for (int i=0; i<8; i++)
	{
		*p++ = hex_digits[(ci >> 28)];
		ci = ci << 4;
	}
	*p = '\0';
}
#endif	//	DEBUGCopyright 2001, SIL International. All rights reserved.

File: COMSupport.cp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			2001-05-24	Neil Mayhew

	$Id$

	Support for COM in FieldWorks on MacOS.

Fixed definition of first parameter to CoCreateInstance().
	2003-05-14, GDLC
Added RegisterServer() for registering class factories in our ComRegistry.
	2003-05-01, GDLC
Added CoGetClassObject() and changed CoCreateInstance() to use it.
	2003-04-21, GDLC
Fixed bug in StringFromCLSID (L'-' and missing unsigned)
	2003-04-14, GDLC
Added #pragma export on/off. Added ComRegistry constructor and destructor so that
linker would not object when linking the shared library.
	2002-12-12, GDLC
Created new project: COMSuppLibShared.mcp as a modified copy of WinSuppLibShared.mcp
Removed use of FieldWorks debug.h; added include of assert.h
	2002-12-09, GDLC
Adjusted for GUID definitions from GUIDDEF.H
	2002-02-13, GDLC
Moved #include "debug.h" from COMSupport.h to this file
	2002-01-16, GDLC
Removed general items to WinSupport.cpp
	2001-07-17, GDLC
----------------------------------------------------------------------------------------------*/

#include "WinSupport.h"
#include "COMSupport.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <cwctype>
#include <ostream>

#include "COMSupportInternals.h"

// No use for pguid in CoCreateGuid
#pragma warn_unusedarg off
//------------------------------------------------------------------
//	CoCreateGuid
//
//	Creates a new GUID.
//
//	At present this merely raises an assertion because it is
//	unlikely that FieldWorks will need to create GUIDs on the fly.
//------------------------------------------------------------------
#pragma export on
HRESULT CoCreateGuid(GUID * pguid)
{
	assert(false);
	return NULL;
}
#pragma export off
#pragma warn_unusedarg reset

//------------------------------------------------------------------
//	CLSIDFromString
//
//	Takes a string representation of a Class ID and converts to the
//	binary GUID form. Places bytes in little endian positions.
//------------------------------------------------------------------
#pragma export on
HRESULT CLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid)
{
	wchar_t	*p = lpsz;
	wchar_t	c;
	int		i, j, cd;
	union {
		GUID	clsid;
		char	cst[16];
	};
	char	*q = &cst[0];

	// Better test needed here?
	if (!lpsz || !pclsid) return E_INVALIDARG;

//	// Consume open brace and any preceding characters
//	while ( (c=*p++) != L'{') if (c==L'\0') return CO_E_CLASSSTRING;

	// Convert data for each of the 16 bytes of the GUID
	for (i=0; i<16; i++)
	{
		for (j=0, cd=0; j<2; j++)
		{
			if ( (c=*p++) == L'\0') return CO_E_CLASSSTRING;
			if (std::iswxdigit(c))
			{
				cd <<= 4;
				if (L'0' <= c && c <= L'9')
					cd += c - L'0';
				else
					cd += std::towupper(c) - ('A' - 10);
    		}
			else return CO_E_CLASSSTRING;
		}
		*q++ = static_cast<char> (cd);
		
		if (i==3 || i==5 || i== 7 || i==9)
		{
			if ( (c=*p++) != L'-') return CO_E_CLASSSTRING;
		}
	}
//	if ( (c=*p) != L'}') return CO_E_CLASSSTRING;
	
	*pclsid = clsid;
	return ERROR_SUCCESS;
}
#pragma export off

//------------------------------------------------------------------
//	StringFromCLSID
//
//	Takes a Class ID in binary GUID form and converts it to the string
//	representation.
//
//	NOTE: The Windows function allocates memory and calls StringFromGUID2()
//	but unless FieldWorks actually uses StringFromGUID2() we don't
//	need to implement it.
//------------------------------------------------------------------
#pragma export on
HRESULT StringFromCLSID(CLSID &rclsid, LPOLESTR *ppsz)
{
	unsigned char	*clsid = reinterpret_cast<unsigned char*>(&rclsid);
	wchar_t			*buf, *q;
	unsigned int	c, i;
	static wchar_t	hex_digits[] = L"0123456789ABCDEF";

	// Better test needed here?
	if (!ppsz) return E_INVALIDARG;
	
	// Allocate buffer for string
	if ( !(buf=(static_cast <wchar_t *> (CoTaskMemAlloc(80))) )) return E_OUTOFMEMORY;
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
	*ppsz = buf;
	return S_OK;
}
#pragma export off

//------------------------------------------------------------------
//	CoTaskMemAlloc
//
//	Allocates memory for COM objects.
//
//	TODO: This is defined as just malloc() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
#pragma export on
void *	CoTaskMemAlloc(SIZE_T cb)
{
	return (std::malloc(cb));
}
#pragma export off

//------------------------------------------------------------------
//	CoTaskMemRealloc
//
//	Reallocates memory for COM objects.
//
//	TODO: This is defined as just realloc() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
#pragma export on
void *	CoTaskMemRealloc(LPVOID pv, SIZE_T cb)
{
	return (std::realloc(pv, cb));
}
#pragma export off

//------------------------------------------------------------------
//	CoTaskMemFree
//
//	Frees memory from COM objects.
//
//	TODO: This is defined as just free() until a reason is found
//	for a more complex implementation.
//------------------------------------------------------------------
#pragma export on
void	CoTaskMemFree(LPVOID pv)
{
	std::free(pv);
}
#pragma export off

//------------------------------------------------------------------
//	ComRegistry creator
//
//	Creates an instance of ComRegistry.
//------------------------------------------------------------------
#pragma export on
ComRegistry::ComRegistry()
{
	// No explicit actions needed
}
#pragma export off

//------------------------------------------------------------------
//	ComRegistry destructor
//
//	Destroys an instance of ComRegistry.
//------------------------------------------------------------------
#pragma export on
ComRegistry::~ComRegistry()
{
	// No explicit actions needed
}
#pragma export off
	
//		~ComRegistry();
//------------------------------------------------------------------
//	GetMutableInstance
//
//	Returns a pointer to the registry.
//
//	NOTE: This is where the static instance of the COM registry is created.
//------------------------------------------------------------------
#pragma export on
ComRegistry* ComRegistry::GetMutableInstance()
{
	static ComRegistry instance;
    return &instance;
}
#pragma export off

//------------------------------------------------------------------
//	Register
//
//	Adds an entry to the registry for a Class and its factory pointer
//------------------------------------------------------------------
#pragma export on
void ComRegistry::Register (const CLSID &Class, LPCLASSFACTORY Pointer)
{
	ComRegistry *r = ComRegistry::GetMutableInstance();
	(*r)[Class] = Pointer;
}
#pragma export off

//------------------------------------------------------------------
//	GetFactoryPtr
//
//	Returns the factory pointer of the entry in the registry
//	for Class. If the class is not found, pIFactory is left unchanged.
//------------------------------------------------------------------
#pragma export on
HRESULT ComRegistry::GetFactoryPtr (const CLSID &Class, LPCLASSFACTORY* pIFactory) const
{
	const iterator where = ComRegistry::GetInstance()->find(Class);
	if (where != end())
	{
		*pIFactory = (*where).second;
		return S_OK;
	} else
	return REGDB_E_CLASSNOTREG;
}
#pragma export off

// No use for dwClsContext, pvReserved or riid yet in CoGetClassObject
#pragma warn_unusedarg off
//------------------------------------------------------------------
//	CoGetClassObject
//
//	Returns the pointer to the IClassFactory interface of the entry
//	in the registry for the class rclsid.
//
//	NOTE: The caller is responsible for releasing the class factory.
//
//	NOTE: This implementation is intended for use for inprocess COM only.
//------------------------------------------------------------------
#pragma export on
HRESULT CoGetClassObject(
	/*IN*/ REFCLSID		rclsid,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ LPVOID		pvReserved,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv)
{
	assert(dwClsContext == CLSCTX_INPROC);

	IClassFactory* pIFactory = NULL;
	HRESULT	hr = ComRegistry::GetInstance()->GetFactoryPtr (rclsid, &pIFactory);

	*ppv = pIFactory;
	return hr;
}
#pragma export off
#pragma warn_unusedarg reset

// No use for dwClsContext yet in CoCreateInstance
#pragma warn_unusedarg off
//------------------------------------------------------------------
//	CoCreateInstance
//
//	Returns an instance of the class rclsid after obtaining the factory
//	pointer of the entry in the registry for Class.
//
//	NOTE: This implementation is intended for use for inprocess COM only.
//------------------------------------------------------------------
#pragma export on
HRESULT CoCreateInstance (
	/*IN*/ REFCLSID		rclsid,
	/*IN*/ LPUNKNOWN	pUnkOuter,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv)
{
	*ppv = NULL;

	IClassFactory* pIFactory = NULL;
	HRESULT hr = CoGetClassObject(rclsid,
						static_cast <unsigned long> (CLSCTX_INPROC),
						(void *)0,
						IID_IClassFactory,
						(void **)&pIFactory);

	if (SUCCEEDED(hr))
	{
		// Create the component
		hr = pIFactory->CreateInstance(pUnkOuter, riid, ppv);

		// Release the class factory
		pIFactory->Release();
	}
	return hr;
}
#pragma export off
#pragma warn_unusedarg reset

//------------------------------------------------------------------
//
//	Register Server
#pragma export on
void RegisterServer(const CLSID &Class, LPCLASSFACTORY Pointer)
{
	ComRegistry::GetInstance()->Register (Class, Pointer);
}
#pragma export off

#ifdef DEBUG
//------------------------------------------------------------------
//	Dump
//
//	Dumps the contents of the registry onto the output stream out.
//------------------------------------------------------------------
#pragma export on
void ComRegistry::Dump(std::ostream& out)
{
	ComRegistry* reg=ComRegistry::GetInstance();
	for (ComRegistry::const_iterator p = reg->begin(); p != reg->end(); ++p)
	{
		LPOLESTR c;
		
		GUID	g = p->first;
		HRESULT hr = StringFromCLSID(g, &c);
		
		if (SUCCEEDED(hr)) out << c;
		
		char ptrBuf[11];
		PtrToHex(p->second, &ptrBuf[0]);
		
		out << ptrBuf;
		
		CoTaskMemFree(c);
	}
}
#pragma export off

//------------------------------------------------------------------
//	PtrToHex
//
//	Converts the Class Factory pointer Ptr to a hexadecimal
//	NUL terminated representation in the 11 byte buffer buf.
//------------------------------------------------------------------
#pragma export on
static void PtrToHex(LPCLASSFACTORY Ptr, char *buf)
{
	static	wchar_t hex_digits[] = L"0123456789ABCDEF";
	char* p=buf;
	union {
		LPCLASSFACTORY	cp;
		unsigned long	ci;
	};
	
	cp = Ptr;
	
	*p++ = '0'; *p++ = 'x';
	for (int i=0; i<8; i++)
	{
		*p++ = hex_digits[(ci >> 28)];
		ci = ci << 4;
	}
	*p = '\0';
}
#pragma export off
#endif	//	DEBUG