/*
 *	$Id$
 *
 *	Wall COM Server example and test of COM - tests strings
 *
 *	MarkS - 2008-02-08
 *
 * COM Support Library Test
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

#include <assert.h>
#include <iostream>
#include <vector>
#include <stdexcept>

// TODO AssertPtr, Assert, and ThrowHr should be pulled in correctly some time, once Generic starts to compile etc better.
#define AssertPtr(x) assert((x) != 0)
#define Assert(x) assert(x)
#define ThrowHr(ex, msg) throw std::runtime_error("ThrowHr")

#include "Wall.h"
#include "tools.h"

#ifdef USE_FW_GENERIC_FACTORY
typedef  INT32  COMINT32;
typedef UINT32 UCOMINT32;
#include "BinTree.h"
#include "GenericFactory.h"
#endif // !USE_FW_GENERIC_FACTORY

#include <COMError.h>
#include <unicode/ustring.h>

// Write a UnicodeString (UTF-16) to an ostream (UTF-8)
std::ostream& operator << (std::ostream& o, const icu::UnicodeString& s)
{
	std::vector<char> buffer(s.length()*4+1); // 4 bytes is the maximum UTF-8 sequence
	UErrorCode error = U_ZERO_ERROR;
	u_strToUTF8(&buffer[0], buffer.size(), 0, s.getBuffer(), s.length(), &error);
	assert(U_SUCCESS(error));
	return o << &buffer[0];
}

// Begin copied code from Wine's winbase.h, modified 2007-02-01 by Neil Mayhew

/* Copyright (C) the Wine project */

/*extern inline LONG WINAPI InterlockedExchangeAdd( LONG volatile *dest, LONG incr );
extern inline LONG WINAPI InterlockedExchangeAdd( LONG volatile *dest, LONG incr )
{
    LONG ret;
    __asm__ __volatile__( "lock; xaddl %0,(%1)"
                          : "=r" (ret) : "r" (dest), "0" (incr) : "memory" );
    return ret;
}*/

extern inline LONG WINAPI InterlockedIncrement( LONG volatile *dest );
extern inline LONG WINAPI InterlockedIncrement( LONG volatile *dest )
{
    return ++*dest;
}

extern inline LONG WINAPI InterlockedDecrement( LONG volatile *dest );
extern inline LONG WINAPI InterlockedDecrement( LONG volatile *dest )
{
    return --*dest;
}

// End copied code from Wine's winbase.h

// Global variables

static volatile LONG g_serverLockCount = 0;
/** version-independent ProgID */
const wchar_t g_versionIndependentProgID[] = L"Yup.Wall";
/** ProgID */
const wchar_t* g_ProgID = L"Yup.Wall.1";
const wchar_t* g_classDescription = L"A Wall. Can be written on.";
const wchar_t* g_threadingModel = L"Apartment";

#ifdef USE_FW_GENERIC_FACTORY
// Generic factory to allow creating an instance with CoCreateInstance
static GenericFactory g_factWall(
  g_ProgID, // ProgID
  &CLSID_Wall, // CLSID
  g_classDescription, // Description of the class
  g_threadingModel, // Threading model
  &Wall::CreateCom); // Static create method

/**
 * Create a Wall COM object.
 * @param outerAggregateIUnknown should be NULL
 * @param interfaceid interface ID through which to access the created Wall
 * @param objectInterface receives the interface through which to access Wall
 * @throws std::runtime_error if outerAggregateIUnknown is not NULL, or if the QueryInterface on Wall failed.
 */
void Wall::CreateCom(IUnknown* outerAggregateIUnknown, REFIID interfaceid, void** objectInterface) {
//	AssertPtr(objectInterface);
//	Assert(!*objectInterface);
	if (outerAggregateIUnknown) {
		// TODO: use the ThrowHr like we probably should. (?)
		// ThrowHr(WarnHr(CLASS_E_NOAGGREGATION));
		throw std::runtime_error("CLASS_E_NOAGGREGATION in Wall.cpp");
	}

	Wall* wall = new Wall;
	if (S_OK != wall->QueryInterface(interfaceid, objectInterface)) {
		wall->Release(); // Um, shouldn't we NOT be doing this? If we SHOULD be, then update the CFactory::CreateInstance function to do likewise.
		throw std::runtime_error("Wall createcom queryinterface failed in Wall.cpp.");
	}
}

#else /* !USE_FW_GENERIC_FACTORY */

typedef void* HMODULE;

EXTERN_C BOOL WINAPI DllMain(HMODULE /*hmod*/, DWORD /*dwReason*/, PVOID /*pvReserved*/)
{
}

STDAPI DllRegisterServer()
{
	std::vector<char> progID(g_ProgID, g_ProgID+wcslen(g_ProgID)+1);
	std::vector<char> classDescription(g_classDescription, g_classDescription+wcslen(g_classDescription)+1);
	CoRegisterClassInfo(&CLSID_Wall, &progID[0], &classDescription[0]);
	return S_OK;
}

// Upon being dlopen'ed, this will create our class factory (and as long as RegisterFactory() is still being used, will register a pointer to the class factory (as an IClassFactory)). TODO yeah, probably not any more, right? Or did we leave support for that in?
static CFactory classFactory;

/** DllGetClassObject */
EXTERN_C HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface)
{

	GUID IID_Wall = __uuidof(Wall);

	if (requestedClassID != IID_Wall)
		return CLASS_E_CLASSNOTAVAILABLE;

	CFactory* factory = new CFactory();
	if (NULL == factory)
		return E_OUTOFMEMORY;

	HRESULT hr = factory->QueryInterface(requestedInterfaceID, objectInterface);
	factory->Release();

	return hr;
}

// Wall's Class Factory.

// Wall's Class Factory's implementation of IUnknown.

/**
 * CFactory::QueryInterface
 */
HRESULT __stdcall CFactory::QueryInterface(const IID& interfaceid, void** objectInterface) {
	if ((IID_IUnknown      == interfaceid) ||
	    (IID_IClassFactory == interfaceid)) {
		*objectInterface = static_cast<IClassFactory*>(this);
	} else {
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}

	// Increment the interface reference count
	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	return S_OK;
}

/**
 * CFactory::AddRef
 */
UINT32 __stdcall CFactory::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * CFactory::Release
 */
UINT32 __stdcall CFactory::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount)) {
		delete this; // TODO This might cause trouble like in the past.
		return 0;
	}
	return m_referenceCount;
}

// Wall's Class Factory implementation of IClassFactory
// http://msdn2.microsoft.com/en-us/library/ms694364.aspx

/**
 * CFactory::CreateInstance
 * http://msdn2.microsoft.com/en-us/library/ms682215.aspx
 * We don't call a CoRegisterClassObject, like the MSDN API says we are supposed to.
 * @param outerAggregateIUnknown Must be NULL since we don't support aggregation
 * @param interfaceid interface that will be used to communicate with the created Wall
 * @param objectInterface interface to Wall requested by interfaceid, or NULL if Wall doesn't support interfaceid.
 * @return S_OK if successful Wall creation, E_OUTOFMEMORY if we failed to create Wall due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, or E_NOINTERFACE if Wall does not support the requested interface.
 */
HRESULT __stdcall CFactory::CreateInstance(IUnknown* outerAggregateIUnknown,
	const IID& interfaceid, void** objectInterface) {
	// We don't support aggregation.
	if (outerAggregateIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create a Wall
	Wall* wall = new Wall;
	if (wall == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get the desired interface on Wall
	HRESULT hr = wall->QueryInterface(interfaceid, objectInterface);

	// TODO If we should release Wall upon successful QueryInterface, as in Wall::ComCreate, then do likewise here.

	return hr;
}

/**
 * CFactory::LockServer
 *
 * http://msdn2.microsoft.com/en-us/library/ms682332.aspx
 */
HRESULT __stdcall CFactory::LockServer(BOOL shouldLock) {
	if (shouldLock) {
		InterlockedIncrement(&g_serverLockCount);
	} else {
		InterlockedDecrement(&g_serverLockCount);
	}
	return S_OK;
}

#endif /* !USE_FW_GENERIC_FACTORY */


/** Wall constructor.
 */
Wall::Wall(void) : contents("Mene, Mene, Tekel, Parsin\n"), m_referenceCount((LONG)1) {
}

/** Wall destructor.
 */
Wall::~Wall(void) {
}

/**
 * Get contents of wall as a BSTR.
 * @param text Caller (or possibly COM itself http://msdn2.microsoft.com/en-us/library/ms221069.aspx ) is responsible for freeing the memory we allocate for this variable. Will point to a new location of newly allocated memory containing data in a BSTR.
 */
HRESULT Wall::readWallBSTR(BSTR* text) {

	std::cout << "readWallBSTR(). We will return the contents of wall, which are basically (length of "<<contents.length()<<"): '"<<contents<<"'" << std::endl;


	*text = SysAllocString((OLECHAR*)contents.getTerminatedBuffer());
	(*text)[contents.length()] = 0;


	/*
	OLECHAR* foo;

	// TODO allocate memory for foo the right way. malloc surely isn't the "right way".
	foo = static_cast<OLECHAR*>(malloc(contents.length()*2+2));
	if (NULL == foo)
		exit(1);
	foo[contents.length()] = 0;

	contents.extract(0,contents.length(),foo);
	*text = SysAllocString(foo);

	free(foo);
	*/

	return S_OK;
}

/**
 * Append text to the wall.
 * @param text text to append to the wall.
 */
HRESULT Wall::writeBSTROnWall(BSTR text) {


	std::cout << "writeBSTROnWall(). Before, wall contains '"<< contents <<"'" << std::endl;
	std::cout << "writeBSTROnWall(). appending string of length '"<<SysStringLen(text)<<"'/2" << std::endl;
	contents.append(text,SysStringLen(text)/2);
	std::cout << "writeBSTROnWall().  After, wall contains '"<< contents <<"'" << std::endl;


	return S_OK;
}

HRESULT Wall::readWall(int,OLECHAR*) {
	return E_NOTIMPL;
}
HRESULT Wall::writeOnWall(int,OLECHAR*) {
	return E_NOTIMPL;
}
HRESULT Wall::writeOnWall2(int,const OLECHAR*) {
	return E_NOTIMPL;
}
HRESULT Wall::writeCharStarOnWall(unsigned char*) {
	return E_NOTIMPL;
}
HRESULT Wall::writeWcharStarOnWall(WCHAR*) {
	return E_NOTIMPL;
}


// Wall's implementation of IUnknown (QueryInterface, AddRef, Release)
// http://msdn2.microsoft.com/en-us/library/ms680509.aspx

/**
 * Wall::QueryInterface
 *
 * Get a specific interface to Wall according to interfaceid.
 * http://msdn2.microsoft.com/en-us/library/ms682521.aspx
 *
 * @param interfaceid interface to Wall requested
 * @param objectInterface will receive the interface to Wall requested
 * @return S_OK on success or E_NOINTERFACE if Wall does not support the requested interface.
 */
HRESULT __stdcall Wall::QueryInterface(const IID& interfaceid, void** objectInterface) {
	if (IID_IUnknown == interfaceid) {
		*objectInterface = static_cast<IWall*>(this);
	} else if (IID_IWall == interfaceid) {
		*objectInterface = static_cast<IWall*>(this);
	} else {
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}

	// Increment the reference count on the interface
	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	return S_OK;
}

/**
 * Wall::AddRef
 *
 * Increment the refence count on Wall.
 */
UINT32 __stdcall Wall::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * Wall::Release
 *
 * Decrement the reference count on Wall. Wall will be destroyed if
 * the reference count drops to zero.
 */
UINT32 __stdcall Wall::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount))
	{
		delete this;
		return 0;
	}
	return m_referenceCount;
}
