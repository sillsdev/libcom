/*
 *	$Id$
 *
 *	More Standard COM interfaces
 *
 *	Neil Mayhew - 2005-04-22
 *
 * Win32More Compatibility Library
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

#include <COMInterfacesMore.h>

#include "ThreadLocalStorage.h"
#include <WinError.h>
#include "ErrorObjects.h"

// Define GUIDs

#define DEFINE_INTERFACE_GUID(N, G)	\
	template<> const GUID __uuidof(N)(G)

DEFINE_INTERFACE_GUID(IErrorInfo, "1CF2B120-547D-101B-8E65-08002B2BD119");
DEFINE_INTERFACE_GUID(ICreateErrorInfo, "22F03340-547D-101B-8E65-08002B2BD119");
DEFINE_INTERFACE_GUID(ISupportErrorInfo, "DF0B3D60-548F-101B-8E65-08002B2BD119");

DEFINE_INTERFACE_GUID(ISequentialStream, "0c733a30-2a1c-11ce-ade5-00aa0044773d");
DEFINE_INTERFACE_GUID(IStream, "0000000c-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IPicture, "7BF80980-BF32-101A-8BBB-00AA00300CAB");
DEFINE_INTERFACE_GUID(IAccessible, "618736e0-3c3d-11cf-810c-00aa00389b71");
DEFINE_INTERFACE_GUID(IServiceProvider, "6d5140c1-7436-11ce-8034-00aa006009fa");
DEFINE_INTERFACE_GUID(IEnumVARIANT, "00020404-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(ITypeInfo, "00020401-0000-0000-C000-000000000046");

#undef DEFINE_INTERFACE_GUID

// Globel Thread local storage object
ThreadLocalStorage g_ErrorInfo;

/* Store the passed IErrorInfo* into Thrad local storage g_ErrorInfo
   if IerrorInfo* not null increment ref count.
   If g_ErrorInfo already contains a non null value decrement existing 
   values ref count.
 */
STDAPI SetErrorInfo(ULONG dwReserved, IErrorInfo* perrinfo)
{
	IErrorInfo* pErrorInfoOld = NULL;
	
	// g_ErrorInfo stores void** ptr value hence need to reinterpret_cast.
	void **pvoidTemp = reinterpret_cast<void **>(&pErrorInfoOld);
	
	if (g_ErrorInfo.Get(pvoidTemp) && pErrorInfoOld != NULL)
	{
		pErrorInfoOld->Release();
	}

	if (perrinfo != NULL)
	{
		perrinfo->AddRef();
	}
	
	return (g_ErrorInfo.Set(perrinfo)?S_OK:S_FALSE);		
}

STDAPI GetErrorInfo(ULONG dwReserved, IErrorInfo** pperrinfo)
{	
	// g_ErrorInfo stores void** ptr value hence need to reinterpret_cast.
	return (g_ErrorInfo.Get(reinterpret_cast<void**>(pperrinfo))?S_OK:S_FALSE);	
}

STDAPI CreateErrorInfo(ICreateErrorInfo** pperrinfo)
{
	*pperrinfo = new ErrorObjects::ErrorInfo();
	return *pperrinfo?S_OK:E_OUTOFMEMORY;
}


