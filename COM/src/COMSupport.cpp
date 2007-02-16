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

/**
 * Convert a string representation of a classID to a binary classID GUID (in little endian?).
 * The spec says it could also return REGDB_E_CLASSNOTREG and REGDB_E_READREGDB, but we don't currently.
 * http://msdn2.microsoft.com/en-us/library/ms680589.aspx
 * @param inWideString input string to convert to a classID
 * @param outClassID output binary classID GUID based on inWideString
 * @return NOERROR on successful conversion, E_INVALIDARG if one or more invalid arguments, or CO_E_CLASSSTRING if the class string was not formatted correctly.
 */
#pragma export on
HRESULT WINAPI CLSIDFromString(LPCOLESTR inWideString, LPCLSID outClassID)
{
	const OLECHAR *p = inWideString;
	wchar_t wc;
	char	c;
	int		i, j, cd;
	union {
		PlainGUID clsid;
		char      cst[16];
	};
	char	*q = &cst[0];

	// Better test needed here?
	if (!inWideString || !outClassID) return E_INVALIDARG;

//TODO	// Consume open brace and any preceding characters
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
	
	*outClassID = clsid;
	return ERROR_SUCCESS;
}
#pragma export off

/**
 * Convert binary classID GUID to string representation.
 * http://msdn2.microsoft.com/en-us/library/ms683917.aspx
 * @param inClassID input binary classID GUID to convert
 * @param outWideString output string based on inClassID
 * @return S_OK on successful conversion, E_OUTOFMEMORY if we run out of memory while creating the string, or (despite the spec) E_INVALIDARG if an invalid argument was given.
 */
#pragma export on
HRESULT StringFromCLSID(const CLSID &inClassID, LPOLESTR *outWideString)
{
	// NOTE: The Windows function allocates memory and calls StringFromGUID2()
	// but we're not implementing StringFromGUID2().
	const unsigned char	*clsid = reinterpret_cast<const unsigned char*>(&inClassID);
	OLECHAR			*buf, *q;
	unsigned int	c, i;
	static wchar_t	hex_digits[] = L"0123456789ABCDEF";

	// Better test needed here?
	if (!outWideString) return E_INVALIDARG;
	
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
	*outWideString = buf;
	return S_OK;
}
#pragma export off

/**
 * Allocate memory.
 * Because there is no guarantee that memory will actually be allocated, you should always check that the return value is not NULL.
 * http://msdn2.microsoft.com/en-us/library/ms692727.aspx
 * @param size number of bytes to allocate
 * @return pointer to allocated memory, or NULL if the memory allocation request failed.
 */
#pragma export on
void* CoTaskMemAlloc(SIZE_T size)
{
	// This is defined as just malloc() until a reason is found
	// for a more complex implementation.

	return (std::malloc(size));
}
#pragma export off

/**
 * Change size of a previously allocated chunk of memory. 
 * If previousAllocation is NULL, allocates memory like CoTaskMemAlloc.
 * If previousAllocation is not NULL and newSize is 0, then previousAllocation is freed.
 * http://msdn2.microsoft.com/en-us/library/ms687280.aspx
 * @param previousAllocation pointer returned from a previous CoTaskMemAlloc to previous allocation of memory to be reallocated or freed, or NULL
 * @param newSize number of bytes for the new allocation, or 0 (see description).
 * @return pointer to reallocated memory (which may be in a different memory location than previousAllocation), or NULL if newSize is 0 and previousAllocation is not NULL, or NULL if we fail to allocate memory for the new allocation
 */
#pragma export on
void* CoTaskMemRealloc(LPVOID previousAllocation, SIZE_T newSize)
{
	// This is defined as just realloc() until a reason is found
	// for a more complex implementation.

	return (std::realloc(previousAllocation, newSize));
}
#pragma export off

/**
 * Free previously allocated memory. If allocatedMemory is NULL, nothing happens.
 * http://msdn2.microsoft.com/en-us/library/ms680722.aspx
 * @param allocatedMemory pointer to memory previously allocated by CoTaskMemAlloc or CoTaskMemRealloc, or NULL
 */
#pragma export on
void CoTaskMemFree(LPVOID allocatedMemory)
{
	// This is defined as just free() until a reason is found
	// for a more complex implementation.

	std::free(allocatedMemory);
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

/**
 * ComRegistry destructor. Destroys an instance of ComRegistry.
 * Does nothing.
 */
#pragma export on
ComRegistry::~ComRegistry()
{
	// No explicit actions needed
}
#pragma export off
	
/**
 * Gets a pointer to a previously created registry, or to a new one if none 
 * has yet been created. The static instance of the COM registry is 
 * created here.
 */
#pragma export on
ComRegistry* ComRegistry::GetMutableInstance()
{
	static ComRegistry instance;
	return &instance;
}
#pragma export off

/**
 * Register a class factory. Adds an entry to the registry for a class and its factory pointer.
 * @param classID class ID to register
 * @param classFactory class factory that can create objects of class ID classID.
 */
#pragma export on
void ComRegistry::Register(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	ComRegistry *comRegistry = ComRegistry::GetMutableInstance();
	(*comRegistry)[classID] = classFactory;
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

/**
 * Get a class factory able to create objects of class ID requestedClassID.
 * NOTE: This function does NOT do what the MSDN spec actually says it does. Fix this is if that's important.
 * Note: The caller is responsible for releasing the class factory.
 * Note: This implementation is intended for use for inprocess COM only.
 * http://msdn2.microsoft.com/en-us/library/ms684007.aspx
 * @param requestedClassID class ID for which you want a class factory
 * @param dwClsContext should be CLSCTX_INPROC
 * @param factoryInterface receives the desired class factory, of type IClassFactory
 * @return S_OK upon success, or REGDB_E_CLASSNOTREG upon a bunch of means of failure
 */
#pragma export on
HRESULT CoGetClassObject(
	/*IN*/ REFCLSID		requestedClassID,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ LPVOID		/*pvReserved*/,
	/*IN*/ REFIID		/*requestedInterfaceID*/,
	/*OUT*/ LPVOID*		factoryInterface)
{
	if (dwClsContext =! CLSCTX_INPROC) {
		return CO_E_NOT_SUPPORTED;
	}

	IClassFactory* pIFactory = NULL;
	HRESULT	hr = ComRegistry::GetInstance()->GetFactoryPtr(requestedClassID, &pIFactory);
	if (SUCCEEDED(hr))
	  pIFactory->AddRef();

	*factoryInterface = pIFactory;
	return hr;
}
#pragma export off


/**
 * CoInitialize
 * 
 * Do nothing, but be here for when Mono wants to call CoInitialize.
 * 
 * (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp)
 * @param pvReserved unused
 * @return S_OK
 */
#pragma export on
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

/**
 * Create an instance of a class of class ID requestedClassID, which implements interface ID objectInterfaceID, and will be accessible through the interface objectInterface.
 * NOTE: This implementation is intended for use for inprocess COM only.
 * http://msdn2.microsoft.com/en-us/library/ms686615.aspx
 * @param requestedClassID class ID of class that will be created and given access to through objectInterface
 * @param outerAggregateIUnknown should be NULL since we don't support aggregation
 * @param objectInterfaceID interface ID of the interface through which you desire to interact with class of class ID requestedClassID
 * @param objectInterface receives the interface to the desired instantiated class, or is NULL if the desired instantiated class does not support objectInterfaceID or we otherwise fail
 * @return S_OK upon success, E_OUTOFMEMORY if we failed to create the object due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, E_NOINTERFACE if the object does not support objectInterfaceID, or REGDB_E_CLASSNOTREG upon a bunch of other means of failure (like, if we don't know how to make a requestedClassID).
 */
#pragma export on
extern "C" HRESULT CoCreateInstance (
	/*IN*/ REFCLSID		requestedClassID,
	/*IN*/ LPUNKNOWN	outerAggregateIUnknown,
	/*IN*/ DWORD		/*dwClsContext*/,
	/*IN*/ REFIID		objectInterfaceID,
	/*OUT*/ LPVOID*		objectInterface)
{
	*objectInterface = NULL;

	// little endian to big endian the requestedClassID
	CLSID mangled_requestedClassID = mangleGuid(requestedClassID);

	IClassFactory* pIFactory = NULL;
	HRESULT hr = CoGetClassObject(mangled_requestedClassID,
						static_cast <unsigned long> (CLSCTX_INPROC),
						(void *)0,
						IID_IClassFactory,
						(void **)&pIFactory);
	if (SUCCEEDED(hr))
	{
		// Create the component
		hr = pIFactory->CreateInstance(outerAggregateIUnknown, objectInterfaceID, objectInterface);
		// Release the class factory
		pIFactory->Release();
	}

	return hr;
}
#pragma export off

/**
 * Register a class factory by class ID in the internal registry.
 * @param classID class ID to register
 * @param classFactory class factory to register that can create objects of class ID classID.
 */
#pragma export on
extern "C" void RegisterServer(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	ComRegistry::GetInstance()->Register(classID, classFactory);
}
#pragma export off

#ifdef DEBUG

/**
 * Dump the contents of the registry onto the output stream out.
 * @param out output stream to which to write data
 */
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

/**
 * Converts the Class Factory pointer classFactory to a hexadecimal
 * NUL terminated representation in the 11 byte buffer buf.
 * @param classFactory class factory to use
 * @param buf 11 byte buffer to which to write the hexadecimal representation
 */
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
