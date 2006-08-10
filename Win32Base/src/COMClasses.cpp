/*
 *	$Id$
 *
 *	Standard COM classes
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#include "COMClasses.h"
#include "WinError.h"

// Define GUIDs

#define DECLARE_INTERFACE(N, G)	GUID __uuidof(N)(G)

DECLARE_INTERFACE(IUnknown, "00000000-0000-0000-C000-000000000046");

DECLARE_INTERFACE(IClassFactory, "00000001-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IDataObject, "0000010e-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IDispatch, "00020400-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IStorage, "0000000b-0000-0000-C000-000000000046");

DECLARE_INTERFACE(IRpcStubBuffer, "D5F56AFC-593B-101A-B569-08002B2DBF7A");
DECLARE_INTERFACE(IRpcChannelBuffer, "D5F56B60-593B-101A-B569-08002B2DBF7A");

DECLARE_INTERFACE(IEnumFORMATETC, "00000103-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IEnumSTATDATA, "00000105-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IAdviseSink, "0000010f-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IRecordInfo, "0000002F-0000-0000-C000-000000000046");
DECLARE_INTERFACE(IShellFolder, "000214E6-0000-0000-C000-000000000046");

DECLARE_INTERFACE(ISequentialStream, "0c733a30-2a1c-11ce-ade5-00aa0044773d");
DECLARE_INTERFACE(IStream, "0000000c-0000-0000-C000-000000000046");

DECLARE_INTERFACE(IErrorInfo, "1CF2B120-547D-101B-8E65-08002B2BD119");
DECLARE_INTERFACE(ICreateErrorInfo, "22F03340-547D-101B-8E65-08002B2BD119");
DECLARE_INTERFACE(ISupportErrorInfo, "DF0B3D60-548F-101B-8E65-08002B2BD119");

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

#undef DECLARE_INTERFACE

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
