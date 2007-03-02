/*
 *	$Id$
 *
 *	COM Support Library ComRegistry declarations
 *
 *	MarkS - 2007-02-27
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

#ifndef ComRegistry_h
#define ComRegistry_h

#include "COMInterfaces.h"
#include "COMSupportInternals.h"
#include <stdexcept>

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

		/** get_instance */
		static ComRegistry* get_instance() { return get_mutable_instance(); }
		static void pointer_to_hex(const void* pointer, char *buf);
		void register_factory(const CLSID &Class, LPCLASSFACTORY Pointer);
		HRESULT get_factory_pointer(const CLSID& Class, LPCLASSFACTORY* pIFactory);
		string get_dll_filename(const CLSID &);
		void dump_component_map(std::ostream& out);
		HRESULT find_factory_in_dll(void* dllhandle, REFCLSID requestedClassID, IClassFactory** factory);

		/** Base class of all ComRegistry exceptions */
		class ComRegistryException : std::runtime_error {
		public:
			ComRegistryException() : std::runtime_error("libcom ComRegistry exception") {}
		};
		/** Thrown if a specific ClassID was not found (in the component map) */
		class ClassIDNotFound : ComRegistryException {};

	private:

		static ComRegistry* get_mutable_instance();
		LPCLASSFACTORY get_factory(const CLSID &);

		/** CLSID to Class Factory and Dll Filename mapping */
		ComponentMap component_map;
};

#endif // ComRegistry_h
