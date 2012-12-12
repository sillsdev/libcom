/*
 * ErrorObjects.h
 *
 * Classes supporting IErrorInfo and ICreateErrorInfo
 *
 * 2008-06-11 - Tom Hindle
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

#pragma once
#ifndef _ErrorObjects_H__
#define _ErrorObjects_H__

#include <COMInterfacesMore.h>

#include "WinError.h"

/* namesapce ErrorInfo provides implementations for ICreateErrorInfo and IErrorInfo
 *
 */
namespace ErrorObjects
{

/* class ErrorInfo basic implementation of IErrorInfo and CreateErrorInfo
 */
class ErrorInfo : public IErrorInfo, public ICreateErrorInfo
{
public:

	// default construtor
	ErrorInfo();

	// virual destructor
	virtual ~ErrorInfo();

	// IUnknown Methods
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppv);
	virtual UINT32  STDMETHODCALLTYPE AddRef();
	virtual UINT32  STDMETHODCALLTYPE Release();

	// IErrorInfo methods
	virtual HRESULT STDMETHODCALLTYPE GetGUID(
		/* [out] */ GUID *pGUID);

	virtual HRESULT STDMETHODCALLTYPE GetSource(
		/* [out] */ BSTR *pBstrSource);

	virtual HRESULT STDMETHODCALLTYPE GetDescription(
		/* [out] */ BSTR *pBstrDescription);

	virtual HRESULT STDMETHODCALLTYPE GetHelpFile(
		/* [out] */ BSTR *pBstrHelpFile);

	virtual HRESULT STDMETHODCALLTYPE GetHelpContext(
		/* [out] */ DWORD *pdwHelpContext);

	// ICreateErrorInfo methods
	virtual HRESULT STDMETHODCALLTYPE SetGUID(
		/* [in] */ REFGUID rguid);

	virtual HRESULT STDMETHODCALLTYPE SetSource(
		/* [in] */ LPOLESTR szSource);

	virtual HRESULT STDMETHODCALLTYPE SetDescription(
		/* [in] */ LPOLESTR szDescription);

	virtual HRESULT STDMETHODCALLTYPE SetHelpFile(
		/* [in] */ LPOLESTR szHelpFile);

	virtual HRESULT STDMETHODCALLTYPE SetHelpContext(
		/* [in] */ DWORD dwHelpContext);

protected:

	// IErrorInfo Data members

	GUID m_GUID;
	BSTR m_BstrSource;
	BSTR m_BstrDescript;
	BSTR m_BstrHelpFile;
	DWORD m_dwHelpContext;

	// reference counter
	int m_cref;
};

}

#endif
