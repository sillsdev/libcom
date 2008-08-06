/*
 *	$Id$
 *
 *	More smart pointers for standard COM classes
 *
 *	Neil Mayhew - 2006-08-22
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

#ifndef COMPointersMore_h
#define COMPointersMore_h

#include <COMPointers.h>
#include "COMInterfacesMore.h"

_DEFINE_PTR(IErrorInfo);
_DEFINE_PTR(ICreateErrorInfo);
_DEFINE_PTR(ISupportErrorInfo);
_DEFINE_PTR(ISequentialStream);
_DEFINE_PTR(IStream);
_DEFINE_PTR(IPicture);
_DEFINE_PTR(IAccessible);
_DEFINE_PTR(IServiceProvider);
_DEFINE_PTR(IEnumVARIANT);
_DEFINE_PTR(ITypeInfo);

#endif // COMPointersMore_h

