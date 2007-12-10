/*
 *	$Id$
 *
 *	Ball COM Server example and test headers
 *
 *	MarkS - 2005-01
 *
 * Ball COM Support Library Test
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

#ifndef _Ball_h_
#define _Ball_h_

#include "COMLibrary.h"
#include "IBall_idl.h"

/**
 * Ball class.
 */
class Ball : public IBall {

public:
	Ball(void);
	~Ball(void);

	// IUnknown functions
	virtual HRESULT __stdcall QueryInterface(const IID& interfaceid, void** objectInterface);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	
	// Bounce this ball
	virtual HRESULT bounce(void);
	// Roll this ball a distance
	virtual HRESULT roll(long distance, long *total);

	// Called to create the COM object.
	static void CreateCom(IUnknown *outerAggregateIUnknown, REFIID interfaceid, void ** objectInterface);

private:
	/** Distance this ball has rolled */
	long distanceRolled;
	/** Number of times this ball has been bounced */
	long timesBounced;
	/** Ball reference count */
	volatile LONG m_referenceCount; 
};

#ifndef USE_FW_GENERIC_FACTORY
// CFactory class
// TODO: this should probably be in a separate file and should probably be an 
// interface or inheritable class or something that things like 
// Ball implement. (??)
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
		// RegisterFactory(CLSID_Ball, static_cast<IClassFactory*>(this));
	}

	~CFactory() 
	{
	}

private:
	volatile LONG m_referenceCount;
};

#endif /* !USE_FW_GENERIC_FACTORY */

#endif /* _Ball_h_ */
