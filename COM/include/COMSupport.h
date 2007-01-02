/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupport.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			2001-05-24	Neil Mayhew

	Support for COM in FieldWorks on MacOS.

Contains definitions copied and (sometimes modified) from the Windows headers:

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

Added CoInitialize() and made other API tweaks according to changes in COMSupport.cpp.
Moved RegisterServer() into its own header file so our COMSupport.h headers can optionally not be 
included, yet the definition for RegisterServer() still be included. (Not including the COMSupport.h 
may mean we're using, for example, the Wine headers.)
Made CoCreateInstance() extern C.
	2007-01-02, MarkS
Moved MultiByteToWideChar and WideCharToMultiByte to here from COMSupport.h.
	2003-06-23, GDLC
Fixed definition of first parameter to CoCreateInstance(). Minor adjustments to typedefs
and #defines for GUIDs, IIDs, CLSIDs and their references. Removed unwanted "static"s.
	2003-05-14, GDLC
Added RegisterServer() for registering class factories in our ComRegistry.
	2003-05-01, GDLC
Added line numbers in Windows headers from which the excerpts are taken.
Added CoGetClassObject().
	2003-04-21, GDLC
Started to modify to use CFPlugIn.h & CFPlugInCOM.h from Core Foundation (Carbon)
but reverted to original approach.
	2003-03-31, GDLC
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

#ifndef FILE_COMSUPPORT_SEEN
#define FILE_COMSUPPORT_SEEN

#ifndef __GNUC__
#pragma once
#endif

//
//	The header WinSupport.h should be included before this header.
//

//--------------------------------------------------------------------------------------------
// Copied, with adaptations, from the Windows header GUIDDEF.H
typedef struct
{
    unsigned long	Data1;
    unsigned short	Data2;
    unsigned short	Data3;
    unsigned char	Data4[8];
} GUID;

//#include <string.h>

typedef GUID			*LPGUID;
typedef const GUID 		*LPCGUID;
#define REFGUID			const GUID &

typedef	GUID			IID;
#define REFIID			const IID &

typedef	GUID			CLSID;
typedef	CLSID			*LPCLSID;
#define REFCLSID		const IID &

//Line 149
// Faster (but makes code fatter) inline version...use sparingly
__inline int InlineIsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
   return (
      ((unsigned long *) &rguid1)[0] == ((unsigned long *) &rguid2)[0] &&
      ((unsigned long *) &rguid1)[1] == ((unsigned long *) &rguid2)[1] &&
      ((unsigned long *) &rguid1)[2] == ((unsigned long *) &rguid2)[2] &&
      ((unsigned long *) &rguid1)[3] == ((unsigned long *) &rguid2)[3]);
}

// Line 158
// Slower (but keeps code size down) inline version...use mostly
__inline int IsEqualGUID(REFGUID rguid1, REFGUID rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}

// Line 191
__inline int operator==(REFGUID guidOne, REFGUID guidOther)
{
    return IsEqualGUID(guidOne,guidOther);
}

// Line 196
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

// Line 3148
#define DECLSPEC_UUID(x)
// Line 3152
#define MIDL_INTERFACE(x)   struct

#define EXTERN_GUID(itf,l1,s1,s2,c1,c2,c3,c4,c5,c6,c7,c8) EXTERN_C const IID itf

//--end copy from RPCNDR.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header OBJBASE.H
//	Other items from OBJBASE.H are at the end of this header.

// Extract from commentary in OBJBASE.H:
//	These are macros for declaring interfaces.  They exist so that
//	a single definition of the interface is simultaneously a proper
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

// Line 172
#define interface struct
#define STDMETHOD(method)       virtual HRESULT method
#define STDMETHOD_(type,method) virtual type method
#define STDMETHODV(method)       virtual HRESULT method
#define STDMETHODV_(type,method) virtual type method
#define PURE                    = 0
#define THIS_
#define THIS                    void
#define DECLARE_INTERFACE(iface)    interface iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface iface : public baseiface

#define BEGIN_INTERFACE virtual void a() {}
#define END_INTERFACE

// Line 282
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

// Line 617 (tho modified for a const parameter)
HRESULT CLSIDFromString(LPCOLESTR lpsz, LPCLSID pclsid);
// Line 618 (tho modified for a const parameter)
HRESULT StringFromCLSID(const CLSID& rclsid, LPOLESTR* ppsz);

// Line 627
HRESULT CoCreateGuid(GUID *pguid);

// Line 692
void *	CoTaskMemAlloc(SIZE_T cb);
// Line 693
void *	CoTaskMemRealloc(LPVOID pv, SIZE_T cb);
// Line 694
void	CoTaskMemFree(LPVOID pv);

//--end copy from OBJBASE.H-------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the ActiveX header OLEAUTO.H

// Line 27
#define WINOLEAUTAPI        STDAPI
// Line 28
#define WINOLEAUTAPI_(type) STDAPI_(type)

// Line 64
WINOLEAUTAPI_(BSTR) SysAllocString(const OLECHAR *);
// Line 65
WINOLEAUTAPI_(INT)  SysReAllocString(BSTR *, const OLECHAR *);
// Line 66
WINOLEAUTAPI_(BSTR) SysAllocStringLen(const OLECHAR *, UINT);
// Line 67
WINOLEAUTAPI_(INT)  SysReAllocStringLen(BSTR *, const OLECHAR *, UINT);
// Line 68
WINOLEAUTAPI_(void) SysFreeString(BSTR);

// FieldWorks does not yet use this one:
// Line 69
//WINOLEAUTAPI_(UINT) SysStringLen(BSTR);

// FieldWorks does not yet use this one:
// Line 72
//WINOLEAUTAPI_(UINT) SysStringByteLen(BSTR bstr);

// Line 73
WINOLEAUTAPI_(BSTR) SysAllocStringByteLen(LPCSTR psz, UINT len);

//--end copy from OLEAUTO.H-------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header UNKNWN.H

// Line 160
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

// Line 93
// For CodeWarrior needs to be after IUnknown
typedef	IUnknown*	LPUNKNOWN;

//	{00000000-0000-0000-C000-000000000046}
const IID IID_IUnknown =
	{0x00000000, 0x0000, 0x0000,
	{0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46}};

// Line 471
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
            /* [in] */ BOOL fLock) = 0;
	END_INTERFACE
};

// Line 464
// For CodeWarrior needs to be after IClassFactory
typedef IClassFactory*	LPCLASSFACTORY;

//	{00000001-0000-0000-C000-000000000046}
const IID IID_IClassFactory =
	{0x00000001, 0x0000, 0x0000,
	{0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x46}};

//--end copy from UNKNWN.H--------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Copied, with adaptations, from the Windows header OBJIDL.H

// Line 4314
typedef struct
    {                      
        LPOLESTR		pwcsName;
//		FSSpec			*pspec;		// For apps ported from Windows this must not be present.
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
    
// Line 4346
typedef enum
    {
    	STGTY_STORAGE	= 1,
		STGTY_STREAM	= 2,
		STGTY_LOCKBYTES	= 3,
		STGTY_PROPERTY	= 4
    } STGTY;

// Line 4354
typedef enum
    {
    	STREAM_SEEK_SET	= 0,
		STREAM_SEEK_CUR	= 1,
		STREAM_SEEK_END	= 2
    } STREAM_SEEK;

// Line 4361
typedef enum
    {
    	LOCK_WRITE		= 1,
		LOCK_EXCLUSIVE	= 2,
		LOCK_ONLYONCE	= 4
    } LOCKTYPE;

// Line 4191
MIDL_INTERFACE("0c733a30-2a1c-11ce-ade5-00aa0044773d")
ISequentialStream : public IUnknown
{
public:
	BEGIN_INTERFACE
		virtual /* [local] */	HRESULT	Read( 
			/* [length_is][size_is][out] */	void *pv,
			/* [in] */			ULONG	cb,
			/* [out] */			ULONG	*pcbRead) = 0;

		virtual /* [local] */	HRESULT	Write( 
			/* [size_is][in] */ const	void *pv,
			/* [in] */			ULONG	cb,
			/* [out] */			ULONG	*pcbWritten) = 0;
	END_INTERFACE
};

// Line 4373
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

// Line 447
HRESULT CoGetClassObject(
	/*IN*/ REFCLSID		rclsid,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ LPVOID		pvReserved,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv);

// Line 567
extern "C" HRESULT CoCreateInstance(
	/*IN*/ REFCLSID		rclsid,
	/*IN*/ LPUNKNOWN	pUnkOuter,
	/*IN*/ DWORD		dwClsContext,
	/*IN*/ REFIID		riid,
	/*OUT*/ LPVOID*		ppv);

//--end copy from OBJBASE.H-------------------------------------------------------------------

/**
 * CoInitialize
 * 
 * (http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp)
 */
extern "C" HRESULT CoInitialize(
	LPVOID pvReserved);


#include "COMLibrary.h"

#endif //FILE_COMSUPPORT_SEEN
