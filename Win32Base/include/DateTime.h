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

typedef struct _TIME_ZONE_INFORMATION {
  LONG Bias;
  WCHAR StandardName[32];
  SYSTEMTIME StandardDate;
  LONG StandardBias;
  WCHAR DaylightName[32];
  SYSTEMTIME DaylightDate;
  LONG DaylightBias;
} TIME_ZONE_INFORMATION, *PTIME_ZONE_INFORMATION;

int GetTimeFormat(LCID, DWORD, PSYSTEMTIME, LPCTSTR, LPTSTR, int);
void GetLocalTime(PSYSTEMTIME);
void GetSystemTime(PSYSTEMTIME);
void GetSystemTimeAsFileTime(FILETIME*);
BOOL SystemTimeToFileTime(PSYSTEMTIME, FILETIME*);
BOOL FileTimeToSystemTime(FILETIME*, PSYSTEMTIME);
DWORD GetTickCount();
DWORD GetTimeZoneInformation(PTIME_ZONE_INFORMATION);

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
