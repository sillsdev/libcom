

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Nov 06 14:11:08 2007
 */
/* Compiler settings for IBakery.idl:
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


#ifndef __IBakery_idl_h__
#define __IBakery_idl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBakery_FWD_DEFINED__
#define __IBakery_FWD_DEFINED__
typedef interface IBakery IBakery;
#endif 	/* __IBakery_FWD_DEFINED__ */


#ifndef __Bakery_FWD_DEFINED__
#define __Bakery_FWD_DEFINED__

#ifdef __cplusplus
typedef class Bakery Bakery;
#else
typedef struct Bakery Bakery;
#endif /* __cplusplus */

#endif 	/* __Bakery_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __IBakeryLib_LIBRARY_DEFINED__
#define __IBakeryLib_LIBRARY_DEFINED__

/* library IBakeryLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IBakeryLib;

#ifndef __IBakery_INTERFACE_DEFINED__
#define __IBakery_INTERFACE_DEFINED__

/* interface IBakery */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IBakery;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("cbdb8382-a142-489a-8831-2b60069d2841")
    IBakery : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getNumberCookiesBaked( 
            /* [retval][out] */ long *number) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE bakeCookies( 
            /* [in] */ long amount,
            /* [in] */ IUnknown *jar) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBakeryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBakery * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBakery * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBakery * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *getNumberCookiesBaked )( 
            IBakery * This,
            /* [retval][out] */ long *number);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *bakeCookies )( 
            IBakery * This,
            /* [in] */ long amount,
            /* [in] */ IUnknown *jar);
        
        END_INTERFACE
    } IBakeryVtbl;

    interface IBakery
    {
        CONST_VTBL struct IBakeryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBakery_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBakery_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IBakery_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IBakery_getNumberCookiesBaked(This,number)	\
    (This)->lpVtbl -> getNumberCookiesBaked(This,number)

#define IBakery_bakeCookies(This,amount,jar)	\
    (This)->lpVtbl -> bakeCookies(This,amount,jar)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBakery_getNumberCookiesBaked_Proxy( 
    IBakery * This,
    /* [retval][out] */ long *number);


void __RPC_STUB IBakery_getNumberCookiesBaked_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBakery_bakeCookies_Proxy( 
    IBakery * This,
    /* [in] */ long amount,
    /* [in] */ IUnknown *jar);


void __RPC_STUB IBakery_bakeCookies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IBakery_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Bakery;

#ifdef __cplusplus

class DECLSPEC_UUID("01fe1db4-5cfc-482b-9aae-266d94f5ece8")
Bakery;
#endif
#endif /* __IBakeryLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


