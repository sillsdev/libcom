/*----------------------------------------------------------------------------
Copyright 2002, SIL International. All rights reserved.

File: TestCOMSupportLib.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Header for test program for COMSupportLib.

Added ComponentLoaded()
	2003-05-15, GDLC
Added prototype for trace()
	2003-05-08, GDLC
Modified for sample code from Chapter 7 of Rogerson's book.
	2003-04-30, GDLC
Moved component definition to separate file.
	2003-04-21, GDLC
Modified interface definitions for component CA with interfaces IX, IY, IZ (Rogerson Chap 4).
	2003-04-17, GDLC
Added interface definitions for component CA with interfaces IX, IY, IZ (Rogerson Chap 3).
	2003-04-16, GDLC
Added default cout stream output parameters.
	2003-04-14, GDLC
Begun.
	2003-03-31, GDLC
----------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>

void TestCLSIDFromString(void);

void TestStringFromCLSID(void);

extern void ComponentLoaded(void);

void trace(const char* msg);

void HexGUIDOut(const GUID &gd, std::ostream& stm = std::cout);

void HexDWORDOut(const DWORD &dw, std::ostream& stm = std::cout);

void HexByteOut(UInt8 b, std::ostream& stm = std::cout);
