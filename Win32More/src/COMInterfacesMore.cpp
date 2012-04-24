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

DEFINE_UUIDOF(IErrorInfo,0x1CF2B120,0x547D,0x101B,0x8E,0x65,0x08,0x00,0x2B,0x2B,0xD1,0x19);
DEFINE_UUIDOF(ICreateErrorInfo,0x22F03340,0x547D,0x101B,0x8E,0x65,0x08,0x00,0x2B,0x2B,0xD1,0x19);
DEFINE_UUIDOF(ISupportErrorInfo,0xDF0B3D60,0x548F,0x101B,0x8E,0x65,0x08,0x00,0x2B,0x2B,0xD1,0x19);

DEFINE_UUIDOF(ISequentialStream,0x0c733a30,0x2a1c,0x11ce,0xad,0xe5,0x00,0xaa,0x00,0x44,0x77,0x3d);
DEFINE_UUIDOF(IStream,0x0000000c,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

DEFINE_UUIDOF(IPicture,0x7BF80980,0xBF32,0x101A,0x8B,0xBB,0x00,0xAA,0x00,0x30,0x0C,0xAB);
DEFINE_UUIDOF(IAccessible,0x618736e0,0x3c3d,0x11cf,0x81,0x0c,0x00,0xaa,0x00,0x38,0x9b,0x71);
DEFINE_UUIDOF(IServiceProvider,0x6d5140c1,0x7436,0x11ce,0x80,0x34,0x00,0xaa,0x00,0x60,0x09,0xfa);
DEFINE_UUIDOF(IEnumVARIANT,0x00020404,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(ITypeInfo,0x00020401,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

// Global Thread local storage object
__thread IErrorInfo* g_ErrorInfo;

/* Store the passed IErrorInfo* into Thread local storage g_ErrorInfo.
   If IErrorInfo* not NULL increment ref count by calling QueryInterface.
   If g_ErrorInfo already contains a non-null value decrement existing
   values ref count.
 */
STDAPI_IMPL SetErrorInfo(UINT32 dwReserved, IErrorInfo* perrinfo)
{
	if (g_ErrorInfo != NULL)
		g_ErrorInfo->Release();

	if (perrinfo != NULL)
		perrinfo->QueryInterface(IID_IErrorInfo, (void**)&g_ErrorInfo);
	else
		g_ErrorInfo = NULL;

	return S_OK;
}

STDAPI_IMPL GetErrorInfo(UINT32 dwReserved, IErrorInfo** pperrinfo)
{
	*pperrinfo = g_ErrorInfo;
	g_ErrorInfo = NULL;
	return *pperrinfo != NULL ? S_OK : S_FALSE;
}

STDAPI_IMPL CreateErrorInfo(ICreateErrorInfo** pperrinfo)
{
	*pperrinfo = new ErrorObjects::ErrorInfo();
	return *pperrinfo ? S_OK : E_OUTOFMEMORY;
}


