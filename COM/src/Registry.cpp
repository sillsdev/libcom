/*
 *	$Id$
 *
 *	COM Support Library Registry
 *
 *	MarkS - 2007-02-26
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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

#include "BasicTypes.h"
#include "COMError.h"
#include "COM.h"
#include "COMPointers.h"
#include "COMSupportInternals.h"
#include "ComRegistry.h"
#include "COM.h"

#define DLLEXPORT // Define properly if ever needed

/**
 * CoInitialize.
 *
 * Do nothing, but be here for when Mono wants to call CoInitialize.
 *
 * (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp)
 * @param unused NULL
 * @return S_OK
 */
DLLEXPORT
extern "C" HRESULT CoInitialize(LPVOID unused)
{
	return S_OK;
}


// Closes the COM Library on the current apartment.
DLLEXPORT
HRESULT CoUninitialize()
{
	return S_OK;
}

/**
 * @brief Initializes the COM library from the current Apartment and sets the concurrency model as single-threaded apartment.
 * @param pvReserved unused
 */
DLLEXPORT
HRESULT OleInitialize(LPVOID pvReserved)
{
	return CoInitialize(NULL);
}

/**
 * @brief Closes the COM library on the apartment.
 */
DLLEXPORT
void OleUninitialize()
{

}

/**
 * @brief Get a class factory able to create objects of class ID requestedClassID.
 *
 * NOTE: This function does NOT do what the MSDN spec actually says it does. Fix this is if that's important.
 * Note: The caller is responsible for releasing the class factory.
 * Note: This implementation is intended for use for inprocess COM only.
 * http://msdn2.microsoft.com/en-us/library/ms684007.aspx
 * @param requestedClassID class ID for which you want a class factory
 * @param dwClsContext should be CLSCTX_INPROC
 * @param factoryInterface receives the desired class factory, of type IClassFactory
 * @return S_OK upon success, CO_E_NOT_SUPPORTED upon being given an unsupported context, or REGDB_E_CLASSNOTREG upon a bunch of means of failure
 * @return E_OUTOFMEMORY upon running out of memory while creating the class factory
 * @return E_NOINTERFACE if the class does not support the requested interface
 * @return CLASS_E_CLASSNOTAVAILABLE if the DLL does not support the requested class id, though the dll map file claimed it did
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory
 */
DLLEXPORT
HRESULT CoGetClassObject(REFCLSID requestedClassID, DWORD dwClsContext, LPVOID,
	REFIID /*requestedInterfaceID*/, LPVOID* factoryInterface)
{
	if (dwClsContext != CLSCTX_INPROC) {
		return CO_E_NOT_SUPPORTED;
	}

	IClassFactory* factory = NULL;
	HRESULT	hr = ComRegistry::getInstance()->getFactoryPointer(requestedClassID, &factory);
	if (SUCCEEDED(hr))
	  factory->AddRef();

	*factoryInterface = factory;
	return hr;
}

/**
 * @brief Create an instance of a class of class ID requestedClassID, which implements interface ID objectInterfaceID, and will be accessible through the interface objectInterface.
 *
 * NOTE: This implementation is intended for use for inprocess COM only.
 * http://msdn2.microsoft.com/en-us/library/ms686615.aspx
 * @param requestedClassID class ID of class that will be created and given access to through objectInterface
 * @param outerAggregateIUnknown should be NULL since we don't support aggregation
 * @param objectInterfaceID interface ID of the interface through which you desire to interact with class of class ID requestedClassID
 * @param objectInterface receives the interface to the desired instantiated class, or is NULL if the desired instantiated class does not support objectInterfaceID or we otherwise fail
 * @return S_OK upon success, E_OUTOFMEMORY if we failed to create the object due to insufficient memory, CLASS_E_NOAGGREGATION if outerAggregateIUnknown is not NULL, E_NOINTERFACE if the object does not support objectInterfaceID, or REGDB_E_CLASSNOTREG upon a bunch of other means of failure (like, if we don't know how to make a requestedClassID).
 * @return REGDB_E_CLASSNOTREG upon a bunch of means of failure
 * @return CLASS_E_CLASSNOTAVAILABLE if a DLL does not support the requested class id, though the dll map file claimed it did
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory
 */
DLLEXPORT
extern "C" HRESULT CoCreateInstance(REFCLSID requestedClassID,
	LPUNKNOWN outerAggregateIUnknown, DWORD /*dwClsContext*/,
	REFIID objectInterfaceID, LPVOID* objectInterface)
{
	*objectInterface = NULL;

	IClassFactory* pIFactory = NULL;
	HRESULT hr = CoGetClassObject(requestedClassID, static_cast<unsigned long>(CLSCTX_INPROC),
		(void *)0, IID_IClassFactory, (void **)&pIFactory);
	if (SUCCEEDED(hr))
	{
		// Create the component
		hr = pIFactory->CreateInstance(outerAggregateIUnknown, objectInterfaceID, objectInterface);
		// Release the class factory
		pIFactory->Release();
	}

	return hr;
}

DLLEXPORT
void CoFreeUnusedLibraries()
{
	// no-op
}
