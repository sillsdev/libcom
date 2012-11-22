/*
 *	$Id$
 *
 *	Ball COM Server example and test of COM
 *
 *	MarkS - 2005-01
 *
 * Ball COM Support Library Test
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

#include "Ball.h"
#include "tools.h"

#ifdef USE_FW_GENERIC_FACTORY
typedef  INT32  COMINT32;
typedef UINT32 UCOMINT32;
#include "BinTree.h"
#include "GenericFactory.h"
#endif

#include <WinError.h>

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
const wchar_t g_versionIndependentProgID[] = L"Yup.Ball";
/** ProgID */
const wchar_t* g_ProgID = L"Yup.Ball.1";
const wchar_t* g_classDescription = L"A bouncing ball. Fun to play with!";
const wchar_t* g_threadingModel = L"Apartment";


#ifdef USE_FW_GENERIC_FACTORY
// Generic factory to allow creating an instance with CoCreateInstance
static GenericFactory g_factBall(
	g_ProgID, // ProgID
  &CLSID_Ball, // CLSID
  g_classDescription, // Description of the class
  g_threadingModel, // Threading model
  &Ball::CreateCom); // Static create method

/**
 * Create a Ball COM object.
 * @param outerAggregateIUnknown should be NULL
 * @param interfaceid interface ID through which to access the created Ball
 * @param objectInterface receives the interface through which to access Ball
 * @throws std::runtime_error if outerAggregateIUnknown is not NULL, or if the QueryInterface on Ball failed.
 */
void Ball::CreateCom(IUnknown* outerAggregateIUnknown, REFIID interfaceid,
	void** objectInterface) {
//	AssertPtr(objectInterface);
//	Assert(!*objectInterface);
	if (outerAggregateIUnknown) {
		// TODO: use the ThrowHr like we probably should. (?)
		// ThrowHr(WarnHr(CLASS_E_NOAGGREGATION));
		throw std::runtime_error("CLASS_E_NOAGGREGATION in Ball.cpp");
	}

	Ball* ball = new Ball;
	if (S_OK != ball->QueryInterface(interfaceid, objectInterface)) {
		ball->Release(); // Um, shouldn't we NOT be doing this? If we SHOULD be, then update the CFactory::CreateInstance function to do likewise.
		throw std::runtime_error("ball createcom queryinterface failed in Ball.cpp.");
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
	CoRegisterClassInfo(&CLSID_Ball, &progID[0], &classDescription[0]);
	return S_OK;
}

// Upon being dlopen'ed, this will create our class factory (and as long as RegisterFactory() is still being used, will register a pointer to the class factory (as an IClassFactory))
static CFactory classFactory;

EXTERN_C HRESULT DllGetClassObject(REFCLSID requestedClassID, REFIID requestedInterfaceID, LPVOID * objectInterface)
{

	GUID IID_Ball = __uuidof(Ball);

	if (requestedClassID != IID_Ball)
		return CLASS_E_CLASSNOTAVAILABLE;

	CFactory* factory = new CFactory();
	if (NULL == factory)
		return E_OUTOFMEMORY;

	HRESULT hr = factory->QueryInterface(requestedInterfaceID, objectInterface);
	factory->Release();

	return hr;
}

// Ball's Class Factory.

// Ball's Class Factory's implementation of IUnknown.

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
		delete this; // This might cause trouble like in the past.
		return 0;
	}
	return m_referenceCount;
}

// Ball's Class Factory implementation of IClassFactory
// http://msdn2.microsoft.com/en-us/library/ms694364.aspx

/**
 * CFactory::CreateInstance
 * http://msdn2.microsoft.com/en-us/library/ms682215.aspx
 * We don't call a CoRegisterClassObject, like the MSDN API says we are supposed to.
 * @param outerAggregateIUnknown Must be NULL since we don't support aggregation
 * @param interfaceid interface that will be used to communicated with the created Ball
 * @param objectInterface interface to Ball requested by interfaceid, or NULL if Ball doesn't support interfaceid.
 * @return S_OK if successful Ball creation, E_OUTOFMEMORY if we failed to create Ball due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, or E_NOINTERFACE if Ball does not support the requested interface.
 */
HRESULT __stdcall CFactory::CreateInstance(IUnknown* outerAggregateIUnknown,
	const IID& interfaceid, void** objectInterface) {
	// We don't support aggregation.
	if (outerAggregateIUnknown != NULL)
	{
		return CLASS_E_NOAGGREGATION;
	}

	// Create a Ball
	Ball* ball = new Ball;
	if (ball == NULL)
	{
		return E_OUTOFMEMORY;
	}

	// Get the desired interface on Ball
	HRESULT hr = ball->QueryInterface(interfaceid, objectInterface);

	// If we should release Ball upon successful QueryInterface, as in Ball::ComCreate, then do likewise here.

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


/** Ball constructor.
 */
Ball::Ball(void) : distanceRolled(0), timesBounced(0), m_referenceCount((LONG)1) {
}

/** Ball destructor.
 */
Ball::~Ball(void) {
}

/**
 * Bounce this ball.
 */
HRESULT Ball::bounce(void) {
	timesBounced++;
	return S_OK;
}

/**
 * Roll this ball a distance.
 */
HRESULT Ball::roll(long distance, long* total) {
	distanceRolled += distance;
	*total = distanceRolled;
	return S_OK;
}

// Ball's implementation of IUnknown (QueryInterface, AddRef, Release)
// http://msdn2.microsoft.com/en-us/library/ms680509.aspx

/**
 * Ball::QueryInterface
 *
 * Get a specific interface to Ball according to interfaceid.
 * http://msdn2.microsoft.com/en-us/library/ms682521.aspx
 *
 * @param interfaceid interface to Ball requested
 * @param objectInterface will receive the interface to Ball requested
 * @return S_OK on success or E_NOINTERFACE if Ball does not support the requested interface.
 */
HRESULT __stdcall Ball::QueryInterface(const IID& interfaceid, void** objectInterface) {
	if (IID_IUnknown == interfaceid) {
		*objectInterface = static_cast<IBall*>(this);
	} else if (IID_IBall == interfaceid) {
		*objectInterface = static_cast<IBall*>(this);
	} else {
		*objectInterface = NULL;
		return E_NOINTERFACE;
	}

	// Increment the reference count on the interface
	reinterpret_cast<IUnknown*>(*objectInterface)->AddRef();
	return S_OK;
}

/**
 * Ball::AddRef
 *
 * Increment the refence count on Ball.
 */
UINT32 __stdcall Ball::AddRef() {
	return InterlockedIncrement(&m_referenceCount);
}

/**
 * Ball::Release
 *
 * Decrement the reference count on Ball. Ball will be destroyed if
 * the reference count drops to zero.
 */
UINT32 __stdcall Ball::Release() {
	if (0 == InterlockedDecrement(&m_referenceCount))
	{
		delete this;
		return 0;
	}
	return m_referenceCount;
}
