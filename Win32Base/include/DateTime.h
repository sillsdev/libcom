/*
 *	$Id$
 *
 *	Date and Time emulation for Win32
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#ifndef _DATETIME_H_
#define _DATETIME_H_

#include <BasicTypes.h>
#include <ExtendedTypes.h>

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

enum
{
	DATE_SHORTDATE        = 1 << 0,
	DATE_LONGDATE         = 1 << 1,
	DATE_USE_ALT_CALENDAR = 1 << 2,
	DATE_YEARMONTH        = 1 << 3,
	DATE_LTRREADING       = 1 << 4,
	DATE_RTLREADING       = 1 << 5,
};

DWORD GetTickCount();

#endif //_DATETIME_H_
