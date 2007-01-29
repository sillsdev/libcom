/*
 *	$Id$
 *
 *	Standard COM classes
 *
 *	Neil Mayhew - Apr 22, 2005
 */

#ifndef _COMCLASSES_H_
#define _COMCLASSES_H_

#include "COM.h"
#include "DateTime.h"

class IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppv) = 0;
	virtual ULONG   STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG   STDMETHODCALLTYPE Release() = 0;
};

#define IID_IUnknown __uuidof(IUnknown)

class IClassFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *punkOuter, REFIID iid, void** ppv) = 0;
};

class IDataObject : public IUnknown {};
class IDispatch : public IUnknown {};
class IStorage : public IUnknown {};

class IRpcStubBuffer;	// Forward declaration
class IRpcChannelBuffer;	// Forward declaration
class IEnumSTATDATA;		// Forward declaration
class IAdviseSink;		// Forward declaration
class IRecordInfo;		// Forward declaration

class IEnumFORMATETC : public IUnknown {};
class IShellFolder : public IUnknown {};

#define IID_IClassFactory __uuidof(IClassFactory)
#define IID_IDataObject __uuidof(IDataObject)
#define IID_IDispatch __uuidof(IDispatch)
#define IID_IStorage __uuidof(IStorage)

#define IID_IRpcStubBuffer __uuidof(IRpcStubBuffer)
#define IID_IRpcChannelBuffer __uuidof(IRpcChannelBuffer)
#define IID_IEnumSTATDATA __uuidof(IEnumSTATDATA)
#define IID_IAdviseSink __uuidof(IAdviseSink)
#define IID_IRecordInfo __uuidof(IRecordInfo)

#define IID_IEnumFORMATETC __uuidof(IEnumFORMATETC)
#define IID_IShellFolder __uuidof(IShellFolder)

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

STDAPI SetErrorInfo(ULONG dwReserved, IErrorInfo* perrinfo);
STDAPI GetErrorInfo(ULONG dwReserved, IErrorInfo** pperrinfo);
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
	STGM_READ             = 0x01,
	STGM_WRITE            = 0x02,
	STGM_READWRITE        = STGM_READ | STGM_WRITE,
	STGM_CREATE           = 0x04,
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
		/* [in] */ ULONG cb,
		/* [out] */ ULONG *pcbRead) = 0;
	
	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
		/* [size_is][in] */ const void *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG *pcbWritten) = 0;
	
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

typedef UINT OLE_HANDLE;

typedef LONG OLE_XPOS_HIMETRIC;
typedef LONG OLE_YPOS_HIMETRIC;
typedef LONG OLE_XSIZE_HIMETRIC;
typedef LONG OLE_YSIZE_HIMETRIC;

class IPicture : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE get_Width( 
		/* [out] */ OLE_XSIZE_HIMETRIC *pWidth) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_Height( 
		/* [out] */ OLE_YSIZE_HIMETRIC *pHeight) = 0;
	
#if 0	// Leave out unneeded methods

	virtual HRESULT STDMETHODCALLTYPE get_Handle( 
		/* [out] */ OLE_HANDLE *pHandle) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_hPal( 
		/* [out] */ OLE_HANDLE *phPal) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_Type( 
		/* [out] */ SHORT *pType) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE Render( 
		/* [in] */ HDC hDC,
		/* [in] */ LONG x,
		/* [in] */ LONG y,
		/* [in] */ LONG cx,
		/* [in] */ LONG cy,
		/* [in] */ OLE_XPOS_HIMETRIC xSrc,
		/* [in] */ OLE_YPOS_HIMETRIC ySrc,
		/* [in] */ OLE_XSIZE_HIMETRIC cxSrc,
		/* [in] */ OLE_YSIZE_HIMETRIC cySrc,
		/* [in] */ LPCRECT pRcWBounds) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE set_hPal( 
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
		/* [out] */ LONG *pCbSize) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE get_Attributes( 
		/* [out] */ DWORD *pDwAttr) = 0;

#endif //0
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
enum { VARIANT_FALSE, VARIANT_TRUE };

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
				LONGLONG llVal;
				LONG lVal;
				BYTE bVal;
				SHORT iVal;
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
				SHORT *piVal;
				LONG *plVal;
				LONGLONG *pllVal;
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
				USHORT uiVal;
				ULONG ulVal;
				ULONGLONG ullVal;
				INT intVal;
				UINT uintVal;
	//			DECIMAL *pdecVal;
				CHAR *pcVal;
				USHORT *puiVal;
				ULONG *pulVal;
				ULONGLONG *pullVal;
				INT *pintVal;
				UINT *puintVal;
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

#endif //_COMCLASSES_H_
