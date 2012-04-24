#include "ErrorObjects.h"

#include<assert.h>

#define BEGIN_COM_METHOD \
	try { \

#define END_COM_METHOD \
	} catch (...) { return E_UNEXPECTED; }\
	return S_OK; \

namespace ErrorObjects
{
	///////////////////////////////////////////////////////////////////////////////
	// ErrorInfo class definition
	///////////////////////////////////////////////////////////////////////////////

	ErrorInfo::ErrorInfo()
	:	m_BstrSource(NULL), m_BstrDescript(NULL),
		m_BstrHelpFile(NULL), m_dwHelpContext(0),
		m_cref(1)
	{
		m_GUID.clear();
	}

	ErrorInfo::~ErrorInfo()
	{
		::SysFreeString(m_BstrSource);
		::SysFreeString(m_BstrDescript);
		::SysFreeString(m_BstrHelpFile);
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetGUID(/* [out] */ GUID *pGUID)
	{
		BEGIN_COM_METHOD
		*pGUID = m_GUID;
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetSource(/* [out] */ BSTR *pBstrSource)
	{
		BEGIN_COM_METHOD
		*pBstrSource = ::SysAllocString(m_BstrSource);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetDescription(/* [out] */ BSTR *pBstrDescription)
	{
		BEGIN_COM_METHOD
		*pBstrDescription = ::SysAllocString(m_BstrDescript);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetHelpFile(/* [out] */ BSTR *pBstrHelpFile)
	{
		BEGIN_COM_METHOD
		*pBstrHelpFile = ::SysAllocString(m_BstrHelpFile);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::GetHelpContext(/* [out] */ DWORD *pdwHelpContext)
	{
		BEGIN_COM_METHOD
		*pdwHelpContext = m_dwHelpContext;
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetGUID(/* [in] */ REFGUID rguid)
	{
		BEGIN_COM_METHOD
		m_GUID = rguid;
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetSource(/* [in] */ LPOLESTR szSource)
	{
		BEGIN_COM_METHOD
		::SysFreeString(m_BstrSource);
		m_BstrSource = ::SysAllocString(szSource);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetDescription(/* [in] */ LPOLESTR szDescription)
	{
		BEGIN_COM_METHOD
		::SysFreeString(m_BstrDescript);
		m_BstrDescript = ::SysAllocString(szDescription);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetHelpFile(/* [in] */ LPOLESTR szHelpFile)
	{
		BEGIN_COM_METHOD
		::SysFreeString(m_BstrHelpFile);
		m_BstrHelpFile = ::SysAllocString(szHelpFile);
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::SetHelpContext(/* [in] */ DWORD dwHelpContext)
	{
		BEGIN_COM_METHOD
		m_dwHelpContext = dwHelpContext;
		END_COM_METHOD
	}

	HRESULT STDMETHODCALLTYPE ErrorInfo::QueryInterface(REFIID riid, void ** ppv)
	{
		BEGIN_COM_METHOD
		assert(ppv != NULL);
		if (!ppv)
			return E_POINTER;

		*ppv = NULL;
		if (riid == IID_IUnknown)
			*ppv = static_cast<IUnknown *>(static_cast<IErrorInfo *>(this));
		else if (riid == IID_IErrorInfo)
			*ppv = static_cast<IErrorInfo *>(this);
		else if (riid == IID_ICreateErrorInfo)
			*ppv = static_cast<ICreateErrorInfo *>(this);
		else
			return E_NOINTERFACE;

		AddRef();
		END_COM_METHOD
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
