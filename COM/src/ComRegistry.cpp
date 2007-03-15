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
#include "COMLibrary.h"
#include <iostream>

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
	
		CLSID clsid(classid.c_str());

		m_componentMap[clsid].dllfilename = dllfilename;
	}
	dllmapfilestream.close();

#if DUMP_COM_REGISTRY
	std::cerr << "Registry created:\n";
	dumpComponentMap(std::cerr);
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
ComRegistry* ComRegistry::getMutableInstance()
{
	static ComRegistry instance;
	return &instance;
}
#pragma export off

/**
 * @brief Register a class factory.
 * 
 * Adds an entry to the registry for a class and its factory pointer.
 * @param classID class ID to register
 * @param classFactory class factory that can create objects of class ID classID.
 */
#pragma export on
void ComRegistry::registerFactory(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	m_componentMap[classID].factory = classFactory;

#if DUMP_COM_REGISTRY
	std::cerr << "Registry updated:\n";
	dumpComponentMap(std::cerr);
#endif
}
#pragma export off

/**
 * @brief Get a class factory that can produce classes of a certain classID.
 * 
 * @param classID Class ID that you want a class factory for
 * @return pointer to class factory corresponding to classID, or NULL if not found
 * @see ComRegistry::Register
 */
LPCLASSFACTORY ComRegistry::getFactory(const CLSID &classID) {
	
	ComponentMap::const_iterator where = this->m_componentMap.find(classID);
	
	if (where != m_componentMap.end())
	{
		return (*where).second.factory;
	}
	return NULL;
}

#pragma export on
/**
 * @brief Get a class factory for a class out of the registry, possibly loading a necessary DLL file.
 * 
 * If classID is not found in the registry, we look in the component map, find and open the corresponding DLL file, 
 * the DLL registers its COM objects or we will, and we return the class factory for classID.
 * If we fail to get the desired factory, then classFactory is left unchanged.
 * 
 * @return S_OK upon successfully getting a class factory, or if the class factory is not already in the registry: REGDB_E_CLASSNOTREG if classID is not in the DLL Map, REGDB_E_CLASSNOTREG if the DLL filename in the DLL Map is empty (unspecified), REGDB_E_CLASSNOTREG if there was an error dlopen()ing the DLL file, REGDB_E_CLASSNOTREG if there was a problem registering a factory for classID in the DLL, or REGDB_E_CLASSNOTREG if we really ultimately fail to find the class factory.
 * @return E_OUTOFMEMORY upon running out of memory while creating the class factory
 * @return E_NOINTERFACE if the class does not support the requested interface
 * @return CLASS_E_CLASSNOTAVAILABLE if the DLL does not support the requested class id, though the dll map file claimed it did
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject and we never registered the factory
 */
HRESULT ComRegistry::getFactoryPointer(const CLSID &classID, LPCLASSFACTORY* classFactory)
{
	// Look for the factory pointer, by Class ID, in the COM registry
	LPCLASSFACTORY resultFactory = NULL;
	if (resultFactory = getFactory(classID)) {
		*classFactory = resultFactory;
		return S_OK;
	}

	// No factory claims to support the Class ID. 

	// If we know where the DLL is, dlopen it to get it registered, 
	// and try again.

	// Find the DLL file according to classID in the dllmap
	string dllfilename;
	try {
		dllfilename = getDllFilename(classID);
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
		fprintf(stderr, "COM Support Library: Warning: error loading DLL file '%s' in ComRegistry::getFactoryPointer: %s\n", dllfilename.c_str(), dllerror);
		return REGDB_E_CLASSNOTREG;
	}

	// Get a factory in the COM object we just opened.
	IClassFactory* tempFactory=NULL;
	HRESULT hr = findFactoryInDll(dllhandle, classID, &tempFactory);
	if (FAILED(hr))
		return hr;
	
	// Register the obtained class factory
	RegisterFactory(classID, tempFactory);

	// classID should now be registered (either by it calling RegisterFactory, or by us registering it for it), so try again.
	if (resultFactory = getFactory(classID)) {
		*classFactory = resultFactory;
		return S_OK;
	}
	return REGDB_E_CLASSNOTREG;
}

/**
 * @brief Converts a pointer to a hexadecimal NUL terminated representation in the 11 byte buffer buf.
 * 
 * @param pointer pointer to convert
 * @param buf 11 byte buffer to which to write the hexadecimal representation
 */
void ComRegistry::pointerToHex(const void* pointer, char *buf)
{
	static	wchar_t hex_digits[] = L"0123456789ABCDEF";
	char* p=buf;
	union {
		const void*  	cp;
		unsigned long	ci;
	};
	
	cp = pointer;
	
	*p++ = '0'; *p++ = 'x';
	for (int i=0; i<8; i++)
	{
		*p++ = hex_digits[(ci >> 28)];
		ci = ci << 4;
	}
	*p = '\0';
}

/**
 *	@brief Find DLL filename corresponding to Class ID in the component map.
 * 	@param classID Class ID matching a DLL file in the dllmap
 * 	@return dll filename, which could be empty
 *	@throws ClassIDNotFound if classID is not in the component map
 */
string ComRegistry::getDllFilename(const CLSID &classID) {

	ComponentMap::const_iterator where = m_componentMap.find(classID);
	if (where == m_componentMap.end()) {
		throw ClassIDNotFound();
	}

	string dllfilename = (*where).second.dllfilename;
	
	return dllfilename;
}

/**
 * @brief Output the component map
 * 
 * @param out output stream to which to write data
 */
#pragma export on
void ComRegistry::dumpComponentMap(std::ostream& out)
{
	for (ComponentMap::const_iterator iterator = m_componentMap.begin(); iterator != m_componentMap.end(); ++iterator)
	{
		GUID guid = iterator->first;
		LPCLASSFACTORY factory = iterator->second.factory;
		string dllfilename = iterator->second.dllfilename;
		
		out << guid.str() << " -> " << factory << ", " << dllfilename << "\n";
	}
}
#pragma export off

/**
 * @brief Find a class factory for requestedClassID from an open COM DLL file by calling its DllGetClassObject function.
 * 
 * There could be additional HRESULTs returned than those specified here.
 *
 * @param dllhandle handle to an open COM DLL file
 * @param requestedClassID class ID to get a factory for
 * @param factory receives a class factory able to generate objects with class id requestedClassID
 * @return E_OUTOFMEMORY upon running out of memory while creating the class factory
 * @return E_NOINTERFACE if the class does not support the requested interface
 * @return CLASS_E_CLASSNOTAVAILABLE if the DLL does not support the requested class id
 * @return REGDB_E_CLASSNOTREG if there was an error calling DllGetClassObject (TODO a better HRESULT should probably be used here).
 * @return S_OK upon success
 */
HRESULT ComRegistry::findFactoryInDll(void* dllhandle, REFCLSID requestedClassID, IClassFactory** factory) {

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
	HRESULT hr = (*DllGetClassObject)(requestedClassID, IID_IClassFactory, (VOID**)factory);
	
	if (FAILED(hr))
		return hr;
	if (NULL == factory || NULL == *factory) {
		return REGDB_E_CLASSNOTREG;
	}

	return S_OK;
}
#pragma export off
