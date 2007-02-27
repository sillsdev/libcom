/*
 *	$Id$
 *
 *	COM Support Library ComRegistry
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

#include "ComRegistry.h"
#include <fstream>
#include <vector>
#include "WinError.h"
#include <dlfcn.h>

 /**
 * Creates an instance of ComRegistry. ComRegistry stores a mapping between 
 * classID GUIDs and their corresponding class factories. 
 * This constructor will also populate the DLL Map from the dllmap.txt file.
 */
#pragma export on
ComRegistry::ComRegistry()
{
	// Populate the dllmap

	// Right now, dllmap.txt needs to be present in the directory from which mono was executed.
	char* dllmapfilename = "dllmap.txt";
	int linenum=1;
	std::string::size_type pos;
	std::string::size_type len;
	std::ifstream dllmapfilestream(dllmapfilename); // Maybe have a helpful message if opening fails

	if (dllmapfilestream.fail()) {
		fprintf(stderr, "COM Support Library: %s:%d ComRegistry::ComRegistry() Warning, opening of dllmap '%s' failed.\n", __FILE__,__LINE__,dllmapfilename);
	}

	// For each dllmapping line, add to the dllmap
	for (std::string line; getline(dllmapfilestream, line); linenum++) {
		pos = line.find(" ", 0);
		if (std::string::npos == pos) {
			fprintf(stderr, "COM Support Library: Warning: malformed dllmap line in file '%s' on line %d", dllmapfilename, linenum);
			continue;
		}
		len = line.length();
		std::string classid = line.substr(0,pos);
		std::string dllfilename = line.substr(pos+1, len);
	
		// Convert classid to an OLE string, then to binary
		std::vector<OLECHAR> olestr(classid.begin(), classid.end());
		olestr.push_back(0);
		CLSID clsid;
		CLSIDFromString(&olestr[0], &clsid);

		factory_dllfilename_pair data = {NULL, dllfilename};
		component_map[clsid] = data;
	}
	dllmapfilestream.close();

#if DUMP_COM_REGISTRY
	std::cerr << "Registry created:\n";
	dump_component_map(component_map, std::cerr);
#endif
}
#pragma export off

/**
 * ComRegistry destructor. Destroys an instance of ComRegistry.
 * Does nothing.
 */
#pragma export on
ComRegistry::~ComRegistry()
{
	// No explicit actions needed
}
#pragma export off
	
/**
 * Gets a pointer to a previously created registry, or to a new one if none 
 * has yet been created. The static instance of the COM registry is 
 * created here.
 */
#pragma export on
ComRegistry* ComRegistry::GetMutableInstance()
{
	static ComRegistry instance;
	return &instance;
}
#pragma export off

/**
 * @brief Register a class factory.
 * Adds an entry to the registry for a class and its factory pointer.
 * @param classID class ID to register
 * @param classFactory class factory that can create objects of class ID classID.
 */
#pragma export on
void ComRegistry::Register(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	// Preserve the old dll filename
	string old_dllfilename = "";
	try {
		old_dllfilename = getDLLFilename(classID, component_map);
	} catch(std::exception e) {
		// If getting the dll filename failed, that's okay.
	};
	
	factory_dllfilename_pair data = {classFactory, old_dllfilename};
	component_map[classID] = data;
}
#pragma export off

/**
 * @brief Get a class factory that can produce classes of a certain classID.
 * @param classID Class ID that you want a class factory for
 * @return pointer to class factory corresponding to classID, or NULL if not found
 * @see ComRegistry::Register
 */
LPCLASSFACTORY ComRegistry::getFactory(const CLSID &classID) {
	
	ComponentMap::const_iterator where = this->component_map.find(classID);
	
	if (where != component_map.end())
	{
		return (*where).second.factory;
	}
	return NULL;
}

#pragma export on
/**
 * @brief Get a class factory for a class out of the registry, possibly loading a necessary DLL file.
 * If classID is not found in the registry, we look in the component map, find and open the corresponding DLL file, 
 * the DLL registers its COM objects or we will, and we return the class factory for classID.
 * If we fail to get the desired factory, then classFactory is left unchanged.
 * @return S_OK upon successfully getting a class factory, or if the class factory is not already in the registry: REGDB_E_CLASSNOTREG if classID is not in the DLL Map, REGDB_E_CLASSNOTREG if the DLL filename in the DLL Map is empty (unspecified), REGDB_E_CLASSNOTREG if there was an error dlopen()ing the DLL file, REGDB_E_CLASSNOTREG if there was a problem registering a factory for classID in the DLL, or REGDB_E_CLASSNOTREG if we really ultimately fail to find the class factory.
 * @return E_OUTOFMEMORY upon running out of memory while creating the class factory
 * @return E_NOINTERFACE if the class does not support the requested interface
 * @return CLASS_E_CLASSNOTAVAILABLE if the DLL does not support the requested class id, though the dll map file claimed it did
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory
 */
HRESULT ComRegistry::GetFactoryPtr (const CLSID &classID, LPCLASSFACTORY* classFactory) const
{
	// Look for the factory pointer, by Class ID, in the COM registry
	LPCLASSFACTORY resultFactory = 0;
	ComRegistry* comRegistry = ComRegistry::GetInstance();
	if (resultFactory = comRegistry->getFactory(classID)) {
		*classFactory = resultFactory;
		return S_OK;
	}

	// No factory claims to support the Class ID. 

	// If we know where the DLL is, dlopen it to get it registered, 
	// and try again.

	// Find the DLL file according to classID in the dllmap
	string dllfilename;
	try {
		dllfilename = getDLLFilename(classID, component_map);
	} catch (ClassIDNotFound const& e) {
		return REGDB_E_CLASSNOTREG;
	}
	if (dllfilename.empty())
		return REGDB_E_CLASSNOTREG;

	// Load the DLL file into memory.
	// If RTLD_NODELETE is not enough to avoid undesirable things related to 
	// calling dlopen(3) more than once on the same DLL file, then 
	// RTLD_NOLOAD could be used to test if it's already loaded.
	void* dllhandle = dlopen(dllfilename.c_str(), RTLD_LAZY | RTLD_NODELETE); // Maybe use RTLD_NOW instead of RTLD_LAZY? I definitely want the global static variable to be created.

	if (!dllhandle) {
			const char* dllerror = dlerror();
			fprintf(stderr, "COM Support Library: Warning: error loading DLL file '%s' in ComRegistry::GetFactoryPtr: %s\n", dllfilename.c_str(), dllerror);
			return REGDB_E_CLASSNOTREG;
		}

	// Register the COM object we just opened.
	HRESULT hr = registerFactoryInDLL(dllhandle, classID, IID_IClassFactory);
	if (FAILED(hr))
		return hr;
	
	// classID should now be registered (either by it calling RegisterServer, or by us registering it for it), so try again.
	if (resultFactory = comRegistry->getFactory(classID)) {
		*classFactory = resultFactory;
		return S_OK;
	}
	return REGDB_E_CLASSNOTREG;
}

/**
 * @brief Converts a pointer to a hexadecimal NUL terminated representation in the 11 byte buffer buf.
 * @param Ptr pointer to convert
 * @param buf 11 byte buffer to which to write the hexadecimal representation
 */
void ComRegistry::PtrToHex(const void* Ptr, char *buf)
{
	static	wchar_t hex_digits[] = L"0123456789ABCDEF";
	char* p=buf;
	union {
		const void*  	cp;
		unsigned long	ci;
	};
	
	cp = Ptr;
	
	*p++ = '0'; *p++ = 'x';
	for (int i=0; i<8; i++)
	{
		*p++ = hex_digits[(ci >> 28)];
		ci = ci << 4;
	}
	*p = '\0';
}
