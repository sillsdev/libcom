/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupport.cpp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			2001-05-24	Neil Mayhew

	$Id$

	Support for COM in Linux
	
Cleaning up the code. Removing messy debugging code, putting in proper API comments, and renaming the rest of the hungarian to variables that state their use rather than type.
	2007-01-29, MarkS
Calling DllGetClassObject, and lots of other changes.                  I ♥ gdb
	2007-01   , MarkS
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


#include "Types.h"
#include "WinError.h"
#include "COMInterfaces.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cctype>

#include "COMSupportInternals.h"
#include "COMLibrary.h"

#include <map>
#include <dlfcn.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

#include <stdexcept>

//------------------------------------------------------------------
//	CLSIDFromString
//
//	Takes a const string representation of a Class ID and converts to the
//	binary GUID form. Places bytes in little endian positions.
//------------------------------------------------------------------
#pragma export on
HRESULT CLSIDFromString(LPCOLESTR lpsz, LPCLSID pclsid)
{
	const OLECHAR *p = lpsz;
	wchar_t wc;
	char	c;
	int		i, j, cd;
	union {
		PlainGUID clsid;
		char      cst[16];
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
	OLECHAR			*buf, *q;
	unsigned int	c, i;
	static wchar_t	hex_digits[] = L"0123456789ABCDEF";

	// Better test needed here?
	if (!ppsz) return E_INVALIDARG;
	
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
/**
 * Creates an instance of ComRegistry. ComRegistry stores a mapping between 
 * classID GUIDs and their corresponding class factories. 
 * This constructor will also populate the DLL Map from the dllmap.txt file.
 */
ComRegistry::ComRegistry()
{
	// Populate the dllmap

	// Right now, dllmap.txt needs to be present in the directory from which mono was executed.
	char* dllmapfilename = "dllmap.txt";
	int linenum=1;
	std::string::size_type pos;
	std::string::size_type len;
	std::ifstream dllmapfilestream(dllmapfilename); // Maybe have a helpful message if opening fails

	if (dllmapfilestream.fail()) {
		fprintf(stderr, "COM Support Library: %s:%d ComRegistry::ComRegistry() Warning, opening of dllmap '%s' failed.\n", __FILE__,__LINE__,dllmapfilename);
	}

	// For each dllmapping line, add to the dllmap
	for (std::string line; getline(dllmapfilestream, line); linenum++) {
		pos = line.find(" ", 0);
		if (std::string::npos == pos) {
			fprintf(stderr, "COM Support Library: Warning: malformed dllmap line in file '%s' on line %d", dllmapfilename, linenum);
			continue;
		}
		len = line.length();
		std::string classid = line.substr(0,pos);
		std::string dllfilename = line.substr(pos+1, len);
	
		// Convert classid to an OLE string, then to binary
		std::vector<OLECHAR> olestr(classid.begin(), classid.end());
		olestr.push_back(0);
		CLSID clsid;
		CLSIDFromString(&olestr[0], &clsid);

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

/**
 * Get a class factory from comRegistry that can produce classes of a certain classID.
 * @param classID Class ID that you want a class factory for
 * @param comRegistry COM Registry in which to search
 * @return pointer to class factory corresponding to classID, or NULL if not found
 * @see ComRegistry::Register
 */
LPCLASSFACTORY getFactory(const CLSID &classID, const ComRegistry* comRegistry) {
	
	ComRegistry::const_iterator where = comRegistry->find(classID);
	
	if (where != comRegistry->end())
	{
		return (*where).second;
	}
	return NULL;
}

/** Thrown if a specific ClassID was not found in the DLL Map. */
class ClassIDNotFoundInDllMap : std::exception {};
/** Thrown if the DLL filename corresponding to a ClassID in the DLL Map is empty (not specified). */
class DLLFilenameInDllMapIsEmpty : std::exception {};
/** Thrown if a certain ClassID is not registered. */
class exceptionREGDB_E_CLASSNOTREG : std::exception {};

/**
 *	Find DLL filename corresponding to Class ID in a dllmap.
 * 
 * 	@param classID Class ID matching a DLL file in the dllmap
 * 	@param dllmap dllmap from which to find desired DLL files
 * 	@return dll filename
 *	@throws ClassIDNotFoundInDllMap if classID is not in the dllmap
 *	@throws DLLFilenameInDllMapIsEmpty if the DLL filename is empty
 */
string getDLLFilename(const CLSID &classID, const DllMap& dllmap) {

	DllMap::const_iterator dllwhere = dllmap.find(classID);
	if (dllwhere == dllmap.end()) {
		throw ClassIDNotFoundInDllMap();
	}

	string dllfilename = (*dllwhere).second;
	if (dllfilename.empty()) {
		throw DLLFilenameInDllMapIsEmpty();
	}
	
	return dllfilename;
}


#pragma export on
/**
 * Get a class factory for a class out of the registry, possibly loading a necessary DLL file.
 * If classID is not found in the registry, we look in the DLL Map, find and open the corresponding DLL file, 
 * the DLL registers its COM objects or we will, and we return the class factory for classID.
 * If we fail to get the desired factory, then classFactory is left unchanged.
 * @return S_OK upon successfully getting a class factory, or if the class factory is not already in the registry: REGDB_E_CLASSNOTREG if classID is not in the DLL Map, REGDB_E_CLASSNOTREG if the DLL filename in the DLL Map is empty (unspecified), REGDB_E_CLASSNOTREG if there was an error dlopen()ing the DLL file, REGDB_E_CLASSNOTREG if there was a problem registering a factory for classID in the DLL, or REGDB_E_CLASSNOTREG if we really ultimately fail to find the class factory.
 */
HRESULT ComRegistry::GetFactoryPtr (const CLSID &classID, LPCLASSFACTORY* classFactory) const
{
	// Look for the factory pointer, by Class ID, in the COM registry
	LPCLASSFACTORY resultFactory = 0;
	ComRegistry* comRegistry = ComRegistry::GetInstance();
	if (resultFactory = getFactory(classID, comRegistry)) {
		*classFactory = resultFactory;
		return S_OK;
	}

	// No factory claims to support the Class ID. 

	// If we know where the DLL is, dlopen it to get it registered, 
	// and try again.

	// Find the DLL file according to classID in the dllmap
	string dllfilename;
	try {
		dllfilename = getDLLFilename(classID, dllmap);
	} catch (ClassIDNotFoundInDllMap const& e) {
		return REGDB_E_CLASSNOTREG;
	} catch (DLLFilenameInDllMapIsEmpty const& e) {
		return REGDB_E_CLASSNOTREG;
	}

	// Load the DLL file into memory.
	// If RTLD_NODELETE is not enough to avoid undesirable things related to 
	// calling dlopen(3) more than once on the same DLL file, then 
	// RTLD_NOLOAD could be used to test if it's already loaded.
	void* dllhandle = dlopen(dllfilename.c_str(), RTLD_LAZY | RTLD_NODELETE); // Maybe use RTLD_NOW instead of RTLD_LAZY? I definitely want the global static variable to be created.

	if (!dllhandle) {
			char* dllerror = dlerror();
			fprintf(stderr, "COM Support Library: Warning: error loading DLL file '%s' in ComRegistry::GetFactoryPtr: %s\n", dllfilename.c_str(), dllerror);
			return REGDB_E_CLASSNOTREG;
		}

	// Register the COM object we just opened.
	try {
		registerFactoryInDLL(dllhandle, classID, IID_IClassFactory);
	} catch(exceptionREGDB_E_CLASSNOTREG const& e) {
		return REGDB_E_CLASSNOTREG;
	}

	// classID should now be registered (either by it calling RegisterServer, or by us registering it for it), so try again.
	// TODO: Note, the whole idea of it calling our RegisterServer I think is our own invention, so that ..way should probably be removed.
	if (resultFactory = getFactory(classID, comRegistry)) {
		*classFactory = resultFactory;
		return S_OK;
	}
	return REGDB_E_CLASSNOTREG;
}
/**
 * Register a class factory for requestedClassID from an open COM DLL file by calling its DllGetClassObject function.
 * 
 * @param dllhandle handle to an open COM DLL file
 * @param requestedClassID class ID to get a factory for
 * @param factoryInterfaceID interface ID for IClassFactory (or an interface ID for any interface that an IClassFactory in the DLL implements)
 * @throws exceptionREGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory (which might be a little different than REGDB_E_CLASSNOTREG is really intended for).
 */
void registerFactoryInDLL(void* dllhandle, REFCLSID requestedClassID, REFIID factoryInterfaceID /*= IID_IClassFactory*/) {

	IClassFactory* factory;
	// DllGetClassObject: http://msdn2.microsoft.com/en-us/library/ms680760.aspx
	HRESULT (*DllGetClassObject)(REFCLSID requestedClassID, REFIID requestedInterfaceID, VOID ** objectInterface);
	dlerror(); // clear any old error conditions
	*(void **) (&DllGetClassObject) = dlsym(dllhandle, "DllGetClassObject");
	char* dllerror = dlerror();
	if (NULL != dllerror)
	{
		fprintf(stderr, "COM Support Library: Error getting COM object's DllGetClassObject function. Error: %s\n", dllerror);
		throw exceptionREGDB_E_CLASSNOTREG();
	}
	// Note that if we pass factory as a null pointer, it'll not work.
	(*DllGetClassObject)(requestedClassID, factoryInterfaceID, (VOID**)&factory);
	// Register the class factory
	RegisterServer(requestedClassID, factory);
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

inline void swap(unsigned char &a, unsigned char &b) {
	char tmp = a;
	a = b;
	b = tmp;
}

/**
 * Change a little endian GUID to a big endian GUID.
 * @param guid little endian GUID
 * @return big endian GUID based on guid
 */
GUID mangleGuid(GUID guid) {

	// this is implemented quickly and dirtily

	unsigned char tmp;
	
	//unsigned char* data = guid.str().c_str();
	unsigned char data[37];
	//const char* origGuid = guid.str().c_str();
	
	memcpy(data,reinterpret_cast<void*>(&guid),37);
	int last = sizeof(long)-1;

	for (int i=0;i< (last+1)/2 ; i++) {
		tmp = data[i];
		data[i] = data[last-i];
		data[last-i] = tmp;
	}
	
	//fprintf(stderr,"data8 is %c.\n", data[8]);
	swap(data[4],data[5]);
	swap(data[6],data[7]);
	//fprintf(stderr,"data8 is now %c.\n",data[8]);
	
	// bswap_32/16 would be better, but doesn't seem to be working for me
	
	//GUID origguidguid = origGuid;
	//GUID origguidguid;
	//CLSIDFromString(origGuid, origguidguid);
	//return origguidguid;
	
	memcpy((void*)&guid,data,37);
	return guid;
}


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

	// little endian to big endian the requestedClassID
	CLSID mangled_clsid = mangleGuid(rclsid);

	IClassFactory* pIFactory = NULL;
	HRESULT hr = CoGetClassObject(mangled_clsid,
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
