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
#include "COMError.h"
#include <dlfcn.h>
#include "COMLibrary.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

using namespace std;
using std::vector;

/**
 * Creates an instance of ComRegistry. ComRegistry stores a mapping between
 * classID GUIDs and their corresponding class factories.
 * This constructor will also populate the component map from the disk file(s).
 */
ComRegistry::ComRegistry()
{
	componentsMapPathEnvironmentKey = "COMPONENTS_MAP_PATH";
	componentsMapFilename = "components.map";
	componentsMapPathDelimiter = ":";
	directorySeparator = "/";
	componentsMapCommentIndicator = "#";

	populateComponentMap();

#if DUMP_COM_REGISTRY
	std::cerr << "Registry created:\n";
	dumpComponentMap(std::cerr);
#endif
}

/**
 * ComRegistry destructor. Destroys an instance of ComRegistry.
 * Does nothing.
 */
ComRegistry::~ComRegistry()
{
	// No explicit actions needed
}

/**
 * Gets a pointer to a previously created registry, or to a new one if none
 * has yet been created. The static instance of the COM registry is
 * created here.
 */
ComRegistry* ComRegistry::getMutableInstance()
{
	static ComRegistry instance;
	return &instance;
}

/**
 * @brief Register a class factory.
 *
 * Adds an entry to the registry for a class and its factory pointer.
 * @param classID class ID to register
 * @param classFactory class factory that can create objects of class ID classID.
 */
void ComRegistry::registerFactory(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	m_componentMap[classID].factory = classFactory;

#if DUMP_COM_REGISTRY
	std::cerr << "Registry updated:\n";
	dumpComponentMap(std::cerr);
#endif
}

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
	if (LPCLASSFACTORY resultFactory = getFactory(classID)) {
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
	if (LPCLASSFACTORY resultFactory = getFactory(classID)) {
		*classFactory = resultFactory;
		return S_OK;
	}
	return REGDB_E_CLASSNOTREG;
}

/**
 * @brief Get the Assembly Filename associated with the CLSID
 *
 * @return S_OK upon successfully getting the assembly filename.
 */
HRESULT ComRegistry::getAssemblyFilename(const CLSID& Class, std::string& assemblyFilename) const
{
	ComponentMap::const_iterator where = this->m_componentMap.find(Class);

	if (where != m_componentMap.end())
	{
		assemblyFilename = (*where).second.assemblyFilename;
		return S_OK;
	}

	// This method is designed to be used by the Factory itself, so Class should always be found.
	return REGDB_E_CLASSNOTREG;
}

/**
 * @brief Get the Class Name associated with the CLSID
 *
 * @return S_OK upon successfully getting the managed class name.
 */
HRESULT ComRegistry::getClassName(const CLSID& Class, std::string& className) const
{
	ComponentMap::const_iterator where = this->m_componentMap.find(Class);

	if (where != m_componentMap.end())
	{
		className = (*where).second.className;
		return S_OK;
	}

	// This method is designed to be used by the Factory itself, so Class should always be found.
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
void ComRegistry::dumpComponentMap(std::ostream& out)
{
	for (ComponentMap::const_iterator iterator = m_componentMap.begin(); iterator != m_componentMap.end(); ++iterator)
	{
		string guid = iterator->first.str();
		LPCLASSFACTORY factory = iterator->second.factory;
		string dllfilename = iterator->second.dllfilename;

		out << guid << " -> " << factory << ", " << dllfilename << "\n";
	}
}

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
	typedef HRESULT (*GetClassObjectFunc)(REFCLSID requestedClassID, REFIID requestedInterfaceID, VOID** objectInterface);
	dlerror(); // clear any old error conditions
	GetClassObjectFunc dllGetClassObject = (GetClassObjectFunc)dlsym(dllhandle, "DllGetClassObject");
	if (const char* error = dlerror())
	{
		fprintf(stderr, "COM Support Library: Error getting COM object's DllGetClassObject function. Error: %s\n", error);
		return REGDB_E_CLASSNOTREG;
	}
	// Note that if we pass factory as a null pointer, it'll not work.
	HRESULT hr = (*dllGetClassObject)(requestedClassID, IID_IClassFactory, (VOID**)factory);

	if (FAILED(hr))
		return hr;
	if (NULL == factory || NULL == *factory) {
		return REGDB_E_CLASSNOTREG;
	}

	return S_OK;
}

/**
 * @brief Add GUID-dllfilename mappings to the component map based on entries in a components-map file.
 *
 * # Comment lines are ignored
 *
 * @param mapfilename components-map file to process
 */
void ComRegistry::populateFromComponentsMapFile(const string mapfilename)
{
	std::ifstream dllmapfilestream(mapfilename.c_str());
	int errorNumber = errno;
	if (dllmapfilestream.fail()) {
		// Don't print an error if it's just that the file doesn't exist
		if (ENOENT != errorNumber)
		{
			const int bufLength=1024;
			char errorMessage[bufLength];
			char *actualErrorMessage = strerror_r(errorNumber, errorMessage, bufLength);
			fprintf(stderr,
				"libcom: Warning: opening of components-map '%s' failed: %s\n",
				mapfilename.c_str(), actualErrorMessage);
		}
		return;
	}

	// For each valid dllmapping line, add to the component map
	int linenum=1;
	for (std::string line; getline(dllmapfilestream, line); linenum++)
	{
		// Ignore comments (lines beginning with #)
		if (componentsMapCommentIndicator == line.substr(0,1))
			continue;

		// Ignore empty lines
		if (line.empty())
			continue;

		// Split the line into GUID and DLL Filename parts + optional  assemblyFilename and className
		string::size_type sep1 = string::npos; // Seperator between classID and dllfilename
		string::size_type sep2 = string::npos; // Sepeator between dllfilename and assemblyFilename (may equal string::npos)
		string::size_type sep3 = string::npos; // Sepeator between assemblyFilename and className (may equal string::npos)
		string::size_type sep4 = string::npos; // Sepeator between className end of line (should equal string::npos)

		sep1 = line.find(" ", 0);

		if (sep1 != string::npos)
			sep2 = line.find(" ", sep1 + 1);

		if (sep2 != string::npos)
			sep3 = line.find(" ", sep2 + 1);

		if (sep3 != string::npos)
			sep4 = line.find(" ", sep3 + 1);

		if (string::npos == sep1) {
			fprintf(stderr, "libcom: Warning: malformed line (no space) at %s:%d\n",
				mapfilename.c_str(), linenum);
			continue;
		}

		if (string::npos != sep4) {
			fprintf(stderr, "libcom: Warning: malformed line (extra characters at end of line) at %s:%d\n",
				mapfilename.c_str(), linenum);
		}
		string classIdString;
		string dllfilename;
		string assemblyFilename; // optional parameter used when creating managed COM Objects
		string className; // should only exist if assemblyFilename is specified

		classIdString = line.substr(0, sep1);

		if (string::npos != sep1)
			dllfilename = line.substr(sep1 + 1, sep2 - sep1 -1);

		if (string::npos != sep2)
			assemblyFilename = line.substr(sep2 + 1, sep3 - sep2 -1);

		if (string::npos != sep3)
			className = line.substr(sep3 + 1, sep4 - sep3 - 1);

		if (classIdString.empty())
		{
			fprintf(stderr,
				"libcom: Warning: malformed line; can't find class id at %s:%d\n",
				mapfilename.c_str(), linenum);
			continue;
		}

		if (dllfilename.empty())
		{
			fprintf(stderr,
				"libcom: Warning: malformed line; can't find filename at %s:%d\n",
				mapfilename.c_str(), linenum);
			continue;
		}

		if (!assemblyFilename.empty() && className.empty())
		{
			fprintf(stderr,
				"libcom: Warning: malformed line; assembly name specified without classname %s:%d\n",
				mapfilename.c_str(), linenum);
			continue;
		}

		CLSID clsid;
		try {
			clsid.initialize(classIdString.c_str());
		}
		catch (std::runtime_error) {
			fprintf(stderr, "libcom: Warning: malformed GUID string at %s:%d\n",
				mapfilename.c_str(), linenum);
			continue;
		}

		// Make dllfilename absolute

		// If dllfilename is not yet absolute, make it based on the
		// location of the containing components-map file rather than the cwd
		// before making it absolute.
		if (directorySeparator != dllfilename.substr(0,1)) // if not begin with /
		{
			char* mapfilename_tmp = strdup(mapfilename.c_str());
			if (NULL == mapfilename_tmp)
			{
				fprintf(stderr,
					"libcom: Warning: Insufficient memory available to duplicate a string :P\n");
				continue;
			}
			// Note that dirname(3) may modify its argument and its return is
			// statically allocated and probably not threadsafe.
			string mapfilename_dirname(dirname(mapfilename_tmp)); //TODO any error checking?
			free(mapfilename_tmp);

			dllfilename = mapfilename_dirname + directorySeparator + dllfilename;

			// Make dllfilename an absolute pathname
			char* absoluteDllPath_tmp = canonicalize_file_name(dllfilename.c_str()); // TODO won't this still not work unless I cd to the components-map directory?
			errorNumber = errno;
			if (NULL == absoluteDllPath_tmp)
			{
				const int bufLength = 1024;
				char errorMessage[bufLength];
				char *actualErrorMessage = strerror_r(errorNumber, errorMessage, bufLength);
				fprintf(stderr,
					"libcom: Warning: error at %s:%d making dll filename '%s' into an absolute path: %s\n",
					mapfilename.c_str(), linenum, dllfilename.c_str(), actualErrorMessage);
				continue;
			}
			dllfilename = string(absoluteDllPath_tmp);
			free(absoluteDllPath_tmp);
		}

		// Associate the GUID with the dll file in the components-map unless
		// the GUID is already in the component map.

		if (m_componentMap.end() != m_componentMap.find(clsid))
		{
			fprintf(stderr,
				"libcom: Warning: duplicate class id '%s' at %s:%d not readded to component map.\n",
				classIdString.c_str(), mapfilename.c_str(), linenum);
			continue;
		}

		m_componentMap[clsid].dllfilename = dllfilename;
		m_componentMap[clsid].assemblyFilename = assemblyFilename;
		m_componentMap[clsid].className = className;
	}
	dllmapfilestream.close();
}

/**
 * @brief Populate the component map
 *
 * Populate the GUID-filename part of the component map based on the
 * components-map file in each directory listed in COMPONENTS_MAP_PATH.
 *
 * COMPONENTS_MAP_PATH would look like LD_LIBRARY_PATH, as a
 * colon-delimited list of directories, such as:
 *   dir/dir:../dir:dir:/dir
 *
 * If COMPONENTS_MAP_PATH is NULL or empty, the component map will be loaded from the current
 * directory.
 */
void ComRegistry::populateComponentMap()
{
	// Get the paths from the environment, or empty string if NULL.
	const char* value_tmp = getenv(componentsMapPathEnvironmentKey.c_str());
	string paths = value_tmp ? value_tmp : "";

	// If the environment value was NULL or empty string, try to use the current directory
	if (paths.empty())
	{
		populateFromComponentsMapFile(componentsMapFilename);
		return;
	}

	// Process each path in the environment value.
	string::size_type location = 0;
	do {
		string::size_type delimiterPos =
			paths.find(componentsMapPathDelimiter, location);
		string::size_type tokenLen =
			paths.npos == delimiterPos ? paths.npos : delimiterPos-location;
		string path = paths.substr(location,tokenLen)
			+ directorySeparator + componentsMapFilename;
		populateFromComponentsMapFile(path);
		location = delimiterPos;
	} while(location++ < paths.npos);
}
