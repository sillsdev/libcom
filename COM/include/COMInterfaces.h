/*
 *	$Id$
 *
 *	Standard COM interfaces
 *
 *	Neil Mayhew - 2005-04-22
 *
 * COM Support Library
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

#ifndef _COMINTERFACES_H_
#define _COMINTERFACES_H_

#include "COM.h"

class IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppv) = 0;
	virtual ULONG   STDMETHODCALLTYPE AddRef() = 0;
	virtual ULONG   STDMETHODCALLTYPE Release() = 0;
};

#define IID_IUnknown __uuidof(IUnknown)

typedef IUnknown* LPUNKNOWN;

class IClassFactory : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE LockServer(BOOL fLock) = 0;
	virtual HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *punkOuter, REFIID iid, void** ppv) = 0;
};

typedef IClassFactory* LPCLASSFACTORY;

class IDataObject : public IUnknown {};
class IDispatch : public IUnknown {};


class IRpcStubBuffer;	// Forward declaration
class IRpcChannelBuffer;	// Forward declaration
class IEnumSTATDATA;		// Forward declaration
class IAdviseSink;		// Forward declaration
class IRecordInfo;		// Forward declaration
class IStream;			// Forward declaration
struct STATSTG;			// Forward declaration


class IEnumFORMATETC : public IUnknown {};
class IShellFolder : public IUnknown {};

typedef void*** SNB; // SNB is a pointer to an array of pointers to strings

class IEnumSTATSTG
{
public:
	virtual HRESULT STDMETHODCALLTYPE Clone(IEnumSTATSTG **ppenum);

	virtual HRESULT STDMETHODCALLTYPE Next(
		ULONG celt,
		STATSTG *rgelt,
		ULONG *pceltFetched);

	virtual HRESULT STDMETHODCALLTYPE Reset();

	virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
};

typedef IEnumSTATSTG* IEnumSTATSTGPtr;

// declaration of IStorage interface.
class IStorage : public IUnknown 
{
public:
	virtual HRESULT STDMETHODCALLTYPE CreateStream(
		const OLECHAR *pwcsName, 
		DWORD grfMode, 
		DWORD res1, 
		DWORD res2, 
		IStream **ppstm) = 0;

	virtual HRESULT STDMETHODCALLTYPE OpenStream(
		const OLECHAR *pwcsName, 
		void *res1, 
		DWORD grfMode, 
		DWORD res2, 
		IStream **ppstm) = 0;

	virtual HRESULT STDMETHODCALLTYPE CreateStorage(
		const OLECHAR *pwcsName, 
		DWORD grfMode, 
		DWORD res1, 
		DWORD res2, 
		IStorage **ppstg) = 0;

	virtual HRESULT STDMETHODCALLTYPE OpenStorage(
		const OLECHAR *pwcsName, 
		IStorage *pstgPriority, 
		DWORD grfMode, 
		SNB snbExclude, 
		DWORD reserved, 
		IStorage **ppstg) = 0;

	virtual HRESULT STDMETHODCALLTYPE CopyTo(
		DWORD ciid,
		const IID *rgiid,
		SNB snb,
		IStorage *dest) = 0;

	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(
		const OLECHAR *pwcsName,
		IStorage *pstgDest,
		const OLECHAR *pwcsNewName,
		DWORD grfFlags) = 0;

	virtual HRESULT STDMETHODCALLTYPE Commit(
		DWORD grfCommitFlags) = 0;

	virtual HRESULT STDMETHODCALLTYPE Revert() = 0;

	virtual HRESULT STDMETHODCALLTYPE EnumElements(
		DWORD res1,
		void *res2,
		DWORD res3,
		IEnumSTATSTG **ppenum) = 0;

	virtual HRESULT STDMETHODCALLTYPE DestroyElement(
		const OLECHAR *pwcsName) = 0;

	virtual HRESULT STDMETHODCALLTYPE RenameElement(
		const OLECHAR *old) = 0;
	
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(
		const OLECHAR name,
		const void /*FILETIME*/ *created,
		const void /*FILETIME*/ *accessed,
		const void /*FILETIME*/ *modified) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetClass(
		REFCLSID clsid) = 0;

	virtual HRESULT STDMETHODCALLTYPE SetStateBits(
		DWORD grfStateBits,
		DWORD grfMask) = 0;

	virtual HRESULT STDMETHODCALLTYPE Stat(
		STATSTG *pstatstg,
		DWORD grfStatFlag) = 0;
};

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

#endif //_COMCLASSES_H_
