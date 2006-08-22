/*
 *	$Id$
 *
 *	Smart pointers for standard COM classes
 *
 *	Neil Mayhew - 22 Aug 2006
 */

#ifndef _COMPOINTERS_H_
#define _COMPOINTERS_H_

#include "COMClasses.h"

#ifdef _COM_SMARTPTR_TYPEDEF	// Smart-pointer classes must define this
	#define _DEFINE_PTR(I) _COM_SMARTPTR_TYPEDEF(I, IID_##I)
#else
	#define _DEFINE_PTR(I) typedef I* I##Ptr
#endif

_DEFINE_PTR(IUnknown);

_DEFINE_PTR(IClassFactory);
_DEFINE_PTR(IDataObject);
_DEFINE_PTR(IDispatch);
_DEFINE_PTR(IStorage);

_DEFINE_PTR(IRpcStubBuffer);
_DEFINE_PTR(IRpcChannelBuffer);
_DEFINE_PTR(IEnumSTATDATA);
_DEFINE_PTR(IAdviseSink);
_DEFINE_PTR(IRecordInfo);

_DEFINE_PTR(IEnumFORMATETC);
_DEFINE_PTR(IShellFolder);

_DEFINE_PTR(IErrorInfo);
_DEFINE_PTR(ICreateErrorInfo);
_DEFINE_PTR(ISupportErrorInfo);
_DEFINE_PTR(ISequentialStream);
_DEFINE_PTR(IStream);
_DEFINE_PTR(IPicture);
_DEFINE_PTR(IAccessible);
_DEFINE_PTR(IServiceProvider);
_DEFINE_PTR(IEnumVARIANT);
_DEFINE_PTR(ITypeInfo);

#endif //_COMPOINTERS_H_
