/*
 *	$Id$
 *
 *	Wall COM Server example and test headers
 *
 *	MarkS - 2008-02-08
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

#ifndef _Wall_h_
#define _Wall_h_

#include "COMLibrary.h"
#include "IWall.h"
#include <unicode/unistr.h>

/**
 * Wall class.
 */
class Wall : public IWall {

public:
	Wall(void);
	~Wall(void);

	// IUnknown functions
	virtual HRESULT __stdcall QueryInterface(const IID& interfaceid, void** objectInterface);
	virtual UINT32 __stdcall AddRef();
	virtual UINT32 __stdcall Release();

	virtual HRESULT readWallBSTR(BSTR* text);
	virtual HRESULT writeBSTROnWall(BSTR text);
	HRESULT readWall(int,OLECHAR*);
	HRESULT writeOnWall(int,OLECHAR*);
	HRESULT writeOnWall2(int,const OLECHAR*);
	HRESULT writeCharStarOnWall(unsigned char*);
	HRESULT writeWcharStarOnWall(WCHAR*);

	/** Called to create the COM object. */
	static void CreateCom(IUnknown* outerAggregateIUnknown, REFIID interfaceid, void** objectInterface);

private:
	/** The writing on the wall */
	icu::UnicodeString contents;
	/** Wall reference count */
	volatile LONG m_referenceCount;
};

#ifndef USE_FW_GENERIC_FACTORY
// CFactory class
// TODO: this should probably be in a separate file and should probably be an
// interface or inheritable class or something that things like
// Wall implement. (??)
//

/**
 * Class CFactory
 */
class CFactory : public IClassFactory
{

public:
	// IUnknown functions
	virtual HRESULT __stdcall QueryInterface(const IID& interfaceid, void** objectInterface);
	virtual UINT32 __stdcall AddRef();
	virtual UINT32 __stdcall Release();

	// IClassFactory functions
	virtual HRESULT __stdcall CreateInstance(IUnknown* outerAggregateIUnknown, const IID& interfaceid, void** objectInterface);
	virtual HRESULT __stdcall LockServer(BOOL shouldLock);

	CFactory() : m_referenceCount((LONG)1) {
		// RegisterFactory(CLSID_Wall, static_cast<IClassFactory*>(this));
	}

	~CFactory()
	{
	}

private:
	volatile LONG m_referenceCount;
};

#endif /* !USE_FW_GENERIC_FACTORY */

#endif /* _Wall_h_ */
