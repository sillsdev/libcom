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

#ifndef _COMINTERFACES_H_
#define _COMINTERFACES_H_

#include "COM.h"

class IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppv) = 0;
	virtual ULONG   STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG   STDMETHODCALLTYPE Release() = 0;
};

#define IID_IUnknown __uuidof(IUnknown)

typedef IUnknown* LPUNKNOWN;

class IClassFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *punkOuter, REFIID iid, void** ppv) = 0;
};

typedef IClassFactory* LPCLASSFACTORY;

class IDataObject : public IUnknown {};
class IDispatch : public IUnknown {};
class IStorage : public IUnknown {};

class IRpcStubBuffer;	// Forward declaration
class IRpcChannelBuffer;	// Forward declaration
class IEnumSTATDATA;		// Forward declaration
class IAdviseSink;		// Forward declaration
class IRecordInfo;		// Forward declaration

class IEnumFORMATETC : public IUnknown {};
class IShellFolder : public IUnknown {};

#define IID_IClassFactory __uuidof(IClassFactory)
#define IID_IDataObject __uuidof(IDataObject)
#define IID_IDispatch __uuidof(IDispatch)
#define IID_IStorage __uuidof(IStorage)

#define IID_IRpcStubBuffer __uuidof(IRpcStubBuffer)
#define IID_IRpcChannelBuffer __uuidof(IRpcChannelBuffer)
#define IID_IEnumSTATDATA __uuidof(IEnumSTATDATA)
#define IID_IAdviseSink __uuidof(IAdviseSink)
#define IID_IRecordInfo __uuidof(IRecordInfo)

#define IID_IEnumFORMATETC __uuidof(IEnumFORMATETC)
#define IID_IShellFolder __uuidof(IShellFolder)

#endif //_COMCLASSES_H_
