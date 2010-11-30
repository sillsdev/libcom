/*
 *	$Id$
 *
 *	More Standard COM interfaces
 *
 *	Neil Mayhew - 2005-04-22
 *
 * Win32More Compatibility Library
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

#ifndef COMInterfacesMore_h
#define COMInterfacesMore_h

#include "DateTime.h"
#include <ExtendedTypes.h>
#include <COMInterfaces.h>

class IErrorInfo : public IUnknown
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

#define IID_IErrorInfo __uuidof(IErrorInfo)

class ICreateErrorInfo : public IUnknown
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

#define IID_ICreateErrorInfo __uuidof(ICreateErrorInfo)

class ISupportErrorInfo : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE InterfaceSupportsErrorInfo(
		/* [in] */ REFIID riid) = 0;
	
};

#define IID_ISupportErrorInfo __uuidof(ISupportErrorInfo)

STDAPI SetErrorInfo(UINT32 dwReserved, IErrorInfo* perrinfo);
STDAPI GetErrorInfo(UINT32 dwReserved, IErrorInfo** pperrinfo);
STDAPI CreateErrorInfo(ICreateErrorInfo** pperrinfo);

enum STREAM_SEEK
{
	STREAM_SEEK_SET	= 0,
	STREAM_SEEK_CUR	= 1,
	STREAM_SEEK_END	= 2
};

enum STATFLAG
{
	STATFLAG_DEFAULT = 0,
	STATFLAG_NONAME	= 1,
	STATFLAG_NOOPEN	= 2
};

struct STATSTG
{
	LPOLESTR pwcsName;
	DWORD type;
	ULARGE_INTEGER cbSize;
	FILETIME mtime;
	FILETIME ctime;
	FILETIME atime;
	DWORD grfMode;
	DWORD grfLocksSupported;
	CLSID clsid;
	DWORD grfStateBits;
	DWORD reserved;
};

enum STGC
{
	STGC_DEFAULT = 0,
	STGC_OVERWRITE = 1,
	STGC_ONLYIFCURRENT = 2,
	STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE = 4,
	STGC_CONSOLIDATE = 8
};

enum STGTY
{
	STGTY_STORAGE   = 1,
	STGTY_STREAM    = 2,
	STGTY_LOCKBYTES = 3,
	STGTY_PROPERTY  = 4
};

enum LOCKTYPE
{
	LOCK_WRITE      = 1,
	LOCK_EXCLUSIVE  = 2,
	LOCK_ONLYONCE   = 4
};

enum
{
	STGM_READ             = 0x00,
	STGM_WRITE            = 0x01,
	STGM_READWRITE        = 0x02,
	STGM_CREATE           = 0x1000,
	STGM_FAILIFTHERE      = 0x08,
	STGM_SHARE_DENY_NONE  = 0x00,
	STGM_SHARE_DENY_READ  = 0x10,
	STGM_SHARE_DENY_WRITE = 0x20,
	STGM_SHARE_EXCLUSIVE  = STGM_SHARE_DENY_READ | STGM_SHARE_DENY_WRITE,
};

class ISequentialStream : public IUnknown
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
		/* [length_is][size_is][out] */ void *pv,
		/* [in] */ UINT32 cb,
		/* [out] */ UINT32 *pcbRead) = 0;
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
		/* [size_is][in] */ const void *pv,
		/* [in] */ UINT32 cb,
		/* [out] */ UINT32 *pcbWritten) = 0;
	
};

#define IID_ISequentialStream __uuidof(ISequentialStream)

class IStream : public ISequentialStream
{
public:
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
		/* [in] */ LARGE_INTEGER dlibMove,
		/* [in] */ DWORD dwOrigin,
		/* [out] */ ULARGE_INTEGER *plibNewPosition) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetSize( 
		/* [in] */ ULARGE_INTEGER libNewSize) = 0;
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
		/* [unique][in] */ IStream *pstm,
		/* [in] */ ULARGE_INTEGER cb,
		/* [out] */ ULARGE_INTEGER *pcbRead,
		/* [out] */ ULARGE_INTEGER *pcbWritten) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Commit( 
		/* [in] */ DWORD grfCommitFlags) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Revert( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE LockRegion( 
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
		/* [in] */ ULARGE_INTEGER libOffset,
		/* [in] */ ULARGE_INTEGER cb,
		/* [in] */ DWORD dwLockType) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Stat( 
		/* [out] */ STATSTG *pstatstg,
		/* [in] */ DWORD grfStatFlag) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Clone( 
		/* [out] */ IStream **ppstm) = 0;
	
};

#define IID_IStream __uuidof(IStream)

typedef IStream* LPSTREAM;

enum PICTUREATTRIBUTES
{
	PICTURE_SCALABLE	= 0x1,
	PICTURE_TRANSPARENT	= 0x2
};

typedef UINT32 OLE_HANDLE;

typedef INT32 OLE_XPOS_HIMETRIC;
typedef INT32 OLE_YPOS_HIMETRIC;
typedef INT32 OLE_XSIZE_HIMETRIC;
typedef INT32 OLE_YSIZE_HIMETRIC;

class IPicture : public IUnknown
{
public:

	virtual HRESULT STDMETHODCALLTYPE get_Handle( 
	/* [out] */ OLE_HANDLE *pHandle) = 0;

	virtual HRESULT STDMETHODCALLTYPE get_hPal( 
	/* [out] */ OLE_HANDLE *phPal) = 0;

	virtual HRESULT STDMETHODCALLTYPE get_Type( 
		/* [out] */ INT16 *pType) = 0;

	virtual HRESULT STDMETHODCALLTYPE get_Width( 
		/* [out] */ OLE_XSIZE_HIMETRIC *pWidth) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_Height( 
		/* [out] */ OLE_YSIZE_HIMETRIC *pHeight) = 0;

	virtual HRESULT STDMETHODCALLTYPE Render( 
		/* [in] */ HDC hDC,
		/* [in] */ INT32 x,
		/* [in] */ INT32 y,
		/* [in] */ INT32 cx,
		/* [in] */ INT32 cy,
		/* [in] */ OLE_XPOS_HIMETRIC xSrc,
		/* [in] */ OLE_YPOS_HIMETRIC ySrc,
		/* [in] */ OLE_XSIZE_HIMETRIC cxSrc,
		/* [in] */ OLE_YSIZE_HIMETRIC cySrc,
		/* [in] */ LPCRECT pRcWBounds) = 0;

	virtual HRESULT STDMETHODCALLTYPE put_hPal( 
	/* [in] */ OLE_HANDLE hPal) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_CurDC( 
		/* [out] */ HDC *phDC) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SelectPicture( 
		/* [in] */ HDC hDCIn,
		/* [out] */ HDC *phDCOut,
		/* [out] */ OLE_HANDLE *phBmpOut) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_KeepOriginalFormat( 
		/* [out] */ BOOL *pKeep) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE put_KeepOriginalFormat( 
		/* [in] */ BOOL keep) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE PictureChanged( void) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SaveAsFile( 
		/* [in] */ LPSTREAM pStream,
		/* [in] */ BOOL fSaveMemCopy,
		/* [out] */ INT32 *pCbSize) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_Attributes( 
		/* [out] */ DWORD *pDwAttr) = 0;

	virtual HRESULT STDMETHODCALLTYPE get_Hdc( 
	/* [in] */ DWORD pDwAttr) = 0;
};

#define IID_IPicture __uuidof(IPicture)

typedef IPicture *LPPICTURE;

enum
{
	ROLE_SYSTEM_CLIENT,
	ROLE_SYSTEM_GROUPING,
	ROLE_SYSTEM_SEPARATOR,
	ROLE_SYSTEM_GRAPHIC,
	ROLE_SYSTEM_TEXT,
	ROLE_SYSTEM_TABLE,
	ROLE_SYSTEM_ROW,
	ROLE_SYSTEM_CELL,
	STATE_SYSTEM_SELECTABLE,
	STATE_SYSTEM_INVISIBLE,
};

class IAccessible : public IUnknown
{
};

#define IID_IAccessible __uuidof(IAccessible)

class IServiceProvider : public IUnknown
{
};

#define IID_IServiceProvider __uuidof(IServiceProvider)

class IEnumVARIANT : public IUnknown
{
};

#define IID_IEnumVARIANT __uuidof(IEnumVARIANT)

class ITypeInfo : public IUnknown
{
};

#define IID_ITypeInfo __uuidof(ITypeInfo)

// Variants

typedef short VARIANT_BOOL;
enum { VARIANT_FALSE = 0, VARIANT_TRUE = -1};

typedef unsigned short VARTYPE;

struct VARIANT
{
	union
	{
		struct
		{
			VARTYPE vt;
			WORD wReserved1;
			WORD wReserved2;
			WORD wReserved3;
			union
			{
				INT64 llVal;
				INT32 lVal;
				BYTE bVal;
				INT16 iVal;
				FLOAT fltVal;
				DOUBLE dblVal;
				VARIANT_BOOL boolVal;
	//			_VARIANT_BOOL bool;
				SCODE scode;
				CY cyVal;
				DATE date;
				BSTR bstrVal;
				IUnknown *punkVal;
				IDispatch *pdispVal;
	//			SAFEARRAY *parray;
				BYTE *pbVal;
				INT16 *piVal;
				INT32 *plVal;
				INT64 *pllVal;
				FLOAT *pfltVal;
				DOUBLE *pdblVal;
				VARIANT_BOOL *pboolVal;
	//			_VARIANT_BOOL *pbool;
				SCODE *pscode;
				CY *pcyVal;
				DATE *pdate;
				BSTR *pbstrVal;
				IUnknown **ppunkVal;
				IDispatch **ppdispVal;
	//			SAFEARRAY **pparray;
				VARIANT *pvarVal;
				PVOID byref;
				CHAR cVal;
				UINT16 uiVal;
				UINT32 ulVal;
				UINT64 ullVal;
				INT32 intVal;
				UINT32 uintVal;
	//			DECIMAL *pdecVal;
				CHAR *pcVal;
				UINT16 *puiVal;
				UINT32 *pulVal;
				UINT64 *pullVal;
				INT32 *pintVal;
				UINT32 *puintVal;
				struct //__tagBRECORD
				{
					PVOID pvRecord;
					IRecordInfo *pRecInfo;
				};
			};
		};
	//	DECIMAL decVal;
	};
};

enum VARENUM
{
	VT_EMPTY = 0,
	VT_NULL = 1,
	VT_I2 = 2,
	VT_I4 = 3,
	VT_R4 = 4,
	VT_R8 = 5,
	VT_CY = 6,
	VT_DATE = 7,
	VT_BSTR = 8,
	VT_DISPATCH = 9,
	VT_ERROR = 10,
	VT_BOOL = 11,
	VT_VARIANT = 12,
	VT_UNKNOWN = 13,
	VT_DECIMAL = 14,
	VT_I1 = 16,
	VT_UI1 = 17,
	VT_UI2 = 18,
	VT_UI4 = 19,
	VT_I8 = 20,
	VT_UI8 = 21,
	VT_INT = 22,
	VT_UINT = 23,
	VT_VOID = 24,
	VT_HRESULT	= 25,
	VT_PTR = 26,
	VT_SAFEARRAY = 27,
	VT_CARRAY = 28,
	VT_USERDEFINED = 29,
	VT_LPSTR = 30,
	VT_LPWSTR = 31,
	VT_FILETIME = 64,
	VT_BLOB = 65,
	VT_STREAM = 66,
	VT_STORAGE = 67,
	VT_STREAMED_OBJECT = 68,
	VT_STORED_OBJECT = 69,
	VT_BLOB_OBJECT = 70,
	VT_CF = 71,
	VT_CLSID = 72,
	VT_VECTOR = 0x1000,
	VT_ARRAY = 0x2000,
	VT_BYREF = 0x4000,
	VT_RESERVED = 0x8000,
	VT_ILLEGAL = 0xffff,
	VT_ILLEGALMASKED = 0xfff,
	VT_TYPEMASK = 0xfff
};

HRESULT VariantClear(VARIANT*);
HRESULT VariantCopy(VARIANT* to, const VARIANT* from);

struct STGMEDIUM
{
    DWORD tymed;
    union 
	{
        HBITMAP hBitmap;
        HMETAFILEPICT hMetaFilePict;
        HENHMETAFILE hEnhMetaFile;
        HGLOBAL hGlobal;
        LPOLESTR lpszFileName;
        IStream* pstm;
        IStorage* pstg;
	};
    IUnknown *pUnkForRelease;
};

HRESULT GetActiveObject(REFCLSID rclsid, void* pvReserved, IUnknown** ppunk);

#endif // COMInterfacesMore_h

