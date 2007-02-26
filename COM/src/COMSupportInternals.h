/*
 *	$Id$
 *
 *	COM Support Library internals
 *
 *	Graeme Costin - 2001-08
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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


#include <map>
#include <string>
using std::string;

struct factory_dllfilename_pair {
	LPCLASSFACTORY factory;
	string dllfilename;
};

typedef std::map<CLSID, factory_dllfilename_pair> ComponentMap;

HRESULT registerFactoryInDLL(void* dllhandle, REFCLSID requestedClassID, REFIID factoryInterfaceID = IID_IClassFactory);
void dump_component_map(ComponentMap component_map, std::ostream& out);
string getDLLFilename(const CLSID &, const ComponentMap&);

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
 * 
 * TODO: update comment
 */
class ComRegistry
{
	public:
	
		ComRegistry();
	
		~ComRegistry();

	private:

		static ComRegistry* GetMutableInstance();
		LPCLASSFACTORY getFactory(const CLSID &);

		/** CLSID to Class Factory and Dll Filename mapping */
		ComponentMap component_map;
		
	public:

		static ComRegistry* GetInstance() { return GetMutableInstance(); }
	
		void Register(const CLSID &Class, LPCLASSFACTORY Pointer);

		HRESULT GetFactoryPtr(const CLSID& Class, LPCLASSFACTORY* pIFactory) const;
		
		static void PtrToHex(const void* Ptr, char *buf);

};
