//	WinSupportTesting.h
//
//	Prefix file for testing Carbon target of WinSupport library
//
//	#define TESTING 1 added.
//		2002-06-21, GDLC
//	_MAC changed to MAC.
//		2002-03-21, GDLC
//	Written, Graeme Costin, 2001-08-21

#include <MacHeadersCarbon.h>

#define _DEBUG
#define _assert __assertion_failed

#define	TESTING 1

#define	MAC
