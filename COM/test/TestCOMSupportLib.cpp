/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestCOMSupportLib.cp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Test program for COMSupportLib.

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

//	Include WinSupport.h and COMSupport.h
#include "windows.h"

#include "TestCOMSupportLib.h"

#include "ComponentInterfaces.h"

#include <CodeFragments.h>

//#include "ostype.h"

//#include <iostream>
//#include <fstream>

using namespace std;

void trace(const char* msg) { cout << "Client: \t\t" << msg << endl ;}

#pragma warn_unusedvar off
int main()
{
	trace("TestCOMSupportLib\n");

	TestCLSIDFromString();

	TestStringFromCLSID();

//	Load Component shared library

CFragConnectionID	componentConnID;
Ptr					componentServerAddr;	// Not used by this test program
Str255				errorMsg;


OSErr cfErr = GetSharedLibrary ("\pComponent.shlb",
								kPowerPCCFragArch,
								kReferenceCFrag,
								&componentConnID,
                  				&componentServerAddr,
                  				errorMsg
							);

	if (cfErr != noErr) {
		cout << "Client: \t\tError loading Component server (";
		cout << cfErr << ")" << endl;
		return cfErr;
	}

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
	cout << endl;
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
	cout << endl;

	// Delete the string
	CoTaskMemFree(s);
}

void HexGUIDOut(const GUID &gd, std::ostream& stm)
{
	union {
		GUID		tmp;
		UInt8		b[16];
	};

	tmp = gd;

	for (int i=0; i<16; i++)
	{
		UInt8	c;

		c = b[i];
		HexByteOut(static_cast <UInt8> (b[i]));
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
		HexByteOut(static_cast <UInt8> ((tmp & 0xFF000000) >> 24));
		tmp <<= 8;
		if (i == 1 ) {
			stm << " ";
		}
	}
}

void HexByteOut(UInt8 b, std::ostream& stm)
//void HexByteOut(UInt8 b)
{
	UInt8 hc[16] =
	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	stm << hc[b >> 4];
	stm << hc[b & 0xF];
}
