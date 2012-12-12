/*
 *	COMError.h
 *
 *	Error codes for COM compatibility library
 *
 *	Neil Mayhew - 2012-11-28
 *
 * COM Support Library
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

#ifndef _COMError_h_
#define _COMError_h_

#include <BasicTypes.h>

const HRESULT _ERRORBASE = 0x80000000;

inline bool FAILED(HRESULT hr)    { return hr & _ERRORBASE; }
inline bool SUCCEEDED(HRESULT hr) { return !FAILED(hr); }

const HRESULT S_OK                      = 0;
const HRESULT S_FALSE                   = 1;

const HRESULT ERROR_SUCCESS             = 0;

const HRESULT E_INVALIDARG              = _ERRORBASE + 0x70057L;
const HRESULT E_NOINTERFACE             = _ERRORBASE + 0x04002L;
const HRESULT E_NOTIMPL                 = _ERRORBASE + 0x04001L;
const HRESULT E_OUTOFMEMORY             = _ERRORBASE + 0x7000EL;

const HRESULT CLASS_E_CLASSNOTAVAILABLE = _ERRORBASE + 0x40111L;
const HRESULT CLASS_E_NOAGGREGATION     = _ERRORBASE + 0x40110L;
const HRESULT CO_E_CLASSSTRING          = _ERRORBASE + 0x401F3L;
const HRESULT CO_E_NOT_SUPPORTED        = _ERRORBASE + 0x04021L;
const HRESULT REGDB_E_CLASSNOTREG       = _ERRORBASE + 0x40154L;

#endif /* _COMError_h_ */
