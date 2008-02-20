/*
 *	$Id$
 *
 *	Bag COM Server example and test of COM
 *
 *	MarkS - 2008-02-15
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

#include "Bag.h"
#include "tools.h"
#include "Ball.h"

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
const wchar_t g_versionIndependentProgID[] = L"Yup.Bag";
/** ProgID */
const wchar_t* g_ProgID = L"Yup.Bag.1";
const wchar_t* g_classDescription = L"A Bag. Can be written on.";
const wchar_t* g_threadingModel = L"Apartment";

#ifdef USE_FW_GENERIC_FACTORY
// Generic factory to allow creating an instance with CoCreateInstance
static GenericFactory g_factBag(
  g_ProgID, // ProgID
  &CLSID_Bag, // CLSID
  g_classDescription, // Description of the class
  g_threadingModel, // Threading model
  &Bag::CreateCom); // Static create method

/**
 * Create a Bag COM object.
 * @param outerAggregateIUnknown should be NULL
 * @param interfaceid interface ID through which to access the created Bag
 * @param objectInterface receives the interface through which to access Bag
 * @throws std::runtime_error if outerAggregateIUnknown is not NULL, or if the QueryInterface on Bag failed.
 */
void Bag::CreateCom(IUnknown* outerAggregateIUnknown, REFIID interfaceid, void** objectInterface) {
//	AssertPtr(objectInterface);
//	Assert(!*objectInterface);
	if (outerAggregateIUnknown) {
		// TODO: use the ThrowHr like we probably should. (?)
		// ThrowHr(WarnHr(CLASS_E_NOAGGREGATION));
		throw std::runtime_error("CLASS_E_NOAGGREGATION in Bag.cpp");
	}

	Bag* bag = new Bag; 
	if (S_OK != bag->QueryInterface(interfaceid, objectInterface)) {
		bag->Release(); // Um, shouldn't we NOT be doing this? If we SHOULD be, then update the CFactory::CreateInstance function to do likewise. 
		throw std::runtime_error("Bag createcom queryinterface failed in Bag.cpp.");
	}
}
  
#else /* !USE_FW_GENERIC_FACTORY */

// Upon being dlopen'ed, this will create our class factory (and as long as RegisterFactory() is still being used, will register a pointer to the class factory (as an IClassFactory)). TODO yeah, probably not any more, right? Or did we leave support for that in?
static CFactory classFactory;

/** DllGetClassObject */
EXTERN_C HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface)
{

	GUID IID_Bag = __uuidof(Bag);

	if (requestedClassID != IID_Bag)
		return CLASS_E_CLASSNOTAVAILABLE;

	CFactory* factory = new CFactory();
	if (NULL == factory)
		return E_OUTOFMEMORY;
	
	HRESULT hr = factory->QueryInterface(requestedInterfaceID, objectInterface);
	factory->Release();
	
	return hr;
}

// Bag's Class Factory.

// Bag's Class Factory's implementation of IUnknown.

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
ULONG __stdcall CFactory::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/** 
 * CFactory::Release
 */
ULONG __stdcall CFactory::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount)) {
		delete this; // TODO This might cause trouble like in the past.
		return 0;
	}
	return m_referenceCount;
}

// Bag's Class Factory implementation of IClassFactory
// http://msdn2.microsoft.com/en-us/library/ms694364.aspx

/**
 * CFactory::CreateInstance
 * http://msdn2.microsoft.com/en-us/library/ms682215.aspx
 * We don't call a CoRegisterClassObject, like the MSDN API says we are supposed to.
 * @param outerAggregateIUnknown Must be NULL since we don't support aggregation
 * @param interfaceid interface that will be used to communicate with the created Bag
 * @param objectInterface interface to Bag requested by interfaceid, or NULL if Bag doesn't support interfaceid.
 * @return S_OK if successful Bag creation, E_OUTOFMEMORY if we failed to create Bag due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, or E_NOINTERFACE if Bag does not support the requested interface.
 */
HRESULT __stdcall CFactory::CreateInstance(IUnknown* outerAggregateIUnknown, 
	const IID& interfaceid, void** objectInterface) {
	// We don't support aggregation.
	if (outerAggregateIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create a Bag
	Bag* bag = new Bag;
	if (bag == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get the desired interface on Bag
	HRESULT hr = bag->QueryInterface(interfaceid, objectInterface);

	// TODO If we should release Bag upon successful QueryInterface, as in Bag::ComCreate, then do likewise here.

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


/** Bag constructor.
 */
Bag::Bag(void) : m_referenceCount((LONG)1) {
}

/** Bag destructor.
 */
Bag::~Bag(void) {
}

HRESULT Bag::ProvideBall(IUnknown** iball) {
/*	Ball* ball = new Ball; // TODO create the ball a COM way probably instead
	*iball = dynamic_cast<IBall*> (ball);
	return S_OK;
	*/

	//GUID IID_Ball = __uuidof(IBall);
	//GUID CLSID_Ball = __uuidof(Ball);
	//Ball* ball;
	return CoCreateInstance(CLSID_Ball, NULL, (DWORD)CLSCTX_INPROC_SERVER, IID_IBall, (void**)iball);
	

}

HRESULT Bag::InspectBall(IUnknown* iball) {
	long distance_rolled;
	/*(dynamic_cast<Ball*> (iball))->roll(3, &distance_rolled);
	return S_OK; */
	
	
//	GUID IID_IBall = __uuidof(IBall);
	Ball* ballinstance;
	long hr = iball->QueryInterface(IID_IBall, (void**)&ballinstance);
	if (FAILED(hr))
		return hr;
	hr = ballinstance->roll(3, &distance_rolled);
	if (FAILED(hr))
		return hr;
	
	return S_OK;
}


// Bag's implementation of IUnknown (QueryInterface, AddRef, Release)
// http://msdn2.microsoft.com/en-us/library/ms680509.aspx

/**
 * Bag::QueryInterface
 * 
 * Get a specific interface to Bag according to interfaceid.
 * http://msdn2.microsoft.com/en-us/library/ms682521.aspx
 * 
 * @param interfaceid interface to Bag requested
 * @param objectInterface will receive the interface to Bag requested
 * @return S_OK on success or E_NOINTERFACE if Bag does not support the requested interface.
 */
HRESULT __stdcall Bag::QueryInterface(const IID& interfaceid, void** objectInterface) {
	if (IID_IUnknown == interfaceid) {
		*objectInterface = static_cast<IBag*>(this); 
	} else if (IID_IBag == interfaceid) {
		*objectInterface = static_cast<IBag*>(this);
	} else {
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}
	
	// Increment the reference count on the interface
	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	return S_OK;
}

/**
 * Bag::AddRef
 * 
 * Increment the refence count on Bag.
 */
ULONG __stdcall Bag::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * Bag::Release
 * 
 * Decrement the reference count on Bag. Bag will be destroyed if 
 * the reference count drops to zero.
 */
ULONG __stdcall Bag::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount))
	{
		delete this;
		return 0;
	}
	return m_referenceCount;
}