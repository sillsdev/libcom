/*
 *	$Id$
 *
 *	Standard COM interfaces
 *
 *	Neil Mayhew - 2005-04-22
 *
 * COM Support Library
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

#include "COMInterfaces.h"

// Define GUIDs

#define DEFINE_INTERFACE_GUID(N, G)	\
	template<> const GUID __uuidof(N)(G)

DEFINE_INTERFACE_GUID(IUnknown, "00000000-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IClassFactory, "00000001-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IDataObject, "0000010e-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IDispatch, "00020400-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IStorage, "0000000b-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IRpcStubBuffer, "D5F56AFC-593B-101A-B569-08002B2DBF7A");
DEFINE_INTERFACE_GUID(IRpcChannelBuffer, "D5F56B60-593B-101A-B569-08002B2DBF7A");

DEFINE_INTERFACE_GUID(IEnumFORMATETC, "00000103-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IEnumSTATDATA, "00000105-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IAdviseSink, "0000010f-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IRecordInfo, "0000002F-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IShellFolder, "000214E6-0000-0000-C000-000000000046");

#undef DEFINE_INTERFACE_GUID
