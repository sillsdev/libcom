/*
 *	WinError.h
 *
 *	Error codes for COM compatibility library
 *
 *	Neil Mayhew - 2012-11-28
 *
 * Win32 Compatibility Library
 * Copyright (C) 2007 SIL International
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

#ifndef _WinError_h_
#define _WinError_h_

#include <BasicTypes.h>

#include <errno.h> // For POSIX errors that are also used on Windows

const HRESULT _ERRORBASE = 0x80000000;

inline bool FAILED(HRESULT hr)    { return hr & _ERRORBASE; }
inline bool SUCCEEDED(HRESULT hr) { return !FAILED(hr); }

const HRESULT S_OK                      = 0;
const HRESULT S_FALSE                   = 1;

const HRESULT ERROR_SUCCESS             = 0;
const HRESULT NOERROR                   = 0;

const HRESULT E_ABORT                   = _ERRORBASE + 0x04004;
const HRESULT E_ACCESSDENIED            = _ERRORBASE + 0x70005;
const HRESULT E_FAIL                    = _ERRORBASE + 0x04005;
const HRESULT E_HANDLE                  = _ERRORBASE + 0x70006;
const HRESULT E_INVALIDARG              = _ERRORBASE + 0x70057;
const HRESULT E_NOINTERFACE             = _ERRORBASE + 0x04002;
const HRESULT E_NOTIMPL                 = _ERRORBASE + 0x04001;
const HRESULT E_OUTOFMEMORY             = _ERRORBASE + 0x7000E;
const HRESULT E_POINTER                 = _ERRORBASE + 0x04003;
const HRESULT E_PENDING                 = _ERRORBASE + 0x0000A;
const HRESULT E_UNEXPECTED              = _ERRORBASE + 0x0FFFF;

const HRESULT CLASS_E_CLASSNOTAVAILABLE = _ERRORBASE + 0x40111;
const HRESULT CLASS_E_NOAGGREGATION     = _ERRORBASE + 0x40110;
const HRESULT CO_E_CLASSSTRING          = _ERRORBASE + 0x401F3;
const HRESULT CO_E_NOT_SUPPORTED        = _ERRORBASE + 0x04021;
const HRESULT REGDB_E_CLASSNOTREG       = _ERRORBASE + 0x40154;

const HRESULT ERROR_OPEN_FAILED         = _ERRORBASE + 0xE0001;

const HRESULT STG_E_ACCESSDENIED        = _ERRORBASE + 0x30001;
const HRESULT STG_E_INSUFFICIENTMEMORY  = _ERRORBASE + 0x30002;
const HRESULT STG_E_INVALIDFLAG         = _ERRORBASE + 0x30003;
const HRESULT STG_E_INVALIDFUNCTION     = _ERRORBASE + 0x30004;
const HRESULT STG_E_INVALIDPARAMETER    = _ERRORBASE + 0x30005;
const HRESULT STG_E_INVALIDPOINTER      = _ERRORBASE + 0x30006;
const HRESULT STG_E_SEEKERROR           = _ERRORBASE + 0x30007;
const HRESULT STG_E_READFAULT           = _ERRORBASE + 0x3001E;
const HRESULT STG_E_WRITEFAULT          = _ERRORBASE + 0x3001D;

#endif /* _WinError_h_ */
