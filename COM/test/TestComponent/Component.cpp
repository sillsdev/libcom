//
// Component.cpp
//	Copied from Rogerson's book CD-ROM and modified for CodeWarrior with COMSupportLib
//

#include "Component.h"

// Trace function
void trace(const char* msg)
{
	cout << msg << endl ;
}

#pragma export on
void ComponentLoaded()
{
	trace("Component shared library is now loaded\n\n");
}
#pragma export off


///////////////////////////////////////////////////////////
//
// Global variables
//
static HMODULE g_hModule = NULL ;   // DLL module handle
static long g_cComponents = 0 ;     // Count of active components
static long g_cServerLocks = 0 ;    // Count of locks

// Friendly name of component
const char g_szFriendlyName[] = "Inside COM, Chapter 7 Example" ;

// Version-independent ProgID
const char g_szVerIndProgID[] = "InsideCOM.Chap07" ;

// ProgID
const char g_szProgID[] = "InsideCOM.Chap07.1" ;

///////////////////////////////////////////////////////////
//
//	Create the class factory and register a pointer to its IClassFactory
//

static CFactory *	classFactory = new CFactory;


///////////////////////////////////////////////////////////
//
// Component 
//

//
// Constructor
//
CA::CA() : m_cRef(1)
{ 
	InterlockedIncrement(&g_cComponents) ; 
}

//
// Destructor
//
CA::~CA() 
{ 
	InterlockedDecrement(&g_cComponents) ; 
	trace("Component:\t\tDestroy self.") ;
}

//
// IUnknown implementation
//
HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{    
	if (iid == IID_IUnknown)
	{
		*ppv = static_cast<IX*>(this) ; 
	}
	else if (iid == IID_IX)
	{
		*ppv = static_cast<IX*>(this) ;
		trace("Component:\t\tReturn pointer to IX.") ; 
	}
	else if (iid == IID_IY)
	{
		*ppv = static_cast<IY*>(this) ; 
		trace("Component:\t\tReturn pointer to IY.") ; 
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

ULONG __stdcall CA::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

ULONG __stdcall CA::Release() 
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}

///////////////////////////////////////////////////////////
//
// Component Class factory
//

//
// IUnknown implementation
//
HRESULT __stdcall CFactory::QueryInterface(const IID& iid, void** ppv)
{    
	if ((iid == IID_IUnknown) || (iid == IID_IClassFactory))
	{
		*ppv = static_cast<IClassFactory*>(this) ; 
	}
	else
	{
		*ppv = NULL ;
		return E_NOINTERFACE ;
	}
	reinterpret_cast<IUnknown*>(*ppv)->AddRef() ;
	return S_OK ;
}

ULONG __stdcall CFactory::AddRef()
{
	return InterlockedIncrement(&m_cRef) ;
}

ULONG __stdcall CFactory::Release() 
{
	if (InterlockedDecrement(&m_cRef) == 0)
	{
		delete this ;
		return 0 ;
	}
	return m_cRef ;
}

//
// IClassFactory implementation
//
HRESULT __stdcall CFactory::CreateInstance(IUnknown* pUnknownOuter,
                                           const IID& iid,
                                           void** ppv) 
{
	trace("Class factory:\t\tCreate component.") ;

	// Cannot aggregate.
	if (pUnknownOuter != NULL)
	{
		return CLASS_E_NOAGGREGATION ;
	}

	// Create component.
	CA* pA = new CA ;
	if (pA == NULL)
	{
		return E_OUTOFMEMORY ;
	}

	// Get the requested interface.
	HRESULT hr = pA->QueryInterface(iid, ppv) ;

	// Release the IUnknown pointer.
	// (If QueryInterface failed, component will delete itself.)
	pA->Release() ;
	return hr ;
}

// LockServer
HRESULT __stdcall CFactory::LockServer(BOOL bLock) 
{
	if (bLock)
	{
		InterlockedIncrement(&g_cServerLocks) ; 
	}
	else
	{
		InterlockedDecrement(&g_cServerLocks) ;
	}
	return S_OK ;
}

///////////////////////////////////////////////////////////
//
// Exported functions
//
//	Not needed for MacOS because these exported functions are
//	there to be called by the Windows system, not by applications.
//

//#pragma export on
//// Can DLL unload now?
////
//BOOL DllCanUnloadNow()
//{
//	if ((g_cComponents == 0) && (g_cServerLocks == 0))
//	{
//		return S_OK ;
//	}
//	else
//	{
//		return S_FALSE ;
//	}
//}
//#pragma export off

// Get class factory
//
//#pragma export on
//STDAPI DllGetClassObject(const CLSID& clsid,
//                         const IID& iid,
//                         void** ppv)
//{
//	trace("DllGetClassObject:\tCreate class factory.") ;
//
//	// Can we create this component?
//	if (clsid != CLSID_Component1)
//	{
//		return CLASS_E_CLASSNOTAVAILABLE ;
//	}
//
//	// Create class factory.
//	CFactory* pFactory = new CFactory ;  // Reference count set to 1
//	                                     // in constructor
//	if (pFactory == NULL)
//	{
//		return E_OUTOFMEMORY ;
//	}
//
//	// Get requested interface.
//	HRESULT hr = pFactory->QueryInterface(iid, ppv) ;
//	pFactory->Release() ;
//
//	return hr ;
//}
//#pragma export off
//
//
// Server registration
//
//STDAPI DllRegisterServer()
//{
//	return RegisterServer(g_hModule, 
//	                      CLSID_Component1,
//	                      g_szFriendlyName,
//	                      g_szVerIndProgID,
//	                      g_szProgID) ;
//}


//
// Server unregistration
//
//STDAPI DllUnregisterServer()
//{
//	return UnregisterServer(CLSID_Component1,
//	                        g_szVerIndProgID,
//	                        g_szProgID) ;
//}

///////////////////////////////////////////////////////////
//
// DLL module information
//
//BOOL APIENTRY DllMain(HANDLE hModule,
//                      DWORD dwReason,
//                      void* lpReserved)
//{
//	if (dwReason == DLL_PROCESS_ATTACH)
//	{
//		g_hModule = hModule ;
//	}
//	return TRUE ;
//}
