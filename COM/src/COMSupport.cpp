/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupport.cpp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			2001-05-24	Neil Mayhew

	$Id$

	Support for COM in Linux
	
Implemented a DLL Map. Added CoInitialize. Made CoGetClassObject AddRef to the factory to prevent a crash. And other little changes. 
	2006-12   , MarkS
Adapted for command line builds with gcc, removed Carbon dependencies and ported to Linux.
Clean up ready for checking in.
	2003-06-24, NJHM, GDLC
Modified CLSIDFromString() to avoid use of wide character functions
	2003-06-21, NJHM
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
#include <iostream>
#include <cctype>

#include "COMSupportInternals.h"

#include <map>
#include <dlfcn.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

#include <stdexcept>

//------------------------------------------------------------------
//	CoCreateGuid
//
//	Creates a new GUID.
//
//	At present this merely raises an assertion because it is
//	unlikely that FieldWorks will need to create GUIDs on the fly.
//------------------------------------------------------------------
#pragma export on
HRESULT CoCreateGuid(GUID * /*pguid*/)
{
	assert(false);
	return 0;
}
#pragma export off

//------------------------------------------------------------------
//	CLSIDFromString
//
//	Takes a const string representation of a Class ID and converts to the
//	binary GUID form. Places bytes in little endian positions.
//------------------------------------------------------------------
#pragma export on
HRESULT CLSIDFromString(LPCOLESTR lpsz, LPCLSID pclsid)
{
	const wchar_t	*p = lpsz;
	wchar_t wc;
	char	c;
	int		i, j, cd;
	union {
		GUID	clsid;
		char	cst[16];
	};
	char	*q = &cst[0];

	// Better test needed here?
	if (!lpsz || !pclsid) return E_INVALIDARG;

//	// Consume open brace and any preceding characters
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
	
	*pclsid = clsid;
	return ERROR_SUCCESS;
}
#pragma export off

//------------------------------------------------------------------
//	StringFromCLSID
//
//	Takes a const Class ID in binary GUID form and converts it to the string
//	representation.
//
//	NOTE: The Windows function allocates memory and calls StringFromGUID2()
//	but unless FieldWorks actually uses StringFromGUID2() we don't
//	need to implement it.
//------------------------------------------------------------------
#pragma export on
HRESULT StringFromCLSID(const CLSID &rclsid, LPOLESTR *ppsz)
{
	const unsigned char	*clsid = reinterpret_cast<const unsigned char*>(&rclsid);
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
//      Populates the DLL Map.
//------------------------------------------------------------------
#pragma export on
ComRegistry::ComRegistry()
{
	// Populate the dllmap
	char* dllmapfilename = "../../dllmap.txt";
	int linenum=1;
	std::string::size_type pos;
	std::string::size_type len;
	std::ifstream dllmapfilestream(dllmapfilename); // Maybe have a helpful message if opening fails
	// For each dllmapping line, add to the dllmap
	for (std::string line; getline(dllmapfilestream, line); linenum++) {

		pos = line.find(" ", 0);
		if (std::string::npos == pos) {
			fprintf(stderr, "Warning: malformed dllmap line in file '%s' on line %d", dllmapfilename, linenum);
			continue;
		}
		len = line.length();
		std::string classid = line.substr(0,pos);
		std::string dllfilename = line.substr(pos, len);
	
		CLSID clsid;

		// magic from Neil. I did not error-check this myself.
		string guidstr = classid;
		std::vector<wchar_t> wstr(guidstr.size() + 1);
		std::copy(guidstr.begin(), guidstr.end(), wstr.begin());
		wstr.back() = 0;
		CLSIDFromString(&wstr[0], &clsid);
		// magic/

		dllmap[clsid] = dllfilename;
	}
	dllmapfilestream.close();
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
//	for Class.
// 
// 	If the Class ID is not found, we look in the dllmap, 
//	find and open the corresponding DLL file, register its COM objects, 
// 	and return the factory pointer for Class.
// 
//	If the Class ID is not found in the registry or dllmap, 
//	pIFactory is left unchanged, and REGDB_E_CLASSNOTREG is returned.
//------------------------------------------------------------------
#pragma export on
HRESULT ComRegistry::GetFactoryPtr (const CLSID &Class, LPCLASSFACTORY* pIFactory) const
{
	// Look for the factory pointer, by Class ID, in the COM registry
	const iterator where = ComRegistry::GetInstance()->find(Class);
	if (where != end())
	{
		*pIFactory = (*where).second;
		return S_OK;
	}

	// No factory claims to support the Class ID. 

	// If we know where the DLL is, dlopen it to get it registered, 
	// and try again.

	// Find the DLL file according to Class ID in the dllmap
	DllMap::const_iterator dllwhere = dllmap.find(Class);
	if (dllwhere == dllmap.end()) {
		return REGDB_E_CLASSNOTREG;
	}
	string dllfilename = (*dllwhere).second;
	if (dllfilename.empty()) {
		return REGDB_E_CLASSNOTREG;
	} else {

		// Load the DLL file into memory
		void* dllhandle = dlopen(dllfilename.c_str(), RTLD_LAZY | RTLD_NODELETE ); // Maybe use RTLD_NOW instead of RTLD_LAZY? I definitely want the global static variable to be created.
		if (!dllhandle) {
			char* dllerror = dlerror();
			fprintf(stderr, "Warning: error loading DLL file '%s' in ComRegistry::GetFactoryPtr: %s\n", dllfilename.c_str(), dllerror);
			return REGDB_E_CLASSNOTREG;
		}

		// I assume that I don't actually have to call any of the methods in the DLL, since there should be a global static GenericFactory. But maybe we need to call DllMain anyway for some reason. We'll see. -MarkS

		// CLSID Class should now be registered, so try again.
		return GetFactoryPtr(Class, pIFactory);
	}
}
#pragma export off

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
	/*IN*/ LPVOID		/*pvReserved*/,
	/*IN*/ REFIID		/*riid*/,
	/*OUT*/ LPVOID*		ppv)
{
	assert(dwClsContext == CLSCTX_INPROC);

	IClassFactory* pIFactory = NULL;
	HRESULT	hr = ComRegistry::GetInstance()->GetFactoryPtr (rclsid, &pIFactory);
	if (SUCCEEDED(hr))
	  pIFactory->AddRef();

	*ppv = pIFactory;
	return hr;
}
#pragma export off


#pragma export on
/**
 * CoInitialize
 * 
 * Do nothing, but be here for when Mono wants to call CoInitialize.
 * 
 * (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp)
 */
extern "C" HRESULT CoInitialize(
	LPVOID pvReserved)
{
	return S_OK;
}
#pragma export off

//------------------------------------------------------------------
//	CoCreateInstance
//
//	Returns an instance of the class rclsid after obtaining the factory
//	pointer of the entry in the registry for Class.
//
//	NOTE: This implementation is intended for use for inprocess COM only.
//------------------------------------------------------------------
#pragma export on
extern "C" HRESULT CoCreateInstance (
	/*IN*/ REFCLSID		rclsid,
	/*IN*/ LPUNKNOWN	pUnkOuter,
	/*IN*/ DWORD		/*dwClsContext*/,
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

//------------------------------------------------------------------
//
//	Register Server
// 	
// 	Registers a class factory by class ID.
#pragma export on
extern "C" void RegisterServer(const CLSID &Class, LPCLASSFACTORY Pointer)
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
#endif	//	DEBUG
