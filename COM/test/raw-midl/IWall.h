

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Feb 03 15:16:54 2012
 */
/* Compiler settings for IWall.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __IWall_h__
#define __IWall_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWall_FWD_DEFINED__
#define __IWall_FWD_DEFINED__
typedef interface IWall IWall;
#endif 	/* __IWall_FWD_DEFINED__ */


#ifndef __Wall_FWD_DEFINED__
#define __Wall_FWD_DEFINED__

#ifdef __cplusplus
typedef class Wall Wall;
#else
typedef struct Wall Wall;
#endif /* __cplusplus */

#endif 	/* __Wall_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IWallLib_LIBRARY_DEFINED__
#define __IWallLib_LIBRARY_DEFINED__

/* library IWallLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IWallLib;

#ifndef __IWall_INTERFACE_DEFINED__
#define __IWall_INTERFACE_DEFINED__

/* interface IWall */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IWall;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("56df7174-bff7-4ae4-96f0-5ab3992769a1")
    IWall : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readWall( 
            /* [in] */ int maxLength,
            /* [size_is][retval][out] */ OLECHAR *text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeOnWall( 
            /* [in] */ int length,
            /* [size_is][in] */ OLECHAR *text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeOnWall2( 
            /* [in] */ int length,
            /* [size_is][in] */ const OLECHAR *text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeCharStarOnWall( 
            /* [in] */ unsigned char *text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeWcharStarOnWall( 
            /* [in] */ WCHAR *text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE writeBSTROnWall( 
            /* [in] */ BSTR text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE readWallBSTR( 
            /* [retval][out] */ BSTR *text) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWallVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWall * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWall * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWall * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *readWall )( 
            IWall * This,
            /* [in] */ int maxLength,
            /* [size_is][retval][out] */ OLECHAR *text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeOnWall )( 
            IWall * This,
            /* [in] */ int length,
            /* [size_is][in] */ OLECHAR *text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeOnWall2 )( 
            IWall * This,
            /* [in] */ int length,
            /* [size_is][in] */ const OLECHAR *text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeCharStarOnWall )( 
            IWall * This,
            /* [in] */ unsigned char *text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeWcharStarOnWall )( 
            IWall * This,
            /* [in] */ WCHAR *text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *writeBSTROnWall )( 
            IWall * This,
            /* [in] */ BSTR text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *readWallBSTR )( 
            IWall * This,
            /* [retval][out] */ BSTR *text);
        
        END_INTERFACE
    } IWallVtbl;

    interface IWall
    {
        CONST_VTBL struct IWallVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWall_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWall_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWall_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWall_readWall(This,maxLength,text)	\
    ( (This)->lpVtbl -> readWall(This,maxLength,text) ) 

#define IWall_writeOnWall(This,length,text)	\
    ( (This)->lpVtbl -> writeOnWall(This,length,text) ) 

#define IWall_writeOnWall2(This,length,text)	\
    ( (This)->lpVtbl -> writeOnWall2(This,length,text) ) 

#define IWall_writeCharStarOnWall(This,text)	\
    ( (This)->lpVtbl -> writeCharStarOnWall(This,text) ) 

#define IWall_writeWcharStarOnWall(This,text)	\
    ( (This)->lpVtbl -> writeWcharStarOnWall(This,text) ) 

#define IWall_writeBSTROnWall(This,text)	\
    ( (This)->lpVtbl -> writeBSTROnWall(This,text) ) 

#define IWall_readWallBSTR(This,text)	\
    ( (This)->lpVtbl -> readWallBSTR(This,text) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWall_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Wall;

#ifdef __cplusplus

class DECLSPEC_UUID("f350519c-2a5f-4c65-a13f-73679cbf171f")
Wall;
#endif
#endif /* __IWallLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


