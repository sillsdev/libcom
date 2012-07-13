//
// Component.h
//
//	Declarations extracted from Component.cpp which was copied from Rogerson's book CD-ROM
//	and modified for CodeWarrior with COMSupportLib
//

#include <iostream>

using namespace std;

#include "windows.h"

#include "ComponentInterfaces.h"	// Interface declarations
//#include "Registry.h"		// Registry helper functions

void trace(const char* msg);

void ComponentLoaded(void);

//extern "C" void _init();

//////////////////////////////////////////////////////////
//
//	Component class declaration
//
class CA : public IX,
           public IY
{
public:
	// IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) ;
	virtual ULONG __stdcall AddRef() ;
	virtual ULONG __stdcall Release() ;

	// Interface IX
	virtual void __stdcall Fx() { cout << "Fx" << endl ;}

	// Interface IY
	virtual void __stdcall Fy() { cout << "Fy" << endl ;}

	// Constructor
	CA() ;

	// Destructor
	~CA() ;

private:
	// Reference count
	long m_cRef ;
} ;

///////////////////////////////////////////////////////////
//
// Class factory class declaration
//
class CFactory : public IClassFactory
{
public:
	// IUnknown
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) ;
	virtual ULONG   __stdcall AddRef() ;
	virtual ULONG   __stdcall Release() ;

	// Interface IClassFactory
	virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter,
	                                         const IID& iid,
	                                         void** ppv) ;
	virtual HRESULT __stdcall LockServer(BOOL bLock) ;

	// Constructor
	CFactory() : m_cRef(1)
	{
		RegisterServer (CLSID_Component1, static_cast<IClassFactory*>(this));
	}

	// Destructor
	~CFactory()
	{
		trace("Class factory:\t\tDestroy self.") ;
	}

private:
	long m_cRef ;
} ;
