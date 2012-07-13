//
//	WinTestWinSupport.cpp
//
//	Test program to be run in Windows to convert the binary FILETIME written to a
//	disk file by the MacOS TestWinSupport program into a human readable date and
//	time so that the correctness of the function in WinSupportLib can be determined.
//
//	Fixed and improved
//	2002-06-15, Neil Mayhew
//	Written
//	2002-05-31	Graeme Costin

#include <windows.h>

#include <fstream>
#include <iostream>

using namespace std;

int main( void )
{
	cout << "This is a test of the WinSupportLib time function" << endl;
	cout << "GetSystemTimeAsFileTime";

	union {
		FILETIME	tWin;
		char		t64[8];
	};

	ifstream		test("WinFILETIME");
	// Read the 64-bit integer, already low-endian
	for (int i=0; i<8; i++) test.get(t64[i]);
	test.close();

	SYSTEMTIME		tSys;
	BOOL	result = FileTimeToSystemTime(&tWin, &tSys);

	int			nc;
	TCHAR		tBuf[256];
	nc = GetDateFormat(
		NULL,
		LOCALE_NOUSEROVERRIDE + DATE_LONGDATE,
		&tSys,
		NULL,
		tBuf,
		256);
	cout << ' ' << (char*)tBuf;
	nc = GetTimeFormat(
		NULL,
		LOCALE_NOUSEROVERRIDE + TIME_FORCE24HOURFORMAT,
		&tSys,
		NULL,
		tBuf,
		256);
	cout << ' ' << (char*)tBuf;

	cout << endl;

	return 0;
}
