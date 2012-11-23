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
	virtual UINT32   STDMETHODCALLTYPE AddRef() = 0;
	virtual UINT32   STDMETHODCALLTYPE Release() = 0;
};

#define IID_IUnknown __uuidof(IUnknown)

typedef IUnknown* LPUNKNOWN;

class IClassFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *punkOuter, REFIID iid, void** ppv) = 0;
};

#define IID_IClassFactory __uuidof(IClassFactory)

typedef IClassFactory* LPCLASSFACTORY;

#endif //_COMCLASSES_H_
