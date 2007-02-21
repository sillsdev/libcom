/*
 *	$Id$
 *
 *	Smart pointers for standard COM classes
 *
 *	Neil Mayhew - 2006-08-22
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

#ifndef _COMPOINTERS_H_
#define _COMPOINTERS_H_

#include "COMInterfaces.h"

#ifdef _COM_SMARTPTR_TYPEDEF	// Smart-pointer classes must define this
	#define _DEFINE_PTR(I) _COM_SMARTPTR_TYPEDEF(I, IID_##I)
#else
	#define _DEFINE_PTR(I) typedef I* I##Ptr
#endif

_DEFINE_PTR(IUnknown);

_DEFINE_PTR(IClassFactory);
_DEFINE_PTR(IDataObject);
_DEFINE_PTR(IDispatch);
_DEFINE_PTR(IStorage);

_DEFINE_PTR(IRpcStubBuffer);
_DEFINE_PTR(IRpcChannelBuffer);
_DEFINE_PTR(IEnumSTATDATA);
_DEFINE_PTR(IAdviseSink);
_DEFINE_PTR(IRecordInfo);

_DEFINE_PTR(IEnumFORMATETC);
_DEFINE_PTR(IShellFolder);

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

#endif //_COMPOINTERS_H_
