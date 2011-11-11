#include "ErrorObjects.h"

#include<assert.h>

namespace ErrorObjects
{
	///////////////////////////////////////////////////////////////////////////////
	// ErrorInfo class definition
	///////////////////////////////////////////////////////////////////////////////

	ErrorInfo::ErrorInfo() : m_GUID(false), m_BstrSource(NULL), m_BstrDescript(NULL),
		m_BstrHelpFile(NULL), m_dwHelpContext(0), m_cref(1)
	{
	}

	ErrorInfo::~ErrorInfo()
	{
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetGUID(/* [out] */ GUID *pGUID)
	{
		*pGUID = m_GUID;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetSource(/* [out] */ BSTR *pBstrSource)
	{
		*pBstrSource = m_BstrSource;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetDescription(/* [out] */ BSTR *pBstrDescription)
	{
		*pBstrDescription = m_BstrDescript;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetHelpFile(/* [out] */ BSTR *pBstrHelpFile)
	{
		*pBstrHelpFile = m_BstrHelpFile;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetHelpContext(/* [out] */ DWORD *pdwHelpContext)
	{
		*pdwHelpContext = m_dwHelpContext;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetGUID(/* [in] */ REFGUID rguid)
	{
		m_GUID = rguid;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetSource(/* [in] */ LPOLESTR szSource)
	{
		m_BstrSource = szSource;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetDescription(/* [in] */ LPOLESTR szDescription)
	{
		m_BstrDescript = szDescription;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetHelpFile(/* [in] */ LPOLESTR szHelpFile)
	{
		m_BstrHelpFile = szHelpFile;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetHelpContext(/* [in] */ DWORD dwHelpContext)
	{
		m_dwHelpContext = dwHelpContext;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::QueryInterface(REFIID riid, void ** ppv)
	{
		assert(ppv != NULL);
		if (!ppv)
			return E_POINTER;

		*ppv = NULL;
		if (riid == IID_IUnknown)
			*ppv = static_cast<IUnknown *>(static_cast<IErrorInfo *>(this));
		if (riid == IID_IErrorInfo)
			*ppv = static_cast<IErrorInfo *>(this);
		else if (riid == IID_ICreateErrorInfo)
			*ppv = static_cast<ICreateErrorInfo *>(this);
		else
			return E_NOINTERFACE;

		return S_OK;
	}

	UINT32 STDMETHODCALLTYPE ErrorInfo::AddRef()
	{
		assert(m_cref > 0);
		return ++m_cref;
	}

	UINT32 STDMETHODCALLTYPE ErrorInfo::Release()
	{
		assert(m_cref > 0);
		if (--m_cref > 0)
			return m_cref;

		m_cref = 1;
		delete this;
		return 0;
	}
} // end namespace ErrorInfo