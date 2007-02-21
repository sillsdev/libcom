/*
 *	$Id$
 *
 *	Standard COM classes
 *
 *	Neil Mayhew - 2005-04-22
 *
 * COM Support Library
 * Copyright (C) 2007 SIL
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

#include "COMInterfaces.h"
#include "WinError.h"

#if 0
    IErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetGUID(
            /* [out] */ GUID *pGUID) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetSource(
            /* [out] */ BSTR *pBstrSource) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetDescription(
            /* [out] */ BSTR *pBstrDescription) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetHelpFile(
            /* [out] */ BSTR *pBstrHelpFile) = 0;

        virtual HRESULT STDMETHODCALLTYPE GetHelpContext(
            /* [out] */ DWORD *pdwHelpContext) = 0;

    };

    ICreateErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetGUID(
            /* [in] */ REFGUID rguid) = 0;
            
        virtual HRESULT STDMETHODCALLTYPE SetSource(
            /* [in] */ LPOLESTR szSource) = 0;
            
        virtual HRESULT STDMETHODCALLTYPE SetDescription(
            /* [in] */ LPOLESTR szDescription) = 0;
            
        virtual HRESULT STDMETHODCALLTYPE SetHelpFile(
            /* [in] */ LPOLESTR szHelpFile) = 0;
            
        virtual HRESULT STDMETHODCALLTYPE SetHelpContext(
            /* [in] */ DWORD dwHelpContext) = 0;
            
    };

    ISupportErrorInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo(
            /* [in] */ REFIID riid) = 0;
        
    };
#endif

#if 0

// CUnknown

class CUnknown : public IUnknown
{
	ULONG refcnt_;
protected:
	CUnknown();
public:
	virtual ULONG AddRef();
	virtual ULONG Release();
	virtual HRESULT QueryInterface(REFIID riid, void** ppv);
};

ULONG CUnknown::AddRef()
{
	return InterlockedIncrement(&refcnt_);
}

ULONG CUnknown::Release()
{
	ULONG result = InterlockedDecrement(&refcnt_);
	if (result == 0)
		delete this;
	return result;
}

HRESULT CUnknown::QueryInterface(REFIID riid, void** ppv)
{
	if (*riid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
		return S_OK;
	}
	return E_NOINTERFACE;
}

// CSequentialStream

class CSequentialStream : public ISequentialStream
{
	ULONG refcnt_;
public:
	// IUnknown
	virtual ULONG AddRef();
	virtual ULONG Release();
	virtual HRESULT QueryInterface(REFIID riid, void** ppv);
	
	// ISequentialStream
	virtual HRESULT STDMETHODCALLTYPE Read( 
		void* pv,
		ULONG cb,
		ULONG* pcbRead);
	virtual HRESULT STDMETHODCALLTYPE Write( 
		const void* pv,
		ULONG cb,
		ULONG* pcbWritten);
};

ULONG CSequentialStream::AddRef()
{
	return InterlockedIncrement(&refcnt_);
}

ULONG CSequentialStream::Release()
{
	ULONG result = InterlockedDecrement(&refcnt_);
	if (result == 0)
		delete this;
	return result;
}

HRESULT CSequentialStream::QueryInterface(REFIID riid, void** ppv)
{
	if (*riid == IID_ISequentialStream)
	{
		*ppv = static_cast<ISequentialStream*>(this);
		return S_OK;
	}
	if (*riid == IID_IUnknown)
	{
		*ppv = static_cast<IUnknown*>(this);
		return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CSequentialStream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
}
	
HRESULT STDMETHODCALLTYPE CSequentialStream::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
}

#endif
