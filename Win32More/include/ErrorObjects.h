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
