/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestWinSupportLib.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Header for test program for WinSupportLib.

Added test of SearchPath()
	2002-11-08, GDLC
Added test of FindResource()
	2002-10-24, GDLC
Added prototypes for factored-out tests of WinAPIs
	2002-06-24, GDLC
Begun.
	2002-05-16, GDLC
----------------------------------------------------------------------------*/

//	GetSystemTimeAsFileTime()
void TestGetSystemTimeAsFileTime(void);
void SInt64ToHex_cout(const SInt64 &time);
void HexByteOut(UInt8 b);

//	GetTimeZoneInformation()
void TestGetTimeZoneInformation(void);
void MachineLocationToHex_cout(const MachineLocation &mLoc);
void DWORDToHex_cout(const DWORD &dw);

//	GetTickCount()
void TestGetTickCount(void);

//	GetModuleFileName()
void TestGetModuleFileName(void);

//	GetModuleHandle()
//	This needs to be part of TestGetModuleFileName()
//void TestGetModuleHandle(void);

//	OutputDebugString()
void TestOutputDebugString(void);

//	FindResource()
const ResID	TEXTResourceID			= 128;
const ResID	IDR_TMPL_SQL_CLASSES	= 129;
const ResID	TEXTResourceID_2		= 130;
void TestFindResource(void);

//	FindFirstFile(), FindNextFile()
void TestFindFirstNextFile(void);

//	SearchPath()
void TestSearchPath(void);
