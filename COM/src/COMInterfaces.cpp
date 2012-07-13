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

DEFINE_UUIDOF(IUnknown,0x00000000,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

DEFINE_UUIDOF(IClassFactory,0x00000001,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IDataObject,0x0000010e,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IDispatch,0x00020400,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IStorage,0x0000000b,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);

DEFINE_UUIDOF(IRpcStubBuffer,0xD5F56AFC,0x593B,0x101A,0xB5,0x69,0x08,0x00,0x2B,0x2D,0xBF,0x7A);
DEFINE_UUIDOF(IRpcChannelBuffer,0xD5F56B60,0x593B,0x101A,0xB5,0x69,0x08,0x00,0x2B,0x2D,0xBF,0x7A);

DEFINE_UUIDOF(IEnumFORMATETC,0x00000103,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IEnumSTATDATA,0x00000105,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IAdviseSink,0x0000010f,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IRecordInfo,0x0000002F,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
DEFINE_UUIDOF(IShellFolder,0x000214E6,0x0000,0x0000,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46);
