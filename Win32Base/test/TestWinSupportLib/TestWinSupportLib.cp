/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestWinSupportLib.cp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

Test program for WinSupportLib.

Successful test of GetSystemTimeAsFileTime(). Binary output was WinFILETIME
(now preserved as WinFILETIME2002-06-14); console output was WinFILETIME.out
(now preserved as WinFILETIME2002-06-14.out)
	2002-06--14, GDLC
Begun.
	2002-04-30, GDLC
----------------------------------------------------------------------------*/

#include "TestWinSupportLib.h"

#include "WinSupport.h"

#include <iostream>
#include <fstream>

#include <UTCUtils.h>
#include <DateTimeUtils.h>

using namespace std;

extern	UTCDateTime		timeUTC;

int main()
{
	cout << "TestWinSupportLib" << endl;

	union {
		FILETIME	tWin;
		SInt64		time64;
		char		t64[8];
	};
	union {
		UTCDateTime	tUTC;
		SInt64		tUTC64;
	};
	cout << "GetSystemTimeAsFileTime()" << endl;
	GetSystemTimeAsFileTime(&tWin);

	cout << "MacOS UTCDateTime:" << endl;
	tUTC = timeUTC;
	SInt64ToHex_cout(tUTC64);
	cout << " - ";
	SInt32			dateTimeSecs = (tUTC64 >> 16) & 0xFFFFFFFF;
	Str255			dateOut, timeOut;
	DateString(dateTimeSecs, longDate, &dateOut[0], NULL);
	TimeString(dateTimeSecs, true, &timeOut[0], NULL);
	
	for (int j=1; j<=dateOut[0]; j++) cout << dateOut[j];
	cout << ' ';
	for (int j=1; j<=timeOut[0]; j++) cout << timeOut[j];
	cout << "  UTC Greenwich Mean Time";
	
	cout << endl << "WinFILETIME:" << endl;
	ofstream		test("WinFILETIME");
	//	Write this in little-endian form for Windows test
	for (int i=7; i>=0; i--) test.put(t64[i]);
	test.close();
	SInt64ToHex_cout(time64);
	
	cout << endl;
	
	return 0;
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

void HexByteOut(UInt8 b)
{
	UInt8 hc[16] =
	{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	cout << hc[b >> 4];
	cout << hc[b & 0xF];
}
