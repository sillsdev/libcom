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


#ifndef COMSupportInternals_h
#define COMSupportInternals_h

#include <map>
#include <string>
using std::string;

/** Thrown if a specific ClassID was not found (in the component map) */
class ClassIDNotFound : std::exception {};

struct factory_dllfilename_pair {
	LPCLASSFACTORY factory;
	string dllfilename;
};

typedef std::map<CLSID, factory_dllfilename_pair> ComponentMap;

HRESULT registerFactoryInDLL(void* dllhandle, REFCLSID requestedClassID, REFIID factoryInterfaceID = IID_IClassFactory);
void dump_component_map(ComponentMap component_map, std::ostream& out);
string getDLLFilename(const CLSID &, const ComponentMap&);
GUID mangleGuid(GUID guid);

#endif // COMSupportInternals_h
