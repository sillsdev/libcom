/*--------------------------------------------------------------------*//*:Ignore this sentence.
Copyright (C) 2009 SIL International. All rights reserved.

Distributable under the terms of either the Common Public License or the
GNU Lesser General Public License, as specified in the LICENSING.txt file.

File: ManagedComBridge.cpp
Responsibility: Tom Hindle
Last reviewed: Not yet.

Description: Implementation of ManagedComBridge

-------------------------------------------------------------------------------*//*:End Ignore*/

//:>********************************************************************************************
//:>	Include files
//:>********************************************************************************************
#if WIN32
#pragma hdrstop
// any other headers (not precompiled)
#endif

#include "ManagedComBridge.h"

#include <ComRegistry.h>
#include <WinError.h>
#include <stdlib.h>
#include <glib.h>
#include <unistd.h>  // for getcwd 
#include <libgen.h> // for dirname + basename

using namespace std;


extern "C"
{
	// NOTE mono-config header doesn't seem to extern c correctly on Linux
	#include <mono/metadata/mono-config.h>		 
}

/**
 * Helper Function to return the Marshal Class
 */
MonoClass* GetMarshalClass()
{	
	MonoImage *coreLib = mono_get_corlib();	
	
	return mono_class_from_name (
	        coreLib, "System.Runtime.InteropServices", "Marshal");	
}

//:>********************************************************************************************
//:>	Forward declarations
//:>********************************************************************************************

//:>********************************************************************************************
//:>	Local Constants and static variables
//:>********************************************************************************************

MonoDomain *ManagedComBridge::m_domain = NULL;

ManagedComBridge::ManagedComBridgeFactoryMap ManagedComBridge::m_managedComBridgeFactoryMap;

//:>********************************************************************************************
//:>	Methods
//:>********************************************************************************************

/**
 * @brief default constructor used for DllGetClassObject to create a new factory instance.
 * 
 * @param assemblyName - Managed assembly that this factory uses to create its COM object
 * @param classFullName - name of COM object that this factory creates.
 */
ManagedComBridge::ManagedComBridge(const std::string& assemblyName, const std::string& classFullName) :
	m_assemblyName(assemblyName), m_classFullName(classFullName)
{
	
}


ManagedComBridge::~ManagedComBridge()
{
}

/**
 * @brief main entry point for Factory
 *
 * Creates a map between clsid and (assemblyName, classFullName). A new factory instance is created
 * per clsid.   
 *
 * @param clsid The Class ID for the factory
 * @param iid not used.
 * @param ppv - returns a ptr to the factory.
 */
// REVIEW: maybe move DLLEXPORT define from Generic/common.h to WIN32 - something.
HRESULT __attribute__ ((visibility("default"))) DllGetClassObject(REFCLSID clsid, REFIID iid, VOID ** ppv)
{
	// Build up a map of clsid - with its implementation factory class.
	
	// does clsid exist in map?
	ManagedComBridge::ManagedComBridgeFactoryMap::const_iterator iter = ManagedComBridge::m_managedComBridgeFactoryMap.find(clsid);
	
	if (iter == ManagedComBridge::m_managedComBridgeFactoryMap.end())
	{
		std::string assemblyName; 
		std::string classFullName;
		
		if (ComRegistry::getInstance()->getAssemblyFilename(clsid, assemblyName) != S_OK)
			return 1; // S_ERROR/FAIL

		if (ComRegistry::getInstance()->getClassName(clsid, classFullName) != S_OK)
			return 1; // S_ERROR/FAIL
		
		ManagedComBridge::m_managedComBridgeFactoryMap[clsid] = new ManagedComBridge(assemblyName, classFullName);
	}
	
	*ppv = ManagedComBridge::m_managedComBridgeFactoryMap[clsid];
	return S_OK;	
}

/**
 * @param iid id of the requested interface
 * @param ppv return parameter ptr to the COM interface
 */
STDMETHODIMP ManagedComBridge::QueryInterface(REFIID iid, void ** ppv)
{
	*ppv = NULL;

	// Set to the proper interface
	if (IID_IUnknown == iid)
		*ppv = static_cast<IUnknown *>(this);
	else if (IID_IClassFactory == iid)
		*ppv = static_cast<IClassFactory *>(this);
	else
		return E_NOINTERFACE;

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();
	return S_OK;
}

/**
 * Reference counting not implemented for this factory class. 
 */
UINT32 ManagedComBridge::AddRef(void)
{
	return 0;	
}

/**
 * Reference counting not implemented for this factory class. 
 */
UINT32 ManagedComBridge::Release(void)
{
	return 0;	
}

/**
 * Not used.
 */
HRESULT ManagedComBridge::LockServer(BOOL fLock)
{
	return S_OK;	
}

/**
 * @brief Uses this factory class to create an instance of a COM object that 
 * implements the specfied interface. 
 *
 * @param pUnkOuter - Not used
 * @param riid - id of the Interface that the COM object implements. 
 * @param ppvObject - return parameter - returns ptr instance to request interface implementation  
 */
HRESULT ManagedComBridge::CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppvObject)
{	
	std::string classNamespace; 
	std::string className;
	MonoAssembly *assembly; // ptr to the assembly that conatins the COM object.
	MonoImage *image;
	MonoClass *comClass; // ptr to the COM object class type.
	MonoObject *comClassInstance; // ptr to the COM object instance.
	
	const char* const defaultMonoRuntimeVersion = "v2.0.50727";
	const char* const environVarMonoRuntime = "MONO_RUNTIME";
	const char*  monoRuntimeVersion = getenv(environVarMonoRuntime);
	if (monoRuntimeVersion == NULL)
		monoRuntimeVersion = defaultMonoRuntimeVersion;
	
	////
	// Parse Managed COM information from COMRegistry
	////
		
	// className should be fully qualified name and namespace, split namespace and class name;
	string::size_type sep = m_classFullName.rfind(".", m_classFullName.length());
	if (sep == string::npos)
	{
		// class doesn't have namespace
		fprintf(stderr, "Warning: class doesn't have namespace: %s\n", m_classFullName.c_str());
		className = m_classFullName;
	}
	else
	{
		className = m_classFullName.substr(sep +1, m_classFullName.length() - sep);
		classNamespace = m_classFullName.substr(0, sep);
	}
	
	////
	// Start mono runtime
	////
		
	if (m_domain == NULL)
	{
		m_domain = mono_get_root_domain();
		if (m_domain == NULL)
			m_domain = mono_jit_init_version("", monoRuntimeVersion); // Initialize the selected mono runtime.
		if (m_domain == NULL)
		{
			fprintf(stderr, "Error could not init mono\n");
			return 1;			
		}
	
		// Need to ensure mono config is read other wise certian managed library calls will fail.
		mono_config_parse(NULL);		
	}
				
	////
	// Open managed Assembly and create the COM object from C++
	////
		
	MonoClass* marshalClass = GetMarshalClass();	
	
	assembly = mono_domain_assembly_open(m_domain, m_assemblyName.c_str());
	if (assembly == NULL)
	{
		char oldcwd[2048];
		getcwd(oldcwd, sizeof(oldcwd));

		// TODO-Linux: CompRegistry defines this string as componentsMapPathEnvironmentKey
		const char * componentsMapPath = getenv("COMPONENTS_MAP_PATH");

		std::string absAssemblyPath(componentsMapPath);
		absAssemblyPath.append("/");
		absAssemblyPath.append(m_assemblyName);

		// try as a abs path from COMPONENTS_MAP_PATH
		assembly = mono_domain_assembly_open(m_domain, absAssemblyPath.c_str());

		if (assembly == NULL)
		{	
			// try releative path after changing to componentsMapPath
			chdir(componentsMapPath);
			assembly = mono_domain_assembly_open(m_domain, m_assemblyName.c_str());
		}

		if (assembly == NULL)
		{
			// try abs path after changing to componentsMapPath
			assembly = mono_domain_assembly_open(m_domain, absAssemblyPath.c_str());
		}

		if (assembly == NULL)
		{
			// try changing to the location of the assembly
			char * absPath = strdup(absAssemblyPath.c_str());
			chdir(dirname(absPath));
			free(absPath);
			absPath = strdup(absAssemblyPath.c_str());
			assembly = mono_domain_assembly_open(m_domain, basename(absPath));
			free(absPath);
		}

		if (assembly == NULL)
		{
			fprintf(stderr, "Could not open assembly %s : cwd = %s looking at %s\n", 
				m_assemblyName.c_str(), oldcwd, absAssemblyPath.c_str());

			return 1;
		}
	}
	
	image = mono_assembly_get_image(assembly);
	if (image == NULL)
	{
		fprintf(stderr, "Failed to get MonoImage from assembly %s\n", m_assemblyName.c_str());
		return 1;	
	}
		
	comClass = mono_class_from_name(image, classNamespace.c_str(), className.c_str());
	
	if (comClass == NULL)
	{
		fprintf(stderr, "Failed to extract class %s.%s from assembly %s\n", classNamespace.c_str(), className.c_str(), m_assemblyName.c_str());
		return 1;	
	}
	
	comClassInstance = mono_object_new (m_domain, comClass);	
	
	if (comClassInstance == NULL)
	{
		fprintf(stderr, "Failed to create instance of %s.%s from assembly %s\n", classNamespace.c_str(), className.c_str(), m_assemblyName.c_str());
		return 1;
	}
	
	////
	// Get object callable from C++
	////

	// Find Marshal.GetIUnknownForObjectInternal static method.
	MonoMethod* method = mono_class_get_method_from_name (marshalClass, "GetIUnknownForObjectInternal", 1);
	typedef void* (GetIUnknownForObjectInternal)(MonoObject*);
	GetIUnknownForObjectInternal* getIUnknownForObjectInternal  = (GetIUnknownForObjectInternal*)mono_lookup_internal_call(method);
	
	// Execute Marshal.GetIUnknownForObjectInternal
	void *pCCW  = getIUnknownForObjectInternal(comClassInstance);
	
	// Find Marshal.QueryInterfaceInternal
	MonoMethod* method2 = mono_class_get_method_from_name (marshalClass, "QueryInterfaceInternal", 3);
	typedef int (QueryInterfaceInternal)(gpointer pUnk, gpointer riid, gpointer* ppv);
	QueryInterfaceInternal* queryInterfaceInternal  = (QueryInterfaceInternal*)mono_lookup_internal_call(method2);
	
	// Execute Marshal.QueryInterfaceInternal - return its result
	return queryInterfaceInternal(pCCW ,(guint8*)&riid, (void**)ppvObject);
}

template<> const GUID __uuidof(ManagedComBridge)("0490E147-F2D2-4909-A4B8-3533D2F264D0");
