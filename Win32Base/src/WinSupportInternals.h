/*----------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: WinSupportInternals.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

Header file to provide implementation details of substitutions and kludges
when compiling FieldWorks for MacOS. Some of the implementation is provided
in WinSupport.cpp.

Added UTCMacToWin().
	2002-04-30, GDLC
Written by copying from the WindowsInternals.h file used in the port of
CodeGen. So far none of these items from the CodeGen port are needed in
Mac FieldWorks. The resource kludges may be needed, but the scanning of
files in a directory probably won't be needed.
	2001-07-19, GDLC
----------------------------------------------------------------------------*/

//	Function to convert from MacOS UTC to WinOS UTC
void UTCMacToWin(const UTCDateTime &tMac, LPFILETIME tWin);

//	struct used for obtaining application file
struct ResFileDetails
{
	short	refNum;
	FSSpec	spec;
	
	ResFileDetails();
};

#define PATH_SEPARATOR ':'


bool wildmatch(StringPtr fName, StringPtr wCard);

//#include "PascalString.h"

typedef struct _MAC_FIND_FILE_PARAMS
{
	short	volRefNum;
	long	dirID;
	Str31	pattern;
	short	index;
} MAC_FIND_FILE_PARAMS;
