/*--------------------------------------------------------------------*//*:Ignore this sentence.
Copyright (C) 2009 SIL International. All rights reserved.

Distributable under the terms of either the Common Public License or the
GNU Lesser General Public License, as specified in the LICENSING.txt file.

File: ManagedComBridge.h
Responsibility:
Last reviewed: Not yet.

-------------------------------------------------------------------------------*//*:End Ignore*/
#pragma once
#ifndef MANAGED_COM_BRIDGE
#define MANAGED_COM_BRIDGE

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <dlfcn.h>

#include <COM.h> // For GUID class
#include <COMInterfaces.h> // For IClassFactory
#include <map>

/*----------------------------------------------------------------------------------------------
Class: ManagedComBridge (Generic Factory class)
Description: This allows creating Managed COM objects
Multiple instances of this Factory class is created per unique COM clsid.

This class allows creating of managed COM objects (using libcom) from native code.
For each unique clsid that DllGetClassObject (defined in ManagedComBridge invoked by libcom)
is given it creates a new instance of this factory class.
When createinstance is called on the factory instance (invoked by
libcom possibly from a native CoCreateInstance call) it creates the requested
managed COM object by first starting mono, if needed, then using mono's internal calls to
find and create the COM object and return a native ptr to the COM instance back to libcom.
----------------------------------------------------------------------------------------------*/
class ManagedComBridge : public IClassFactory
{
public:

	// Constructors/destructors/etc.
	ManagedComBridge(const std::string& assemblyName, const std::string& classFullName);
	virtual ~ManagedComBridge();

	// Creates Instances of class provided by Factory.
	HRESULT CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppvObject);

	// Factory's QueryInterface method.
	HRESULT QueryInterface(REFIID iid, void ** ppv);

	// These methods are not used.
	UINT32 AddRef(void);
	UINT32 Release(void);
	HRESULT LockServer(BOOL fLock);

	// Stores the mapping between a creatable COM object classID and their associated ManagedComBridge instance.
	typedef std::map<GUID,ManagedComBridge*> ManagedComBridgeFactoryMap;
	static ManagedComBridgeFactoryMap m_managedComBridgeFactoryMap;

protected:
	static MonoDomain *m_domain;

	const std::string m_assemblyName;
	const std::string m_classFullName;
};
//DEFINE_COM_PTR(ManagedComBridge);


#endif  //MANAGED_COM_BRIDGE
