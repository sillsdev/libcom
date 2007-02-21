/*
 *	$Id$
 *
 *	COM Support Library internals
 *
 *	Graeme Costin - 2001-08-07
 *
 * COM Support Library
 * Copyright (C) 2007 SIL
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

/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupportInternals.h
Responsibility: Graeme Costin
Last reviewed: never

	Internal details of support for COM in FieldWorks on MacOS.

Added a DLL Map
	2006-12-22, MarkS
Commentary improved after loading of both shared and dynamic libraries was worked out.
	2003-06-21, GDLC
Moved internal implementation definitions to here from COMSupport.h
	2001-07-19, GDLC
Added GetMutableInstance() for private use
	2001-08-07, GDLC
----------------------------------------------------------------------------------------------*/

#include <map>
#include <string>
using std::string;

typedef std::map<CLSID, LPCLASSFACTORY>	ComMap;
typedef std::map<CLSID, string>	DllMap;

void registerFactoryInDLL(void* dllhandle, REFCLSID requestedClassID, REFIID factoryInterfaceID = IID_IClassFactory);

/** 
 * ComRegistry class.
 * 
 * This class implements a non-persistent registry of COM classes which is built as an
 * application using it loads itself and the shared libraries it uses. Since this
 * registry is re-built every time an application launches it is always up-to-date
 * with the latest versions of the shared libraries.
 * 
 * The single instance of the COM registry is constructed in response to static items
 * in the shared libraries. This works fine as long as the shared or dynamic libraries
 * are actually loaded into the program's memory space; different APIs are needed for shared
 * libraries on MacOS 9 compared to dynamic libraries on MacOS X.
 */
class ComRegistry : public ComMap
{
	public:
	
		ComRegistry();
	
		~ComRegistry();

	private:

		static ComRegistry* GetMutableInstance();

		static void PtrToHex(const void* Ptr, char *buf);

		/** CLSID to DLL mapping */	
		DllMap dllmap; // Should this be static?

		
		
	public:

		static ComRegistry* GetInstance() { return GetMutableInstance(); }
	
		void Register(const CLSID &Class, LPCLASSFACTORY Pointer);

		HRESULT GetFactoryPtr(const CLSID& Class, LPCLASSFACTORY* pIFactory) const;

		void Dump(std::ostream& out) const;
};
