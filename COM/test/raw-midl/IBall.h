

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Feb 03 15:16:38 2012
 */
/* Compiler settings for IBall_.idl:
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


#ifndef __IBall_h__
#define __IBall_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBall_FWD_DEFINED__
#define __IBall_FWD_DEFINED__
typedef interface IBall IBall;
#endif 	/* __IBall_FWD_DEFINED__ */


#ifndef __Ball_FWD_DEFINED__
#define __Ball_FWD_DEFINED__

#ifdef __cplusplus
typedef class Ball Ball;
#else
typedef struct Ball Ball;
#endif /* __cplusplus */

#endif 	/* __Ball_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IBallLib_LIBRARY_DEFINED__
#define __IBallLib_LIBRARY_DEFINED__

/* library IBallLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_IBallLib;

#ifndef __IBall_INTERFACE_DEFINED__
#define __IBall_INTERFACE_DEFINED__

/* interface IBall */
/* [helpstring][uuid][object] */ 


EXTERN_C const IID IID_IBall;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("304e42cb-e6d8-40c0-a09f-a362f12b428d")
    IBall : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE bounce( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE roll( 
            /* [in] */ long distance,
            /* [retval][out] */ long *ret_distance) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IBallVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBall * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBall * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBall * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *bounce )( 
            IBall * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *roll )( 
            IBall * This,
            /* [in] */ long distance,
            /* [retval][out] */ long *ret_distance);
        
        END_INTERFACE
    } IBallVtbl;

    interface IBall
    {
        CONST_VTBL struct IBallVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBall_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBall_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBall_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBall_bounce(This)	\
    ( (This)->lpVtbl -> bounce(This) ) 

#define IBall_roll(This,distance,ret_distance)	\
    ( (This)->lpVtbl -> roll(This,distance,ret_distance) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IBall_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Ball;

#ifdef __cplusplus

class DECLSPEC_UUID("261509ac-86a4-488e-af54-c81cac39bce6")
Ball;
#endif
#endif /* __IBallLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


