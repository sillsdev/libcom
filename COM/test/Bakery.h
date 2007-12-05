/*
 *	$Id$
 *
 *	Bakery COM Server example and test headers
 *
 *	MarkS - 2007-11-05
 *
 * COM Support Library Test
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

#ifndef _Bakery_h_
#define _Bakery_h_

#include "COMLibrary.h"
#include "IBakery_idl.h"

/**
 * Bakery class.
 */
class Bakery : public IBakery {

public:
	Bakery(void);
	~Bakery(void);

	// IUnknown functions
	virtual HRESULT __stdcall QueryInterface(const IID& interfaceid, void** objectInterface);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	
	virtual HRESULT getNumberCookiesBaked(long * number);
	virtual HRESULT bakeCookies(long amount, IUnknown * jar);
	
	/** Called to create the COM object. */
	static void CreateCom(IUnknown *outerAggregateIUnknown, REFIID interfaceid, void ** objectInterface);

private:
	/** Number of cookies this Bakery has baked */
	long cookiesBaked;
	/** Bakery reference count */
	volatile LONG m_referenceCount; 
};

// TODO do this a better way (checking USE_GENERIC_FACTORY)
#undef USE_GENERIC_FACTORY

#ifndef USE_FW_GENERIC_FACTORY
// CFactory class
// TODO: this should probably be in a separate file and should probably be an 
// interface or inheritable class or something that things like 
// Bakery implement. (??)
//

/**
 * Class CFactory
 */
class CFactory : public IClassFactory
{

public:
	// IUnknown functions
	virtual HRESULT __stdcall QueryInterface(const IID& interfaceid, void** objectInterface);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	// IClassFactory functions
	virtual HRESULT __stdcall CreateInstance(IUnknown* outerAggregateIUnknown, const IID& interfaceid, void** objectInterface);
	virtual HRESULT __stdcall LockServer(BOOL shouldLock);

	CFactory() : m_referenceCount((LONG)1) {
		// RegisterFactory(CLSID_Bakery, static_cast<IClassFactory*>(this));
	}

	~CFactory() 
	{
	}

private:
	volatile LONG m_referenceCount;
};

#endif /* !USE_FW_GENERIC_FACTORY */

#endif /* _Bakery_h_ */
