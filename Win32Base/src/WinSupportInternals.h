#error using WinSupportInternals.h ?
/*----------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: WinSupportInternals.h
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

Header file to provide implementation details of substitutions and kludges
when compiling FieldWorks for MacOS. Some of the implementation is provided
in WinSupport.cpp.

Added DirectoryIDFromName and pstricmp.
	2002-11-07, GDLC
Added GetBundleInfo()
	2002-08-21, GDLC
Deleted struct AppOrLibDetails (no longer needed because of using CFBundle APIs)
	2002-08-09, GDLC
Mutated ResFileDetails into AppOrLibDetails
	2002-07-15, GDLC
Added UTCMacToWin().
	2002-04-30, GDLC
Written by copying from the WindowsInternals.h file used in the port of
CodeGen. So far none of these items from the CodeGen port are needed in
Mac FieldWorks. The resource kludges may be needed, but the scanning of
files in a directory probably won't be needed.
	2001-07-19, GDLC
----------------------------------------------------------------------------*/

#if !__MACH__
#include <CFBundle.h>
#endif

#include <LString.h>

//	Function to convert from MacOS UTC to WinOS UTC
void UTCMacToWin(const UTCDateTime &tMac, LPFILETIME tWin);

//	Structure used to keep information about the application bundle.
struct BUNDLE_INFO
{
	FSRef		appFSRef;
	FSSpec		appFSSpec;
	LStr255		appPathName;
	FSRef		resFSRef;
	FSSpec		resFSSpec;
	LStr255		resPathName;
};
typedef BUNDLE_INFO *BUNDLE_PTR;

// Get Bundle Information
bool GetBundleInfo(void);

#define PATH_SEPARATOR ':'

// Find pathName from dirID
int	PathNameFromDirID(	short	vRef,
						long	dirID,
						Str255	pathName);

// Find dirID from name
SInt32 DirectoryIDFromName(	short vRef,
							long parDirID,
							StringPtr dName);

// Wildcard matching function
bool wildmatch(StringPtr fName, StringPtr wCard);

//#include "PascalString.h"

typedef struct
{
	short	volRefNum;
	long	dirID;
	Str31	pattern;
	short	index;
	bool	wildcard;
} MAC_FIND_FILE_PARAMS;

// Pascal to C string limited copy
void p2cstrncpy(const unsigned char* p, char* q, DWORD n);

// Pascal string case independent comparison
int pstricmp(const unsigned char * p, const unsigned char * q);
