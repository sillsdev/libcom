/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestCOMSupportLib.cp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Test program for COMSupportLib.

Removed inclusion of Carbon.h. Minor improvements to commentary. Removed use of Carbon UInt8
	2003-06-23, NJHM, GDLC
Modified to load all dynamic libraries in the subdirectory DynamicLibraries
	2003-06-23, GDLC
Modified for compilation using a makefile under ProjectBuilder. Also modified for correct
loading of the component in a Mach-O dynamic library.
	2003-06-21, GDLC, NJHM
Added use of GetSharedLibrary() instead of ComponentLoaded();
	2003-05-19, GDLC
Added use of ComponentLoaded()
	2003-05-15, GDLC
Modified for sample code from Chapter 7 of Rogerson's book.
	2003-04-30, GDLC
Modified interface definitions for component CA with interfaces IX, IY, IZ (Rogerson Chap 4).
	2003-04-17, GDLC
Added interface definitions for component CA with interfaces IX, IY, IZ (Rogerson Chap 3).
	2003-04-16, GDLC
Added default cout stream output parameters.
	2003-04-14, GDLC
Begun.
	2003-03-31, GDLC
----------------------------------------------------------------------------*/

#if TARGET_RT_MAC_CFM
#include <CodeFragments.h>
#else
#include <sys/types.h>
#include <dirent.h>		// For opendir etc.
#include <dlfcn.h>		// For dlopen etc.
#endif

#include <iostream>
#include <fstream>
#include <string>

//	Include WinSupport.h and COMSupport.h
#include "windows.h"

#include "TestCOMSupportLib.h"

#include "ComponentInterfaces.h"

using namespace std;

void trace(const char* msg) { cout << "Client: \t\t" << msg << endl ;}

#pragma warn_unusedvar off
int main()
{
	trace("TestCOMSupportLib\n");

//	TestCLSIDFromString();

//	TestStringFromCLSID();

#if TARGET_RT_MAC_CFM
//	Load Component CFM shared library
	CFragConnectionID	componentConnID;		// Not used by this test program
	Ptr					componentServerAddr;	// Not used by this test program
	Str255				errorMsg;

	OSErr cfErr = GetSharedLibrary ("\pComponent.dylib",
									kPowerPCCFragArch,
									kReferenceCFrag,
									&componentConnID,
									&componentServerAddr,
									errorMsg
								);

	if (cfErr != noErr) {
		cout << "Client: \t\tError loading Component server (";
		cout << errorMsg << cfErr << ")" << endl;
		return cfErr;
	}

#else
//	Load Component dynamic library

//	Fixed pathname method
//	void* module = dlopen ("Component.dylib", RTLD_NOW);
//
//	cout << "Value returned from dlopen: " << module << endl;
//
//	if (module == 0) {
//		cout << "Client: \t\tError loading Component server: ";
//		cout << dlerror() << endl;
//		return 1;
//	}

//	Scan DynamicLibraries subdirectory method
	std::string	subdir = "Components";
	DIR *	dlddir = opendir(subdir.c_str());
	if (dlddir == 0)
	{
		cout << subdir << " could not be opened" << endl;
		return 1;
	}
	cout << subdir << " subdir now open" << endl;

	struct dirent *direntry;
	while ((direntry = readdir(dlddir)) != 0)
	{
		cout << direntry->d_name << endl;
		if (direntry->d_type == DT_REG && direntry->d_name[0] != '.')
		{
			cout << "this is a regular file!" << endl;
			std::string path = subdir + '/' + direntry->d_name;
			void* module = dlopen (path.c_str(), RTLD_NOW);

			cout << "Value returned from dlopen: " << module << endl;

			if (module == 0) {
				cout << "Client: \t\tError loading Component server: ";
				cout << dlerror() << endl;
			}
		}
	}

#endif

	trace("Call CoCreateInstance to create") ;
	trace("  component and get interface IX.") ;
	IX* pIX = NULL ;

	HRESULT hr = CoCreateInstance(CLSID_Component1,
	                                (IUnknown *)NULL, 
	                                (DWORD)CLSCTX_INPROC_SERVER,
	                                IID_IX, 
	                                (void **)&pIX) ;
	if (SUCCEEDED(hr))
	{
		trace("Succeeded getting IX.") ;
		pIX->Fx() ;          // Use interface IX.

		trace("Ask for interface IY.") ;
		IY* pIY = NULL ;
		hr = pIX->QueryInterface(IID_IY, (void**)&pIY) ;
		if (SUCCEEDED(hr))
		{
			trace("Succeeded getting IY.") ;
			pIY->Fy() ;       // Use interface IY.
			pIY->Release() ;
			trace("Release IY interface.") ;
		}
		else
		{
			trace("Could not get interface IY.") ;
		}

		trace("Ask for interface IZ.") ;

		IZ* pIZ = NULL ;
		hr = pIX->QueryInterface(IID_IZ, (void**)&pIZ) ;
		if (SUCCEEDED(hr))
		{
			trace("Succeeded in getting interface IZ.") ;
			pIZ->Fz() ;
			pIZ->Release() ;
			trace("Release IZ interface.") ;
		}
		else
		{
			trace("Could not get interface IZ.") ;
		}

		trace("Release IX interface.") ;
		pIX->Release() ;
	}
	else
	{
		cout << "Client: \t\tCould not create component. hr = "
		     << hex << hr << endl ;    
	}

	return 0;
}
#pragma warn_unusedvar reset

//	TestCLSIDFromString
//
void TestCLSIDFromString()
{
	//	IUnknown
	OLECHAR		IUnksz[] = L"00000000-0000-0000-C000-000000000046";
	CLSID		IUnk;
	HRESULT		r;

	cout << "CLSIDFromString() acting on IUnknown" << endl;
	cout << "IUnknown  {00000000-0000-0000-C000-000000000046}" << endl;
	r = CLSIDFromString(IUnksz, &IUnk);
	cout << "HResult = " << r << endl;
	HexGUIDOut(IUnk);
	cout << endl << endl;
}

//	TestStringFromCLSID
//
void TestStringFromCLSID()
{
	//	IUnknown
	OLECHAR		IUnksz[64] = L"00000000-0000-0000-C000-000000000046";
	CLSID		IUnk;
	HRESULT		r;
	LPOLESTR	s;
	char		s8[128];

	cout << "StringFromCLSID() acting on IUnknown" << endl;
	cout << "IUnknown  {00000000-0000-0000-C000-000000000046}" << endl;
	r = CLSIDFromString(IUnksz, &IUnk);
	r = StringFromCLSID(IUnk, &s);
	cout << "HResult = " << r << endl;
	wcstombs(s8, s, 128);
	cout << s8;
	cout << endl << endl;

	// Delete the string
	CoTaskMemFree(s);
}

void HexGUIDOut(const GUID &gd, std::ostream& stm)
{
	union {
		GUID			tmp;
		unsigned char	b[16];
	};

	tmp = gd;

	for (int i=0; i<16; i++)
	{
		unsigned char	c;

		c = b[i];
		HexByteOut(static_cast <unsigned char> (b[i]));
		if (i == 3 || i == 5 || i == 7 || i == 9) {
			stm << "-";
		}
	}
}

void HexDWORDOut(const DWORD &dw, std::ostream& stm)
{
	DWORD			tmp = dw;
	for (int i=0; i<4; i++)
	{
		HexByteOut(static_cast <unsigned char> ((tmp & 0xFF000000) >> 24));
		tmp <<= 8;
		if (i == 1 ) {
			stm << " ";
		}
	}
}

void HexByteOut(unsigned char b, std::ostream& stm)
{
	unsigned char hc[16] =
	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	stm << hc[b >> 4];
	stm << hc[b & 0xF];
}
