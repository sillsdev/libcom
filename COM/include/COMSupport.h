/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupport.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			2001-05-24	Neil Mayhew

	Support for COM in FieldWorks on MacOS.

Contains definitions copied and (sometimes modified) from the Windows headers:

	WINNLS.H
	GUIDDEF.H
	RPCNDR.H
	OBJBASE.H
	OLEAUTO.H
	UNKNWN.H
	OBJIDL.H
	OBJBASE.H	(some more items - dependent on UNKNWN.H)

The copied portions are included here in sections named according to the above Windows
headers. It is suggested that further bits copied from Windows headers be kept in
these sections to facilitate later reading and debugging of the code.

Changed definitions of enums & structs to suit C++ (this file is not intended for
compilation as a C source)
	2002-03-08, GDLC
Added REFCLSID.
	2002-02-28, GDLC
Added CLSCTX_INPROC and CLSCTX_SERVER and improved commentary.
Items from WTYPES.H moved to WinSupport.h
	2002-02-25, GDLC
Added STDMETHOD(method) and STDMETHOD_(type,method) from OBJBASE.H.
Added SysAllocString(), SysReAllocString(), SysAllocStringLen(),
SysReAllocStringLen(), SysFreeString(), SysStringLen(),
SysStringByteLen(BSTR bstr), and SysAllocStringByteLen() from OLEAUTO.H.
Added CLSCTX_ALL from OBJBASE.H.
	2002-02-22, GDLC
Improved commentary. Adjusted for GUID definitions from GUIDDEF.H. Added extra GUID comparison
operators from UtilCom.h.
	2002-02-13, GDLC
Changed to #pragma once
	2002-01-17
Removed non-COM items to WinSupport.h
	2001-07-17, GDLC
Removed internal implementation definitions to COMSupportInternals.h
	2001-07-19, GDLC
Added BSTR & LPBSTR
	2001-08-28, GDLC
----------------------------------------------------------------------------------------------*/

#pragma once

//	The header WinSupport.h should be included before this header.

//--------------------------------------------------------------------------------------------
// Copied, with adaptations, from the Windows header  WINNLS.H

//  Code Page Default Values.

#define CP_ACP					0			// default to ANSI code page
#define CP_OEMCP				1			// default to OEM  code page
#define CP_MACCP				2			// default to MAC  code page
#define CP_THREAD_ACP			3			// current thread's ANSI code page
#define CP_SYMBOL				42			// SYMBOL translations

#define CP_UTF7					65000		// UTF-7 translation
#define CP_UTF8					65001		// UTF-8 translation

int MultiByteToWideChar(
	UINT		CodePage,
	DWORD		dwFlags,
	LPCSTR		lpMultiByteStr,
	int			cbMultiByte,
	LPWSTR		lpWideCharStr,
    int			cchWideChar);

int WideCharToMultiByte(
	UINT		CodePage,
	DWORD		dwFlags,
	LPCWSTR		lpWideCharStr,
	int			cchWideChar,
	LPSTR		lpMultiByteStr,
	int			cbMultiByte,
	LPCSTR		lpDefaultChar,
	LPBOOL		lpUsedDefaultChar);

//--end copy from WINNLS.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
// Copied, with adaptations, from the Windows header GUIDDEF.H
typedef struct
{
    unsigned long	Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char	Data4[8];
} GUID;

#include <string.h>

typedef GUID			*LPGUID;
typedef const GUID 		*LPCGUID;
#define REFGUID			const GUID &

typedef	GUID			IID;
#define REFIID			const IID &

typedef	GUID			CLSID;
#define REFGUID			const GUID &

typedef	CLSID			*LPCLSID;
#define REFCLSID		const IID &

// Faster (but makes code fatter) inline version...use sparingly
__inline int InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
   return (
      ((unsigned long *) &rguid1)[0] == ((unsigned long *) &rguid2)[0] &&
      ((unsigned long *) &rguid1)[1] == ((unsigned long *) &rguid2)[1] &&
      ((unsigned long *) &rguid1)[2] == ((unsigned long *) &rguid2)[2] &&
      ((unsigned long *) &rguid1)[3] == ((unsigned long *) &rguid2)[3]);
}

// Slower (but keeps code size down) inline version...use mostly
__inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}

__inline int operator==(REFGUID guidOne, REFGUID guidOther)
{
    return IsEqualGUID(guidOne,guidOther);
}

__inline int operator!=(REFGUID guidOne, REFGUID guidOther)
{
    return !(guidOne == guidOther);
}

// Added < operator to enable GUIDs to be used in std::map for MacFieldWorks
__inline bool operator < (REFGUID guidOne, REFGUID guidOther)
{
	return memcmp(&guidOne, &guidOther, sizeof(GUID)) < 0;
}

// Added >, <=, >= operators from UtilCom.h
__inline bool operator > (REFGUID guidOne, REFGUID guidOther)
{
	return memcmp(&guidOne, &guidOther, sizeof(GUID)) > 0;
}

__inline bool operator <= (REFGUID guidOne, REFGUID guidOther)
{
	return memcmp(&guidOne, &guidOther, sizeof(GUID)) <= 0;
}

__inline bool operator >= (REFGUID guidOne, REFGUID guidOther)
{
	return memcmp(&guidOne, &guidOther, sizeof(GUID)) >= 0;
}

//--end copy from GUIDDEF.H ------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header RPCNDR.H

/****************************************************************************
 * Special things for VC5 Com support
 ****************************************************************************/

#define DECLSPEC_UUID(x)
#define MIDL_INTERFACE(x)   struct

#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8) EXTERN_C const IID itf

//--end copy from RPCNDR.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header OBJBASE.H
//	Other items from OBJBASE.H are at the end of this header.

// Extract from commentary in OBJBASE.H:
//	These are macros for declaring interfaces.  They exist so that
//	a single definition of the interface is simulataneously a proper
//	declaration of the interface structures (C++ abstract classes)
//	for both C and C++.
//
//	DECLARE_INTERFACE(iface) is used to declare an interface that does
//	not derive from a base interface.
//	DECLARE_INTERFACE_(iface, baseiface) is used to declare an interface
//	that does derive from a base interface.
//
// Even though, for MacFieldWorks, we are not interested in C compilation,
// we need these macros because there are many uses of them in FieldWorks
// source code.

#define BEGIN_INTERFACE
#define END_INTERFACE

#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method

#define CLSCTX_INPROC           (CLSCTX_INPROC_SERVER|CLSCTX_INPROC_HANDLER)

//	TODO: Are these restrictive definitions of CLSCTX_ALL and CLSCTX_SERVER
//	really necessary? Or should MacFieldWorks use CLSCTX_INPROC instead?
//
//	The full definitions of CLSCTX_ALL and CLSCTX_SERVER include
//	CLSCTX_LOCAL_SERVER and CLSCTX_REMOTE_SERVER, which allows for
//	out-of-process COM whether on the same machine or across a network.
//	For MacFieldWorks we need to restrict this to in-process COM.
#define CLSCTX_ALL	(CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER)
#define CLSCTX_SERVER	(CLSCTX_INPROC_SERVER)

HRESULT CoCreateGuid(GUID *pguid);

HRESULT CLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid);
HRESULT StringFromCLSID(CLSID& rclsid, LPOLESTR* ppsz);

void *	CoTaskMemAlloc(SIZE_T cb);
void *	CoTaskMemRealloc(LPVOID pv, SIZE_T cb);
void	CoTaskMemFree(LPVOID pv);

//--end copy from OBJBASE.H-------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the ActiveX header OLEAUTO.H

#define WINOLEAUTAPI        STDAPI
#define WINOLEAUTAPI_(type) STDAPI_(type)

WINOLEAUTAPI_(BSTR) SysAllocString(const OLECHAR *);
WINOLEAUTAPI_(INT)  SysReAllocString(BSTR *, const OLECHAR *);
WINOLEAUTAPI_(BSTR) SysAllocStringLen(const OLECHAR *, UINT);
WINOLEAUTAPI_(INT)  SysReAllocStringLen(BSTR *, const OLECHAR *, UINT);
WINOLEAUTAPI_(void) SysFreeString(BSTR);
// FieldWorks does not yet use this one:
//WINOLEAUTAPI_(UINT) SysStringLen(BSTR);

// FieldWorks does not yet use this one:
//WINOLEAUTAPI_(UINT) SysStringByteLen(BSTR bstr);
WINOLEAUTAPI_(BSTR) SysAllocStringByteLen(LPCSTR psz, UINT len);

//--end copy from OLEAUTO.H-------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header UNKNWN.H

MIDL_INTERFACE("00000000-0000-0000-C000-000000000046")
IUnknown
{
public:
	BEGIN_INTERFACE
		virtual HRESULT QueryInterface( 
			/* [in] */ REFIID riid,
			/* [out] */ void **ppvObject) = 0;
        
		virtual ULONG AddRef( void) = 0;
        
		virtual ULONG Release( void) = 0;
	END_INTERFACE
};

typedef	IUnknown*	LPUNKNOWN;

MIDL_INTERFACE("00000001-0000-0000-C000-000000000046")
IClassFactory : public IUnknown
{
public:
	BEGIN_INTERFACE
		virtual HRESULT CreateInstance( 
            /* [in] */ IUnknown*	pUnkOuter,
            /* [in] */ REFIID		riid,
            /* [out] */ void**		ppvObject) = 0;
        
		virtual HRESULT LockServer( 
            /* [in] */ bool fLock) = 0;
	END_INTERFACE
};

typedef IClassFactory*	LPCLASSFACTORY;

//--end copy from UNKNWN.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header OBJIDL.H

typedef struct
    {                      
        LPOLESTR		pwcsName;
		FSSpec			*pspec;
        DWORD			type;
        ULARGE_INTEGER	cbSize;
        FILETIME		mtime;
        FILETIME		ctime;
        FILETIME		atime;
        DWORD			grfMode;
        DWORD			grfLocksSupported;
        CLSID			clsid;
        DWORD			grfStateBits;
        DWORD			reserved;
    } STATSTG;
    
typedef enum
    {
    	STGTY_STORAGE	= 1,
		STGTY_STREAM	= 2,
		STGTY_LOCKBYTES	= 3,
		STGTY_PROPERTY	= 4
    } STGTY;

typedef enum
    {
    	STREAM_SEEK_SET	= 0,
		STREAM_SEEK_CUR	= 1,
		STREAM_SEEK_END	= 2
    } STREAM_SEEK;

typedef enum
    {
    	LOCK_WRITE		= 1,
		LOCK_EXCLUSIVE	= 2,
		LOCK_ONLYONCE	= 4
    } LOCKTYPE;

MIDL_INTERFACE("0c733a30-2a1c-11ce-ade5-00aa0044773d")
ISequentialStream : public IUnknown
{
public:
	BEGIN_INTERFACE
		virtual /* [local] */ HRESULT Read( 
			/* [length_is][size_is][out] */ void *pv,
			/* [in] */ ULONG cb,
			/* [out] */ ULONG *pcbRead) = 0;

		virtual /* [local] */ HRESULT Write( 
			/* [size_is][in] */ const void *pv,
			/* [in] */ ULONG cb,
			/* [out] */ ULONG *pcbWritten) = 0;
	END_INTERFACE
};

MIDL_INTERFACE("0000000c-0000-0000-C000-000000000046")
IStream : public ISequentialStream
{
public:
	BEGIN_INTERFACE
		virtual /* [local] */ HRESULT Seek( 
			/* [in] */ LARGE_INTEGER dlibMove,
			/* [in] */ DWORD dwOrigin,
			/* [out] */ ULARGE_INTEGER *plibNewPosition) = 0;

		virtual HRESULT SetSize( 
			/* [in] */ ULARGE_INTEGER libNewSize) = 0;

		virtual /* [local] */ HRESULT CopyTo( 
			/* [unique][in] */ IStream *pstm,
			/* [in] */ ULARGE_INTEGER cb,
			/* [out] */ ULARGE_INTEGER *pcbRead,
			/* [out] */ ULARGE_INTEGER *pcbWritten) = 0;

		virtual HRESULT Commit( 
			/* [in] */ DWORD grfCommitFlags) = 0;

		virtual HRESULT Revert( void) = 0;

		virtual HRESULT LockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) = 0;

		virtual HRESULT UnlockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) = 0;

		virtual HRESULT Stat( 
			/* [out] */ STATSTG *pstatstg,
			/* [in] */ DWORD grfStatFlag) = 0;

		virtual HRESULT Clone( 
			/* [out] */ IStream **ppstm) = 0;
	END_INTERFACE
};

//--end copy from OBJIDL.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header OBJBASE.H
//	These items needed to be placed at the end of this header because they depend
//	on UNKNWN.H.

static HRESULT CoCreateInstance(
	/*IN*/ CLSID		&rclsid,
	/*IN*/ LPUNKNOWN	pUnkOuter,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv);

//--end copy from OBJBASE.H-------------------------------------------------------------------
