/*
 *	$Id$
 *
 *	Main type names used in Win32 code
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

#ifndef _BASICTYPES_H_
#define _BASICTYPES_H_

#include <cstddef>
#include <unicode/utypes.h>

// Compiler features

#define __cdecl //__attribute__((cdecl))
#define __stdcall //__attribute__((stdcall))
#define __declspec(X)		// No easy way to emulate this

// Has to be #define, not typedef, because of "unsigned __int64"
#define __int64 long long

// Win32SDK types and defines

#define FAR
#define NEAR
#define PASCAL
#define CONST const

#define CDECL __cdecl

typedef          void      VOID;

typedef          char      CHAR;
typedef unsigned char      UCHAR;
typedef          short     SHORT;
typedef unsigned short     USHORT;
typedef          int       INT;
typedef unsigned int       UINT;
typedef          long      LONG;
typedef unsigned long      ULONG;
typedef          long long LONGLONG;
typedef unsigned long long ULONGLONG;

typedef          float     FLOAT;
typedef          double    DOUBLE;

typedef USHORT	WORD;
typedef ULONG	DWORD;
typedef UCHAR	BYTE;
typedef UCHAR	BCHAR;

typedef LONG        BOOL;
typedef std::size_t SIZE_T;

#define __DECLARE_POINTER_TYPES(TYPE) \
	typedef       TYPE NEAR *P##TYPE; \
	typedef       TYPE FAR  *LP##TYPE; \
	typedef CONST TYPE NEAR *PC##TYPE; \
	typedef CONST TYPE FAR	*LPC##TYPE;

__DECLARE_POINTER_TYPES(VOID)
__DECLARE_POINTER_TYPES(CHAR)
__DECLARE_POINTER_TYPES(UCHAR)
__DECLARE_POINTER_TYPES(SHORT)
__DECLARE_POINTER_TYPES(USHORT)
__DECLARE_POINTER_TYPES(INT)
__DECLARE_POINTER_TYPES(UINT)
__DECLARE_POINTER_TYPES(LONG)
__DECLARE_POINTER_TYPES(ULONG)
__DECLARE_POINTER_TYPES(LONGLONG)
__DECLARE_POINTER_TYPES(ULONGLONG)

__DECLARE_POINTER_TYPES(FLOAT)
__DECLARE_POINTER_TYPES(DOUBLE)

__DECLARE_POINTER_TYPES(WORD)
__DECLARE_POINTER_TYPES(DWORD)
__DECLARE_POINTER_TYPES(BYTE)

__DECLARE_POINTER_TYPES(BOOL)

// Wide characters - 16-bit

typedef UChar				OLECHAR;
typedef UChar				WCHAR;
typedef wchar_t				TCHAR; // Assume we always want Unicode

typedef OLECHAR*			BSTR;

#include <cctype>	// So _T won't get defined again if ctype is included later
#undef  _T			// In <ctype.h>
#define _T(X) L##X

#define __DECLARE_STRING_TYPES(TYPE, NAME) \
	typedef       TYPE NEAR *P##NAME; \
	typedef       TYPE FAR  *LP##NAME; \
	typedef CONST TYPE NEAR *PC##NAME; \
	typedef CONST TYPE FAR	*LPC##NAME;

__DECLARE_POINTER_TYPES(OLECHAR)
__DECLARE_POINTER_TYPES(CHAR)
__DECLARE_POINTER_TYPES(WCHAR)
__DECLARE_POINTER_TYPES(TCHAR)
__DECLARE_STRING_TYPES(OLECHAR, OLESTR)
__DECLARE_STRING_TYPES(CHAR, STR)
__DECLARE_STRING_TYPES(WCHAR, WSTR)
__DECLARE_STRING_TYPES(TCHAR, TSTR)

typedef DWORD				WPARAM;
typedef LONG*				LPARAM;
typedef LONG*				LRESULT;

typedef unsigned char byte;

// API declaration macros

#define WINAPI  	FAR PASCAL
#define CALLBACK	FAR PASCAL

#define STDMETHODCALLTYPE __stdcall
#define STDMETHODVCALLTYPE __cdecl
#define STDMETHODIMP_(TYPE) TYPE STDMETHODCALLTYPE
#define STDMETHODIMP STDMETHODIMP_(HRESULT)
#define STDMETHOD_(RET, NAME) virtual RET STDMETHODCALLTYPE NAME
#define STDMETHOD(NAME) STDMETHOD_(HRESULT, NAME)
#define STDMETHODV_(RET, NAME) virtual RET STDMETHODVCALLTYPE NAME
#define STDMETHODV(NAME) STDMETHOD_(HRESULT, NAME)

#define STDAPICALLTYPE  __stdcall
#define STDAPIVCALLTYPE __cdecl
#define EXTERN_C extern "C"

#define STDAPI EXTERN_C HRESULT STDAPICALLTYPE

#endif //_BASICTYPES_H_
