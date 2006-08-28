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

typedef struct tagSYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}SYSTEMTIME, *PSYSTEMTIME;

int GetTimeFormat(LCID, DWORD, PSYSTEMTIME, LPCTSTR, LPTSTR, int);
void GetLocalTime(PSYSTEMTIME);
void GetSystemTime(PSYSTEMTIME);
void GetSystemTimeAsFileTime(FILETIME*);
BOOL SystemTimeToFileTime(PSYSTEMTIME, FILETIME*);
BOOL FileTimeToSystemTime(FILETIME*, PSYSTEMTIME);
DWORD GetTickCount();

enum
{
	LOCALE_SYSTEM_DEFAULT,
	LOCALE_USER_DEFAULT,
	LOCALE_INVARIANT,
};
enum
{
	DATE_SHORTDATE,
};
int GetDateFormat(LCID, DWORD, PSYSTEMTIME, LPCTSTR, LPTSTR, int);

#endif //_DATETIME_H_
