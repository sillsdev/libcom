/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: COMSupportInternals.h
Responsibility: Graeme Costin
Last reviewed: never

	Internal details of support for COM in FieldWorks on MacOS.

Moved internal implementation definitions to here from COMSupport.h
	2001-07-19, GDLC
Added GetMutableInstance() for private use
	2001-08-07, GDLC
----------------------------------------------------------------------------------------------*/

//--------------------------------------------------------------------------------------------
//	ComRegistry class
//
//	This class implements a non-persistent registry of COM classes which is built as an
//	application using it loads itself and the shared libraries it uses. Since this
//	registry is re-built every time an application launches it is always up-to-date
//	with the latest versions of the shared libraries.
//
//	The single instance of the COM registry is constructed in response to static items
//	in the shared libraries.???CHECK THIS COMMENT!

#include <map>

typedef std::map<CLSID, LPCLASSFACTORY>	ComMap;

class ComRegistry : public ComMap
{
	public:
	
		ComRegistry();
	
		~ComRegistry();

	private:

		static ComRegistry* GetMutableInstance();

		static void PtrToHex(LPCLASSFACTORY Ptr, char *buf);
		
	public:

		static ComRegistry* GetInstance() { return GetMutableInstance(); }
	
		void Register(const CLSID &Class, LPCLASSFACTORY Pointer);

		HRESULT GetFactoryPtr(const CLSID& Class, LPCLASSFACTORY* pIFactory) const;

		void Dump(std::ostream& out);
};
