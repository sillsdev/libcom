/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestWinSupportLib.cp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Test program for WinSupportLib.

Added FindNextFile() and FindClose()
	2002-11-01, GDLC
Deleted struct AppOrLibDetails (no longer needed because of using CFBundle APIs)
	2002-08-09, GDLC
Added testing of bundleInfo structure creator
	2002-07-15, GDLC
Factored out tests of WinAPIs
	2002-06-24, GDLC
Successful test of GetSystemTimeAsFileTime(). Binary output was WinFILETIME
(now preserved as WinFILETIME2002-06-14); console output was WinFILETIME.out
(now preserved as WinFILETIME2002-06-14.out)
	2002-06--14, GDLC
Begun.
	2002-04-30, GDLC
----------------------------------------------------------------------------*/

#include "WinSupport.h"
#include "WinSupportInternals.h"

#include "TestWinSupportLib.h"

#include "ostype.h"

#include <iostream>
#include <fstream>

#include <UTCUtils.h>
#include <DateTimeUtils.h>

using namespace std;

//	Functions exported from WinSupportLib.shlb
extern void GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime);
extern DWORD GetTimeZoneInformation(
    LPTIME_ZONE_INFORMATION lpTimeZoneInformation
    );
extern DWORD GetTickCount(void);
extern DWORD GetModuleFileName(
    /*IN*/ HMODULE hModule,
    /*OUT*/ LPSTR lpFilename,
    /*IN*/ DWORD nSize
    );
extern HMODULE GetModuleHandle(/*IN*/ LPCSTR lpModuleName);
extern VOID OutputDebugStringMac(/*IN*/ LPCSTR lpOutputString);
extern HRSRC FindResource(void*, int rid, const char* type);
extern HANDLE FindFirstFile(
	LPCSTR				lpFileName,
	LPWIN32_FIND_DATA	lpFindFileData
	);
extern BOOL FindNextFile(
    HANDLE hFindFile,
    LPWIN32_FIND_DATA lpFindFileData
    );
extern BOOL FindClose(
    HANDLE hFindFile
    );
extern DWORD SearchPath(
    LPCSTR	lpPath,
    LPCSTR	lpFileName,
    LPCSTR	lpExtension,
    DWORD	nBufferLength,
    LPSTR	lpBuffer,
    LPSTR	*lpFilePart
    );
extern int _strnicmp(const char *p, const char *q, std::size_t n);
extern int _memicmp(const void *, const void *, std::size_t);

//	Some of the structures obtained from MacOS by WinSupport.CarbonTest.Lib are
//	needed by this test program for comparison purposes. They are transmitted from
//	WinSupport.CarbonTest.Lib by means of global static identifiers.
extern	UTCDateTime			timeUTC;
extern	MachineLocation		machLoc;
extern	EventTime			secsFP;

int main()
{
	cout << "TestWinSupportLib" << endl << endl;

	TestGetSystemTimeAsFileTime();
	cout << endl;
	TestGetTimeZoneInformation();
	cout << endl;
	TestGetTickCount();
	cout << endl;
	TestGetModuleFileName();
	cout << endl;
//	TestOutputDebugString();
//	cout << endl;
	TestFindResource();
	cout << endl;
	TestFindFirstNextFile();
	cout << endl;
	TestSearchPath();
	cout << endl;
	return 0;
}

void TestGetSystemTimeAsFileTime()
{
	union {
		FILETIME	tWin;
		SInt64		time64;
		char		t64[8];
	};
	union {
		UTCDateTime	tUTC;
		SInt64		tUTC64;
	};

	// Console output
	cout << "GetSystemTimeAsFileTime()" << endl;
	GetSystemTimeAsFileTime(&tWin);
	cout << "MacOS UTCDateTime:" << endl;
	tUTC = timeUTC;
	SInt64ToHex_cout(tUTC64);
	cout << " - ";
	SInt32		dateTimeSecs = static_cast <SInt32> ((tUTC64 >> 16) & 0xFFFFFFFF);
	Str255		dateOut, timeOut;
	DateString(dateTimeSecs, longDate, &dateOut[0], NULL);
	TimeString(dateTimeSecs, true, &timeOut[0], NULL);
	for (int j=1; j<=dateOut[0]; j++) cout << dateOut[j];
	cout << ' ';
	for (int j=1; j<=timeOut[0]; j++) cout << timeOut[j];
	cout << "  UTC Greenwich Mean Time" << endl;
	cout << "WinFILETIME:" << endl;
	SInt64ToHex_cout(time64);
	cout << endl;

	// Test file output (for WinTestWinSupport.exe)
	ofstream		test("WinFILETIME");
	//	Write this in little-endian form for Windows test
	for (int i=7; i>=0; i--) test.put(t64[i]);
	test.close();
}

void TestGetTimeZoneInformation()
{
	TIME_ZONE_INFORMATION	tZ;

	// Console output
	cout << "GetTimeZoneInformation()" << endl;
	DWORD tZRet = GetTimeZoneInformation(&tZ);
	cout << "MacOS MachineLocation:" << endl;
	MachineLocationToHex_cout(machLoc);
	printf(" gmtDelta = %i seconds\n", ((machLoc.u.gmtDelta & 0XFFFFFF) << 8) >>8);
	cout << "return value from GetTimeZoneInformation: ";
	DWORDToHex_cout(tZRet); cout << endl;
	cout << "WinTIME_ZONE_INFORMATION:" << endl;
	printf("%lx + 168 zeroes\n", tZ.Bias);

	// Test file output (for WinTestWinSupport.exe)
	ofstream		test("WinTIME_ZONE_INFORMATION");
	//	Write Bias in little-endian form for Windows test
	//	The rest of the Windows structure is all zeros so it is ignored.
	//TODO: If we ever do something realistic with the daylight savings fields
	//	this test output will need to output the extra fields and
	//	WinTestWinSupport.exe must be modified to read them.
	union {
		LONG	Bias;
		SInt8	Bch[4];
	};
	Bias = tZ.Bias;
	for (int i=3; i>=0; i--) test.put(Bch[i]);
	test.close();
}

void TestGetTickCount()
{
	DWORD		tickCount;

	// Console output
	cout << "GetTickCount()" << endl;
	tickCount = GetTickCount();
	cout << "MacOS CurrentEventTime:" << endl;
	printf("%f secs\n", secsFP);
	cout << "WinDWORD:" << endl;
	DWORDToHex_cout(tickCount);
	printf(" (%d millisecs)\n", tickCount);
}

void TestGetModuleFileName()
{
	CHAR	buf[256];
	DWORD	nCh;

	cout << "GetModuleFileName(NULL)" << endl;
	nCh = GetModuleFileName(NULL, &buf[0], 255);
	printf("%d chars:\n%s\n", nCh, buf);
	cout << "GetModuleHandle(NULL)" << endl;
	HMODULE hMod = GetModuleHandle(NULL);
	printf("hMod = %x\n", hMod);
//	TODO: Add this test after the non-NULL HMODULE has been implemented
//	cout << "GetModuleFileName(hMod)" << endl;
//	nCh = GetModuleFileName(hMod, &buf[0], 255);
//	printf("%d chars:\n%s\n", nCh, buf);
}

void TestOutputDebugString()
{
	cout << "OutputDebugString()" << endl;
	OutputDebugString("The test of OutputDebugString() is happening now");
#ifdef	_DEBUG
	printf("The test of OutputDebugString() has now happened.");
#else
	printf("The test of OutputDebugString() should have produced nothing!");
#endif
}

void TestFindResource()
{
	HRSRC	textRes;	// Handle to resource
	long	cb_1;		// Size of resource
	void	* pText;	// Pointer to TEXT
	HRSRC	templRes;	// Handle to resource
	long	cb_2;		// Size of resource
	void	* pTemp;	// Pointer to TEXT
	long	i;
	char	c;
	char	*p;
	cout << "FindResource()" << endl;
	if ((textRes = FindResource(NULL, TEXTResourceID, "TEXT")) == NULL)
	{
		printf("TEXT resource not obtainable\n");
		return;
	}
	printf("TEXT ResID=%i ", TEXTResourceID);
	if ((cb_1 = SizeofResource(NULL, textRes)) == 0)
	{
		printf("Resource is empty!\n");
		return;
	}
	printf(" Size=%i ", cb_1);
	if ((pText = LockResource(textRes)) == NULL)
	{
		printf("Resource cannot be locked!\n");
		return;
	}
	cout << endl;
	for (i=0, p=static_cast <char *> (pText); i<cb_1; i++)
	{
		c = *p++;
		cout << c;
	}
	cout << endl;

	if ((templRes = FindResource(NULL, IDR_TMPL_SQL_CLASSES, "TEMPLATE")) == NULL)
	{
		printf("TEMPLATE resource not obtainable\n");
		return;
	}
	printf("TEMPLATE ResID=%i ", IDR_TMPL_SQL_CLASSES);
	if ((cb_2 = SizeofResource(NULL, templRes)) == 0)
	{
		printf("Resource is empty!\n");
		return;
	}
	printf(" Size=%i ", cb_2);
	if ((pTemp = LockResource(templRes)) == NULL)
	{
		printf("Resource cannot be locked!\n");
		return;
	}
	cout << endl;
	for (i=0, p=static_cast <char *> (pTemp); i<cb_2; i++)
	{
		c = *p++;
		if (c != '\r') cout << c;
	}
	cout << endl;
}

void TestFindFirstNextFile()
{
	WIN32_FIND_DATA	fileData;
	HANDLE			progress;
	char			path[] = "CCS2 HD:UnfragTestingBACKUP: TEST_?.DOC";

	// Console output
	cout << "FindFirstFile(" << path << ")" << endl;
	if ((progress = FindFirstFile(path, &fileData)) == INVALID_HANDLE_VALUE)
	{
		cout << "FindFirstFile() returned invalid handle";
		printf(" Error Code: 0x%08X\n", GetLastError());
		return;
	}
	printf("%-33s%5X  %4s  %4s\n", fileData.cFileName, fileData.dwFileAttributes, &ostype(fileData.dwCreatorType), &ostype(fileData.dwFileType));
	while (FindNextFile(progress, &fileData))
	{
		printf("%-33s%5X  %4s  %4s\n", fileData.cFileName, fileData.dwFileAttributes, &ostype(fileData.dwCreatorType), &ostype(fileData.dwFileType));
	}

	cout << "FindClose()" << endl;
	FindClose(progress);
}

void TestSearchPath()
{
	char			path[] = "CCS2 HD:UnfragTestingBACKUP";
	char			file[] = " test_2";
	char			ext[] = ".DOC";
	char			pathName[MAX_PATH];
	char			*filePart;
	DWORD			nChars;

	// Console output
	cout << "SearchPath(" << path << ", " << file << ", " << ext << ")" << endl;
	nChars = SearchPath(path, file, ext, MAX_PATH, pathName, &filePart);
	if (nChars > 0) printf("%s, %s\n", pathName, filePart);
	else cout << "File not found on search path\n";
}

void SInt64ToHex_cout(const SInt64 &time)
{
	SInt64			tmp = time;
	for (int i=0; i<8; i++)
	{
		HexByteOut(static_cast <UInt8> ((tmp & 0xFF00000000000000) >> 56));
		tmp <<= 8;
		if (i == 1 || i == 3 || i == 5) {
			cout << " ";
		}
	}
}

void MachineLocationToHex_cout(const MachineLocation &mLoc)
{
	union {
		MachineLocation		mL;
		UInt8				mLocB[12];
	};
	mL = mLoc;
	for (int i=0; i<12; i++)
	{
		HexByteOut(mLocB[i]);
		if (i == 1 || i == 3 || i == 5 || i == 7 || i == 9) {
			cout << " ";
		}
	}
}

void DWORDToHex_cout(const DWORD &dw)
{
	DWORD			tmp = dw;
	for (int i=0; i<4; i++)
	{
		HexByteOut(static_cast <UInt8> ((tmp & 0xFF000000) >> 24));
		tmp <<= 8;
		if (i == 1 ) {
			cout << " ";
		}
	}
}

void HexByteOut(UInt8 b)
{
	UInt8 hc[16] =
	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	cout << hc[b >> 4];
	cout << hc[b & 0xF];
}
