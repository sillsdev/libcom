/*
 *	$Id$
 *
 *	Additional type names used in Win32 code
 *
 *	Neil Mayhew - 2005-04-22
 *
 * Win32 Compatibility Library
 * Copyright (C) 2008 SIL International
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

#ifndef ExtendedTypes_h
#define ExtendedTypes_h

union LARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	};
	struct
	{
		DWORD LowPart;
		LONG HighPart;
	} u;
	LONGLONG QuadPart;
};

union ULARGE_INTEGER
{
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	};
	struct
	{
		DWORD LowPart;
		DWORD HighPart;
	} u;
	ULONGLONG QuadPart;
};

__DECLARE_POINTER_TYPES(LARGE_INTEGER)
__DECLARE_POINTER_TYPES(ULARGE_INTEGER)

union CY
{
    struct
	{
        unsigned long   Lo;
        long			Hi;
    };
    LONGLONG int64;
};

__DECLARE_POINTER_TYPES(CY)

struct FIXED
{
	short          value;
	unsigned short fract;
};

__DECLARE_POINTER_TYPES(FIXED)

struct POINT
{
	long x, y;
};

__DECLARE_POINTER_TYPES(POINT)

struct RECT
{
	long left, right, top, bottom;
};

__DECLARE_POINTER_TYPES(RECT)


typedef long SCODE;
//typedef long HRESULT;
//#define _HRESULT_DEFINED // To avoid conflict with our WinError.h

typedef void*		HANDLE;
typedef HANDLE		HGLOBAL;

#define DECLARE_HANDLE(NAME) typedef struct NAME##__ *NAME;
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HFILE);
DECLARE_HANDLE(HMETAFILEPICT);
DECLARE_HANDLE(HENHMETAFILE);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HKEY);
DECLARE_HANDLE(HDESK);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HRSRC);
DECLARE_HANDLE(HSTR);
DECLARE_HANDLE(HWINSTA);
DECLARE_HANDLE(HKL);
DECLARE_HANDLE(HGDIOBJ);
DECLARE_HANDLE(HDWP);
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HPALETTE);
DECLARE_HANDLE(HIMC);
DECLARE_HANDLE(HGDIOBJ);
DECLARE_HANDLE(HIMAGELIST);
DECLARE_HANDLE(HTREEITEM);
DECLARE_HANDLE(HMONITOR);
DECLARE_HANDLE(HIMAGELIST);

#undef DECLARE_HANDLE

typedef HGDIOBJ			HFONT;
typedef HGDIOBJ			HBITMAP;
typedef HGDIOBJ			HBRUSH;
typedef HINSTANCE		HMODULE;
typedef HMETAFILEPICT 	HMF;
typedef HENHMETAFILE 	HEMF;

typedef WORD	CLIPFORMAT;
typedef DWORD	COLORREF;

typedef char	CCHAR;          
typedef DWORD	LCID;         
typedef WORD	LANGID;      

// Begin UNIXODBC block
// This could probably be simpler.
#define ALLREADY_HAVE_WINDOWS_TYPE
typedef long int SDWORD;
typedef signed short RETCODE;
typedef void* SQLHWND;
#define SQL_API
typedef signed char SCHAR;
typedef SCHAR SQLSCHAR;
#include <sqltypes.h>
// End UNIXODBC block

#endif // ExtendedTypes_h
