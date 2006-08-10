/*
 *	$Id$
 *
 *	Date and Time emulation for Win32
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#ifndef _DATETIME_H_
#define _DATETIME_H_

typedef double DATE;

struct FILETIME
{
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
};

struct SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

void GetSystemTimeAsFileTime(FILETIME*);
BOOL SystemTimeToFileTime(SYSTEMTIME*, FILETIME*);
BOOL FileTimeToSystemTime(FILETIME*, SYSTEMTIME*);
DWORD GetTickCount();

#endif //_DATETIME_H_
