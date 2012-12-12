/*
 *	DateTime.h
 *
 *	Date and Time emulation for Win32
 *
 *	Neil Mayhew - Apr 22, 2005
 *
 * Win32 compatibility Library
 * Copyright (C) 2005 SIL International
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
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
