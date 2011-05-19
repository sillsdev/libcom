/*
 *	$Id$
 *
 *	Bakery COM Server example and test of COM
 *
 *	MarkS - 2007-11-05
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
#include <stdexcept>

// TODO AssertPtr, Assert, and ThrowHr should be pulled in correctly some time, once Generic starts to compile etc better.
#define AssertPtr(x) assert((x) != 0)
#define Assert(x) assert(x)
#define ThrowHr(ex, msg) throw std::runtime_error("ThrowHr")

#include "Bakery.h"
#include "tools.h"
#include "jar.tlh"

#ifdef USE_FW_GENERIC_FACTORY
#include "BinTree.h"
#include "GenericFactory.h"
#endif // USE_FW_GENERIC_FACTORY


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
const wchar_t g_versionIndependentProgID[] = L"Yup.Bakery";
/** ProgID */
const wchar_t* g_ProgID = L"Yup.Bakery.1";
const wchar_t* g_classDescription = L"A Bakery. Bakes cookies.";
const wchar_t* g_threadingModel = L"Apartment";

#ifdef USE_FW_GENERIC_FACTORY
// Generic factory to allow creating an instance with CoCreateInstance
static GenericFactory g_factBakery(
  g_ProgID, // ProgID
  &CLSID_Bakery, // CLSID
  g_classDescription, // Description of the class
  g_threadingModel, // Threading model
  &Bakery::CreateCom); // Static create method

/**
 * Create a Bakery COM object.
 * @param outerAggregateIUnknown should be NULL
 * @param interfaceid interface ID through which to access the created Bakery
 * @param objectInterface receives the interface through which to access Bakery
 * @throws std::runtime_error if outerAggregateIUnknown is not NULL, or if the QueryInterface on Bakery failed.
 */
void Bakery::CreateCom(IUnknown *outerAggregateIUnknown, REFIID interfaceid, void ** objectInterface) {
//	AssertPtr(objectInterface);
//	Assert(!*objectInterface);
	if (outerAggregateIUnknown) {
		// TODO: use the ThrowHr like we probably should. (?)
		// ThrowHr(WarnHr(CLASS_E_NOAGGREGATION));
		throw std::runtime_error("CLASS_E_NOAGGREGATION in Bakery.cpp");
	}

	Bakery* bakery = new Bakery; 
	if (S_OK != bakery->QueryInterface(interfaceid, objectInterface)) {
		bakery->Release(); // Um, shouldn't we NOT be doing this? If we SHOULD be, then update the CFactory::CreateInstance function to do likewise. 
		throw std::runtime_error("Bakery createcom queryinterface failed in Bakery.cpp.");
	}
}
  
#else /* !USE_FW_GENERIC_FACTORY */

// Upon being dlopen'ed, this will create our class factory (and as long as RegisterFactory() is still being used, will register a pointer to the class factory (as an IClassFactory)). TODO yeah, probably not any more, right? Or did we leave support for that in?
static CFactory classFactory;

/** DllGetClassObject */
EXTERN_C HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface)
{

	GUID IID_Bakery = __uuidof(Bakery);

	if (requestedClassID != IID_Bakery)
		return CLASS_E_CLASSNOTAVAILABLE;

	CFactory* factory = new CFactory();
	if (NULL == factory)
		return E_OUTOFMEMORY;
	
	HRESULT hr = factory->QueryInterface(requestedInterfaceID, objectInterface);
	factory->Release();
	
	return hr;
}

// Bakery's Class Factory.

// Bakery's Class Factory's implementation of IUnknown.

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

// Bakery's Class Factory implementation of IClassFactory
// http://msdn2.microsoft.com/en-us/library/ms694364.aspx

/**
 * CFactory::CreateInstance
 * http://msdn2.microsoft.com/en-us/library/ms682215.aspx
 * We don't call a CoRegisterClassObject, like the MSDN API says we are supposed to.
 * @param outerAggregateIUnknown Must be NULL since we don't support aggregation
 * @param interfaceid interface that will be used to communicate with the created Bakery
 * @param objectInterface interface to Bakery requested by interfaceid, or NULL if Bakery doesn't support interfaceid.
 * @return S_OK if successful Bakery creation, E_OUTOFMEMORY if we failed to create Bakery due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, or E_NOINTERFACE if Bakery does not support the requested interface.
 */
HRESULT __stdcall CFactory::CreateInstance(IUnknown* outerAggregateIUnknown, 
	const IID& interfaceid, void** objectInterface) {
	// We don't support aggregation.
	if (outerAggregateIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create a Bakery
	Bakery* bakery = new Bakery;
	if (bakery == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get the desired interface on Bakery
	HRESULT hr = bakery->QueryInterface(interfaceid, objectInterface);

	// TODO If we should release Bakery upon successful QueryInterface, as in Bakery::ComCreate, then do likewise here.

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


/** Bakery constructor.
 */
Bakery::Bakery(void) : cookiesBaked(0), m_referenceCount((LONG)1) {
}

/** Bakery destructor.
 */
Bakery::~Bakery(void) {
}

/** Get the number of cookies that have been baked by this Bakery.
 * @param number will store the number
 * @return S_OK
 */
HRESULT Bakery::getNumberCookiesBaked(long * number) {
	*number = cookiesBaked;
	return S_OK;
}

/** Bake some cookies and put them in a cookie jar. Update the number of cookies baked.
 * @param amount number of cookies to bake
 * @param jar cookie jar in which to store the freshly baked cookies. Mmm.
 * @return S_OK upon successful operation, or perhaps something else otherwise.
 */
HRESULT Bakery::bakeCookies(long amount, IUnknown * jar) {
	GUID IID_IJar = __uuidof(Jar::IJar);
	Jar::IJar * jarinstance;
	long hr = jar->QueryInterface(IID_IJar, (void**)&jarinstance);
	printf("Bakery::bakeCookies calling Jar QI returns HRESULT of %d.\n", hr);
	if (FAILED(hr))
		return hr;
	hr = jarinstance->AddCookies(amount);
	printf("Bakery::bakeCookies calling Jar AddCookies returns HRESULT of %d.\n", hr);
	if (FAILED(hr))
		return hr;
	cookiesBaked += amount;
	return S_OK;
}

// Bakery's implementation of IUnknown (QueryInterface, AddRef, Release)
// http://msdn2.microsoft.com/en-us/library/ms680509.aspx

/**
 * Bakery::QueryInterface
 * 
 * Get a specific interface to Bakery according to interfaceid.
 * http://msdn2.microsoft.com/en-us/library/ms682521.aspx
 * 
 * @param interfaceid interface to Bakery requested
 * @param objectInterface will receive the interface to Bakery requested
 * @return S_OK on success or E_NOINTERFACE if Bakery does not support the requested interface.
 */
HRESULT __stdcall Bakery::QueryInterface(const IID& interfaceid, void** objectInterface) {
	if (IID_IUnknown == interfaceid) {
		*objectInterface = static_cast<IBakery*>(this); 
	} else if (IID_IBakery == interfaceid) {
		*objectInterface = static_cast<IBakery*>(this);
	} else {
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}
	
	// Increment the reference count on the interface
	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	return S_OK;
}

/**
 * Bakery::AddRef
 * 
 * Increment the refence count on Bakery.
 */
UINT32 __stdcall Bakery::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * Bakery::Release
 * 
 * Decrement the reference count on Bakery. Bakery will be destroyed if 
 * the reference count drops to zero.
 */
UINT32 __stdcall Bakery::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount))
	{
		delete this;
		return 0;
	}
	return m_referenceCount;
}
