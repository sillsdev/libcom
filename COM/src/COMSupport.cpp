/*
 *	$Id$
 *
 *	Main COM Support Library code
 *
 *	Graeme Costin - 2001-05
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

#include "Types.h"
#include "WinError.h"
#include "COMInterfaces.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cctype>

#include "COMSupportInternals.h"
#include "COMLibrary.h"

#include <map>
#include <dlfcn.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

#include <stdexcept>

#include "ComRegistry.h"

/**
 *	@brief Find DLL filename corresponding to Class ID in a component map.
 * 	@param classID Class ID matching a DLL file in the dllmap
 * 	@param component_map component map from which to find desired DLL filename
 * 	@return dll filename, which could be empty
 *	@throws ClassIDNotFound if classID is not in the component map
 */
string getDLLFilename(const CLSID &classID, const ComponentMap& component_map) {

	ComponentMap::const_iterator where = component_map.find(classID);
	if (where == component_map.end()) {
		throw ClassIDNotFound();
	}

	string dllfilename = (*where).second.dllfilename;
	
	return dllfilename;
}


/**
 * @brief Register a class factory for requestedClassID from an open COM DLL file by calling its DllGetClassObject function.
 * There could be additional HRESULTs returned than those specified here.
 * @param dllhandle handle to an open COM DLL file
 * @param requestedClassID class ID to get a factory for
 * @param factoryInterfaceID interface ID for IClassFactory (or an interface ID for any interface that an IClassFactory in the DLL implements)
 * @return E_OUTOFMEMORY upon running out of memory while creating the class factory
 * @return E_NOINTERFACE if the class does not support the requested interface
 * @return CLASS_E_CLASSNOTAVAILABLE if the DLL does not support the requested class id
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory (which might be a little different than REGDB_E_CLASSNOTREG is really intended for).
 * @return S_OK upon success
 */
HRESULT registerFactoryInDLL(void* dllhandle, REFCLSID requestedClassID, REFIID factoryInterfaceID /*= IID_IClassFactory*/) {

	IClassFactory* factory;
	// DllGetClassObject: http://msdn2.microsoft.com/en-us/library/ms680760.aspx
	HRESULT (*DllGetClassObject)(REFCLSID requestedClassID, REFIID requestedInterfaceID, VOID ** objectInterface);
	dlerror(); // clear any old error conditions
	*(void **) (&DllGetClassObject) = dlsym(dllhandle, "DllGetClassObject");
	const char* dllerror = dlerror();
	if (NULL != dllerror)
	{
		fprintf(stderr, "COM Support Library: Error getting COM object's DllGetClassObject function. Error: %s\n", dllerror);
		return REGDB_E_CLASSNOTREG;
	}
	// Note that if we pass factory as a null pointer, it'll not work.
	HRESULT hr = (*DllGetClassObject)(requestedClassID, factoryInterfaceID, (VOID**)&factory);
	
	if (FAILED(hr))
		return hr;
	if (NULL == factory) {
		return REGDB_E_CLASSNOTREG;
	}
	
	// Register the class factory
	RegisterServer(requestedClassID, factory);

#if DUMP_COM_REGISTRY
	// TODO Rework this after more refactoring is done
	//std::cerr << "Registry updated:\n";
	  //ComRegistry::GetInstance()->Dump(std::cout);
	//dump_component_map(component_map, std::cerr);
#endif

	return S_OK;
}
#pragma export off

inline void swap(unsigned char &a, unsigned char &b) {
	char tmp = a;
	a = b;
	b = tmp;
}

/**
 * Change a little endian GUID to a big endian GUID.
 * @param guid little endian GUID
 * @return big endian GUID based on guid
 */
GUID mangleGuid(GUID guid) {

	// this is implemented quickly and dirtily

	unsigned char tmp;
	
	//unsigned char* data = guid.str().c_str();
	unsigned char data[37];
	//const char* origGuid = guid.str().c_str();
	
	memcpy(data,reinterpret_cast<void*>(&guid),37);
	int last = sizeof(long)-1;

	for (int i=0;i< (last+1)/2 ; i++) {
		tmp = data[i];
		data[i] = data[last-i];
		data[last-i] = tmp;
	}
	
	//fprintf(stderr,"data8 is %c.\n", data[8]);
	swap(data[4],data[5]);
	swap(data[6],data[7]);
	//fprintf(stderr,"data8 is now %c.\n",data[8]);
	
	// bswap_32/16 would be better, but doesn't seem to be working for me
	
	//GUID origguidguid = origGuid;
	//GUID origguidguid;
	//CLSIDFromString(origGuid, origguidguid);
	//return origguidguid;
	
	memcpy((void*)&guid,data,37);
	return guid;
}

/**
 * Register a class factory by class ID in the internal registry.
 * "Components should call this function to register their class factory
 * with the ComRegistry map. This should be done in the constructor of the
 * server's class factory and the instance of the class factory should be
 * created as a static pointer to the class factory is initialised." Or 
 * we will register components we load.
 * @param classID class ID to register
 * @param classFactory class factory to register that can create objects of class ID classID.
 */
#pragma export on
void RegisterServer(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	ComRegistry::GetInstance()->Register(classID, classFactory);
}
#pragma export off

#ifdef DEBUG

/**
 * @brief Output the component map
 * @param component_map component map to dump
 * @param out output stream to which to write data
 */
#pragma export on
void dump_component_map(ComponentMap component_map, std::ostream& out)
{
	for (ComponentMap::const_iterator iterator = component_map.begin(); iterator != component_map.end(); ++iterator)
	{
		LPOLESTR classid;
		LPCLASSFACTORY factory;
		char factory_pointer_string[11];
		string dllfilename;
		factory_dllfilename_pair data;
		
		GUID guid = iterator->first;
		data = iterator->second;
		
		HRESULT hr = StringFromCLSID(guid, &classid);
		
		if (SUCCEEDED(hr))
			out << std::string(classid, std::find(classid, classid+999, 0));
		else
			out << "[GUID]";
		
		out << " -> ";
		
		factory = (*iterator).second.factory;
		ComRegistry::PtrToHex(factory, &factory_pointer_string[0]);
		dllfilename = (*iterator).second.dllfilename;
		out << factory_pointer_string << ", " << dllfilename << "\n";
		
		CoTaskMemFree(classid);
	}
}
#pragma export off

#endif	//	DEBUG
