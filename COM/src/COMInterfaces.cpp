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

#define DEFINE_INTERFACE_GUID(N, G)	\
	template<> GUID __uuidof(N)(G)

DEFINE_INTERFACE_GUID(IUnknown, "00000000-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IClassFactory, "00000001-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IDataObject, "0000010e-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IDispatch, "00020400-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IStorage, "0000000b-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IRpcStubBuffer, "D5F56AFC-593B-101A-B569-08002B2DBF7A");
DEFINE_INTERFACE_GUID(IRpcChannelBuffer, "D5F56B60-593B-101A-B569-08002B2DBF7A");

DEFINE_INTERFACE_GUID(IEnumFORMATETC, "00000103-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IEnumSTATDATA, "00000105-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IAdviseSink, "0000010f-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IRecordInfo, "0000002F-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(IShellFolder, "000214E6-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(ISequentialStream, "0c733a30-2a1c-11ce-ade5-00aa0044773d");
DEFINE_INTERFACE_GUID(IStream, "0000000c-0000-0000-C000-000000000046");

DEFINE_INTERFACE_GUID(IErrorInfo, "1CF2B120-547D-101B-8E65-08002B2BD119");
DEFINE_INTERFACE_GUID(ICreateErrorInfo, "22F03340-547D-101B-8E65-08002B2BD119");
DEFINE_INTERFACE_GUID(ISupportErrorInfo, "DF0B3D60-548F-101B-8E65-08002B2BD119");

DEFINE_INTERFACE_GUID(IPicture, "7BF80980-BF32-101A-8BBB-00AA00300CAB");
DEFINE_INTERFACE_GUID(IAccessible, "618736e0-3c3d-11cf-810c-00aa00389b71");
DEFINE_INTERFACE_GUID(IServiceProvider, "6d5140c1-7436-11ce-8034-00aa006009fa");
DEFINE_INTERFACE_GUID(IEnumVARIANT, "00020404-0000-0000-C000-000000000046");
DEFINE_INTERFACE_GUID(ITypeInfo, "00020401-0000-0000-C000-000000000046");

#undef DEFINE_INTERFACE_GUID

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
