#error using WinSupport.cpp ?
/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: WinSupport.cpp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

	$Id$

C++ file to provide substitutions and kludges when compiling FieldWorks for MacOS.

Requires WinSupport.h and WinSupportInternals.h.

Changed wildcard match function to ignore case. Added GetLastError and modified APIs
that can set error codes. Fixed bug in wildcard match function for question mark.
	2002-11-08, GDLC
Added FindNextFile and modified FindFirstFile to handle empty pattern
	2002-11-01, GDLC
Added conditional for Mach-O compilation.
	2002-09-19, GDLC
Added p2cstrncpy() and used it in two places.
	2002-09-03, GDLC
Added use of BUNDLE_INFO structure.
	2002-08-27, GDLC
Moved #pragma export directives from WinSupport.h.  Added .app folder name to the
pathname returned by GetModuleFileName(). Added GetBundleInfo().
	2002-08-21, GDLC
Deleted struct AppOrLibDetails (no longer needed because of using CFBundle APIs);
added PathNameFromDirID() and used it in GetModuleFileName()
	2002-08-09, GDLC
Mutated ResFileDetails into AppOrLibDetails
	2002-07-15, GDLC
Commented out FormatMessage()
	2002-06-25, GDLC
Improved implementation of GetTickCount()
	2002-06-25, GDLC
Used conditional compilation to define timeUTC and machLoc as a public globals in the
Testing target (in debug and release targets they are on local stack).
	2002-06-21, GDLC
Made timeUTC a global static variable for testing purposes.
	2002-06-14, GDLC
Fixed GetSystemTimeAsFileTime().
	2002-04-30, GDLC
Added GetTimeZoneInformation().
	2002-04-26, GDLC
Added GetSystemTimeAsFileTime().
	2002-04-23, GDLC
Removed use of FieldWorks debug.h
	2002-03-30, GDLC
Reordered some functions.
	2002-03-27, GDLC
More parts of the CodeGen WinSupport copied into this file.
	2002-01-16, GDLC
Written by copying parts of the Windows.cp written for CodeGen.
	2001-07-??, GDLC
----------------------------------------------------------------------------------------------*/

#include "WinSupport.h"

#include <String.h>
#include <assert.h>
#include <cstdlib>
#include <cctype>
#include <UTCUtils.h>
#include <CarbonEvents.h>

#if !__MACH__
#include <CFBundle.h>
#endif

#include <LString.h>

#include "WinSupportInternals.h"

//	LockResource(), LoadResource(), SizeofResource() and FindClose()
//	are defined later in this file.

// ---------------------------------------------------------------------------
//		¥ GetLastError
// ---------------------------------------------------------------------------
//	Returns the error code set by whatever was the last Windows API to set an
//	error code.

static DWORD	LastErrorCode;

#pragma export on
DWORD GetLastError(VOID)
{
	return LastErrorCode;
}
#pragma export off

// ---------------------------------------------------------------------------
//		¥ GetSystemTimeAsFileTime
// ---------------------------------------------------------------------------
//	Returns the system time as a 64-bit signed integer being the number of
//	100-nanosecond periods since 12:00AM on January 1, 1601.
//
#if TESTING
//	To facilitate testing of GetSystemTimeAsFileTime(), the UTC obtained from MacOS is
//	put in this globally accessible variable so that the test program can output it as
//	well as the value converted to a Windows FILETIME structure.
#pragma export on
UTCDateTime		timeUTC;
#pragma export off
#endif

#pragma export on
void GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{

#if	!TESTING
	UTCDateTime		timeUTC;
#endif

	//	Get no. of seconds (top 48 bits) since midnight January 1, 1904
	//	This is unsigned.
	::GetUTCDateTime(&timeUTC, kUTCDefaultOptions);

	UTCMacToWin(timeUTC, lpSystemTimeAsFileTime);
}
#pragma export off

void UTCMacToWin(const UTCDateTime &tMac, LPFILETIME tWin)
{
	union {
		UTCDateTime		time;
		UInt64			time64;
	};
	UInt64				secs;
	UInt64				fractSecs;
	UInt64				fractNano;

	time = tMac;

	//	Convert to no. of 100-nanosecond periods since midnight January 1, 1904
	//	treating the whole seconds and the fractional seconds separately to reduce
	//	risk of overflow.
	secs = (time64 & 0xFFFFFFFFFFFF0000) >> 16;
	secs = secs * 10000000;
	fractSecs = static_cast <unsigned long> (time64 & 0xFFFF);
	fractNano = (fractSecs * 10000000) >> 16;
	time64 = secs + fractNano;

	//	Add no. of 100-nanosecond periods between 12:00AM January 1, 1601
	//	and midnight January 1, 1904.
	//	Why did Microsoft and Apple have to be SO VERY different?!
	time64 = time64 + (9561628800 * 10000000);

	//	Send the result to the FILETIME structure
	tWin->dwHighDateTime = static_cast <unsigned long> ((time64 & 0xFFFFFFFF00000000) >> 32);
	tWin->dwLowDateTime = static_cast <unsigned long> (time64 & 0xFFFFFFFF);
}

// ---------------------------------------------------------------------------
//		¥ GetTimeZoneInformation
// ---------------------------------------------------------------------------
//	Returns the information related to the local time zone.
//
//	The Mac system does not hold information about when daylight saving starts and
//	stops in the time zone and so this implementation cannot return that part of
//	the Windows time zone information record.
//
//	TODO: Find out whether WinFieldWorks needs the daylight saving information.

#if TESTING
//	To facilitate testing of GetTimeZoneInformation(), the MachineLocation obtained
//	from MacOS is put in this globally accessible variable so that the test program
//	can output it as well as the value converted to a Windows TIME_ZONE_INFORMATION
//	structure.
#pragma export on
MachineLocation		machLoc;
#pragma export off
#endif

#pragma export on
DWORD GetTimeZoneInformation( LPTIME_ZONE_INFORMATION lpTimeZoneInformation)
{

#if	!TESTING
	MachineLocation		machLoc;
#endif

	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	ReadLocation(&machLoc);
	TIME_ZONE_INFORMATION	tZone;
	//	Most fields of the Windows time zone information cannot be filled from
	//	values read from the MacOS MachineLocation, so we set the lot to zero
	//	before setting the one item we can deal with.
	memset(&tZone, 0, sizeof tZone);

	//	gmtDelta
	//	The signed integer is contained in the lower 3 bytes of the long.
	//	The sign needs to be extended through the upper byte; this is done
	//	by shifting left and then shifting right since arithmetic shift right
	//	extends the sign bit.
	//
	//	The MacOS gmtDelta is in seconds; the Windows bias is in minutes.
	//
	//	The MacOS gmtDelta is added to GMT to get local time; the Windows
	//	Bias is added to local time to get UTC - hence the sign change.
	tZone.Bias = - ((((machLoc.u.gmtDelta & 0xFFFFFF) << 8) >>8) / 60);

	*lpTimeZoneInformation = tZone;

	//	This is the return value for no daylight saving on WinNT, 2000, XP.
	//	We don't have the information to deal with daylight saving items.
	return TIME_ZONE_ID_UNKNOWN;
}
#pragma export off

// ---------------------------------------------------------------------------
//		¥ GetTickCount
// ---------------------------------------------------------------------------
//	This function returns the number of milliseconds since the system was booted.
//	Windows expects the number of milliseconds in a 32-bit unsigned integer; this
//	cannot hold a tick count greater than 49.7 days approx. When the tick count
//	exceeds this amount Windows wraps it around to zero.

#if TESTING
//	To facilitate testing of GetTickCount(), the EventTime obtained from MacOS is
//	put in this globally accessible variable so that the test program can output
//	it as well as the value converted to a Windows DWORD.
#pragma export on
EventTime		secsFP;
#pragma export off
#endif

#pragma export on
DWORD GetTickCount(void)
{

#if	!TESTING
	EventTime		secsFP;
#endif

	EventTime		tc;
	secsFP = ::GetCurrentEventTime();
	tc = secsFP * 1000;
	if (tc < 0x100000000) return static_cast <DWORD> (tc);
	else return 0;
}
#pragma export off

//	FormatMessage() is currently used only in Debug.cpp of FieldWorks, and this
//	file is so system-dependent that it is being re-written for MacFieldWorks.
//	Thus we can use MacOS debug output facilities instead and do not need to
//	implement FormatMessage().
//#pragma export on
//DWORD FormatMessage(
//	/*IN*/ DWORD	dwFlags,
//	/*IN*/ LPCVOID	lpSource,
//	/*IN*/ DWORD	dwMessageId,
//	/*IN*/ DWORD	dwLanguageId,
//	/*OUT*/ LPSTR	lpBuffer,
//	/*IN*/ DWORD	nSize,
//	/*IN*/ va_list	*Arguments
//	);
//#pragma export off

// ---------------------------------------------------------------------------
//		¥ GetModuleFileName
// ---------------------------------------------------------------------------
//	This function returns the full pathname of the module's bundle.
//	If hModule is NULL the path for the application's bundle is returned;
//	otherwise the pathname for the bundle hModule is returned.
//
//	It appears that WinFieldWorks uses this Windows API merely to get the name of
//	the module to use in error reports. Thus it needs the name of the application
//	or other module as the user will see it.
//
//	TODO: Check whether WinFieldWorks or any other Windows app that we port with
//	WinSupportLib has other reasons for calling GetModuleFileName().
//
//	TODO: This function can be extended if it is ever necessary.

static	bool		haveBundleInfo;
static	BUNDLE_PTR	bundleInfoPtr;

#pragma export on
DWORD	GetModuleFileName(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize)
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	//TODO: Remove this assert when we have implemented the case of non-NULL hModule
	//	So far there is no reason to implement it. GDLC 2002-08-27
	assert(hModule == NULL);

	if (hModule == NULL) {
		// Get details of application's bundle if not already obtained
		if (!GetBundleInfo())
		{
			LastErrorCode = ERROR_MOD_NOT_FOUND;
			return 0;
		}

		p2cstrncpy(bundleInfoPtr->appPathName, lpFilename, nSize);
		return strlen(lpFilename);
	} else {
		// Get details of the specified module (Application or Framework) bundle
		//TODO: Implement this when we have an app that actually has a FrameWork
		LastErrorCode = ERROR_CALL_NOT_IMPLEMENTED;
		return 0;
	}
}
#pragma export off

// If the bundle info has not yet been obtained, gets it using the CFBundle APIs.
bool	GetBundleInfo(void)
{
	if (!haveBundleInfo)
	{
		bundleInfoPtr = new BUNDLE_INFO;

		//Get info about application bundle
		CFBundleRef	appBundle = CFBundleGetMainBundle();
		if (appBundle == NULL) goto CannotFindData;
		CFURLRef appBundleURL = CFBundleCopyBundleURL(appBundle);
		if (appBundleURL == NULL) goto CannotFindData;

		CFURLRef appBundleAbsURL = CFURLCopyAbsoluteURL(appBundleURL);
		CFRelease(appBundleURL);

		bool gotAppFSRef = CFURLGetFSRef( appBundleAbsURL, &bundleInfoPtr->appFSRef);
		CFRelease(appBundleAbsURL);
		if (!gotAppFSRef) goto CannotFindData;

		FSGetCatalogInfo(&bundleInfoPtr->appFSRef, kFSCatInfoNone, NULL, NULL, &bundleInfoPtr->appFSSpec, NULL);

		// Extract pathname of application folder
		if (!PathNameFromDirID(	bundleInfoPtr->appFSSpec.vRefNum,
								bundleInfoPtr->appFSSpec.parID,
								bundleInfoPtr->appPathName)) goto CannotFindData;
		if ((bundleInfoPtr->appPathName[0] + bundleInfoPtr->appFSSpec.name[0]) > 255) goto CannotFindData;
		bundleInfoPtr->appPathName = bundleInfoPtr->appPathName + bundleInfoPtr->appFSSpec.name;

		// Get info about resource file or Resources folder, as appropriate
		CFURLRef resBundleURL = CFBundleCopyResourcesDirectoryURL(appBundle);
		if (resBundleURL == NULL) goto CannotFindData;
		CFURLRef resBundleAbsURL = CFURLCopyAbsoluteURL(resBundleURL);
		CFRelease(resBundleURL);
		//	The following call of CFURLGetFSREF willnot succeed if the Resources folder does not exist
		//TODO: What should be done if it doesn't exist? Extract the app's resource fork refNum?
		bool gotResFSRef = CFURLGetFSRef( resBundleAbsURL, &bundleInfoPtr->resFSRef);
		CFRelease(resBundleAbsURL);
		if (gotResFSRef) {
			FSGetCatalogInfo(&bundleInfoPtr->resFSRef, kFSCatInfoNone, NULL, NULL, &bundleInfoPtr->resFSSpec, NULL);
			// Extract pathname of Resources folder
			if (!PathNameFromDirID(	bundleInfoPtr->resFSSpec.vRefNum,
								bundleInfoPtr->resFSSpec.parID,
								bundleInfoPtr->resPathName)) goto CannotFindData;
			if ((bundleInfoPtr->resPathName[0] + bundleInfoPtr->resFSSpec.name[0]) > 255) goto CannotFindData;
			bundleInfoPtr->resPathName = bundleInfoPtr->resPathName + bundleInfoPtr->resFSSpec.name;
		}

		haveBundleInfo = true;
	}
	return true;

// Error exit when Bundle info cannot be obtained
CannotFindData:
	LastErrorCode = ERROR_MOD_NOT_FOUND;
	return false;
}

// Find pathName from dirID. Puts the path as a Pascal string into pathName
// and returns length of path, unless truncation of path has occurred in which case
// zero is returned.
int		PathNameFromDirID(	short	vRef,
							long	dirID,
							Str255	pathName)
{
	CInfoPBRec		pb;

	LStr255		path("\p");	// Initialize full pathname
	LStr255		dirName;		// Directory name
	OSErr		err;			// error code

	pb.dirInfo.ioNamePtr = &dirName[0];
    pb.dirInfo.ioVRefNum = vRef;	// Indicate target volume
    pb.dirInfo.ioDrParID = dirID;	// Initialize parent directory ID
    pb.dirInfo.ioFDirIndex = -1;	// Indicate info about a directory

    // Get name of each parent directory, up to root directory.
	do {
		pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
		err = PBGetCatInfoSync(&pb);

		dirName = dirName + "\p:";
		if ((path[0] + dirName[0]) > 255) return 0;
		path = dirName + path;
	} while (pb.dirInfo.ioDrDirID != fsRtDirID);

	// Return full pathname
	BlockMoveData(path, pathName, static_cast <long> (path[0]+1));
	return path[0];
}

// ---------------------------------------------------------------------------
//		¥ GetModuleHandle
// ---------------------------------------------------------------------------
//	This function returns a pointer to the BUNDLE_INFO structure of the
//	application's main bundle.
//
//	If it doesn't succeed it returns NULL.
//
//	It appears that WinFieldWorks uses this Windows API only to get a handle to
//	the module to use later in obtaining resources for the module.
//	On MacOS X, 9 and 8 this can be a CFURLRef to the application's bundle.
//
//	This initial implementation handles only the case of lpModuleName==NULL
//
//	TODO: Extend this implementation as soon as there is a need for handling
//	a non-NULL lpModuleName.
//
//	TODO: Check whether WinFieldWorks or any other Windows app that we port
//	with WinSupportLib has other reasons for calling GetModuleFileName().
//
//	TODO: Be ready to implement GetLastError() if a ported Windows app is
//	found that uses it!
#pragma export on
HMODULE GetModuleHandle(LPCSTR lpModuleName)
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	assert(lpModuleName == NULL);

	// Get details of application's bundle if not already obtained
	if (!GetBundleInfo())
	{
		LastErrorCode = ERROR_MOD_NOT_FOUND;
		return NULL;
	}

	return bundleInfoPtr;
}
#pragma export off

#ifdef	_DEBUG
// ---------------------------------------------------------------------------
//		¥ OutputDebugStringMac
// ---------------------------------------------------------------------------
//	This implementation for the Windows API OutputDebugString() is not used in
//	the Final target, only in the Debug and Test targets, in which it sends
//	the string to the debugger - if CodeWarrior is set to log debug strings it
//	will display this string in the Log window, otherwise it will send the
//	string to the low level debugger (eg MacsBug).
//	WARNING: If you don't have CodeWarrior logging these strings and also don't
//	have a low level debugger installed your system will crash!
#pragma export on
VOID OutputDebugStringMac(LPCSTR lpOutputString)
{
	LStr255		strg(lpOutputString);
	::DebugStr(strg);
}
#pragma export off
#endif

// ---------------------------------------------------------------------------
//		¥ FindResource
// ---------------------------------------------------------------------------
//	TODO: Extend this for other types of resources if necessary.
#pragma export on
HRSRC	FindResource(void*, int rid, const char* type)
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	if (std::strcmp( type, "TEMPLATE") == 0)
	{
		HRSRC	hrTmpl;

		if (!(hrTmpl = GetResource('TEXT', static_cast <SInt16> (rid)))) goto ResourceNotFound;
		return hrTmpl;
	}
	if (std::strcmp( type, "TEXT") == 0)
	{
		HRSRC	hrTmpl;

		if (!(hrTmpl = GetResource('TEXT', static_cast <SInt16> (rid)))) goto ResourceNotFound;
		return hrTmpl;
	}
	return NULL;	// Other resource types not yet kludged!

ResourceNotFound:
	//	What error code does Windows use for this situation?
	//	Can't find a suitable one in WINERROR.H!
	LastErrorCode = ERROR_FILE_NOT_FOUND;
	return NULL;
}
#pragma export off

// ---------------------------------------------------------------------------
//		¥ FindFirstFile
//
//	Finds the first file that matches the file pathname pattern supplied. The path
//	portion of the pattern indicates which drive and directory to start searching.
//	The file name portion of the pattern may contain wildcards (* and ?). The file
//	name portion may be empty, in which case the first file is obtained (an empty
//	file name portion functions as *).
//
//	Upon success, FindFirstFile returns a handle to some internal search information.
//	Subsequent calls to FindNextFile with this handle as parameter will find the
//	remaining files that match the pattern.
//
//	When no more files are to be searched for, the application should call FindClose
//	with the search handle as parameter so that the search information can be
//	disposed of.
// ---------------------------------------------------------------------------
#pragma export on
HANDLE	FindFirstFile(
    LPCSTR lpFileName,
    LPWIN32_FIND_DATA lpFindFileData
    )
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	// Create MAC_FIND_FILE_PARAMS structure
	MAC_FIND_FILE_PARAMS	**hProgress =
					(MAC_FIND_FILE_PARAMS**)NewHandle(sizeof(MAC_FIND_FILE_PARAMS));

	// Length of supplied pathname including file name
	int				n = static_cast <int> (strlen(lpFileName));

	unsigned char	pathName[MAX_PATH];		// Buffer for Pascal string pathname
//	PascalString<255>	pathName(lpFileName);		// Buffer for Pascal string pathname
	bool			wildcard = false;		// True if pattern is a wildcard
	OSErr			err;					// Mac system error return
	unsigned char	*p;						// Pointers for C->Pascal string copying
	const char		*q;

	// Is there a wildcard for the search? Look only after the last path separator.
	const char* split = strrchr(lpFileName, PATH_SEPARATOR);
	if (!split) split = lpFileName;

	// Test last part for explicit wildcard
	const char * ast = strchr(split, '*');	// Point to asterisk if any
	if ((ast || strchr(split, '?')) &&		// Must have an '*' or a '?'
		(ast ? (strrchr(split, '*') == ast) : true)	// Not more than one '*'
		)
		wildcard = true;

	// Get pathname into Pascal string buffer
	p = pathName;
	int	nPath = static_cast <int> (min((split - lpFileName), (sizeof(pathName) - 1)));
	*p++ = static_cast <unsigned char> (nPath);
	q = lpFileName;
	BlockMoveData(q, p, n);

	// Find volRefNum and dirID of parent directory
	FSSpec			f;

	err = FSMakeFSSpec(0, 0, pathName, &f);
	if (err == fnfErr)
	{
		// Need to check whether we acually have the volume name
		XVolumeParam		v;
		Str31				vName;
		v.ioVolIndex = 0;
		v.ioVRefNum = f.vRefNum;
		v.ioNamePtr = vName;
		if ((PBXGetVolInfoSync(&v) != noErr) || pstricmp(vName, f.name))
		{
			LastErrorCode = ERROR_BAD_PATHNAME;
			return INVALID_HANDLE_VALUE;
		}
		f.parID = fsRtParID;
	} else if (err != noErr)
	{
		LastErrorCode = ERROR_BAD_PATHNAME;
		return INVALID_HANDLE_VALUE;
	}
	(**hProgress).volRefNum = f.vRefNum;

	// Find dirID of direcory to be scanned
	(**hProgress).dirID = DirectoryIDFromName(f.vRefNum, f.parID, f.name);

	// Get pattern into hProgress
	if (*split == ':') split++;
	p = &(**hProgress).pattern[0];
	int nPatt = static_cast <int> (
		min((strlen(split)), (sizeof( (**hProgress).pattern) - 1))
		);
	*p++ = static_cast <unsigned char> (nPatt);
	BlockMoveData(split, p, nPatt);

	if (!wildcard)
	{
		//Explicit filename or implicit * ?
		CInfoPBRec		pbd;
		Str31			feName;
		BlockMoveData(&(**hProgress).pattern[0], &feName[0], (**hProgress).pattern[0] + 1);

		pbd.hFileInfo.ioNamePtr = feName;
		pbd.hFileInfo.ioVRefNum = f.vRefNum;
		pbd.hFileInfo.ioDirID = (**hProgress).dirID;
		pbd.hFileInfo.ioFDirIndex = 0;
		err = PBGetCatInfoSync(&pbd);
		if (err) goto NoneFound;
		if (pbd.hFileInfo.ioFlAttrib & ioDirMask)
		{
			// implicit *
			(**hProgress).dirID = pbd.dirInfo.ioDrDirID;
			p = &(**hProgress).pattern[0];
			*p++ = 1; *p = '*';
			wildcard = true;
		} else
		{
			// explicit file name
//			wildcard = false;		Already initialsed to false!
		}
	}

	// Remember wildcard or not for FindNext()
	(**hProgress).wildcard = wildcard;

	// Scan for first file
	CInfoPBRec pb;
	(**hProgress).index = 1;
	if (wildcard)
	{
		Str31 fName;
		do {
			pb.hFileInfo.ioNamePtr = fName;
			pb.hFileInfo.ioVRefNum = (**hProgress).volRefNum;
			pb.hFileInfo.ioDirID = (**hProgress).dirID;
			pb.hFileInfo.ioFDirIndex = (**hProgress).index++;
			err = PBGetCatInfoSync(&pb);
			if (err) goto NoneFound;
		} while (!wildmatch(fName, (**hProgress).pattern));
	} else
	{
		pb.hFileInfo.ioNamePtr = (**hProgress).pattern;
		pb.hFileInfo.ioVRefNum = (**hProgress).volRefNum;
		pb.hFileInfo.ioDirID = (**hProgress).dirID;
		pb.hFileInfo.ioFDirIndex = 0;
		err = PBGetCatInfoSync(&pb);
		if (err) goto NoneFound;
	}

	// Retrieve data for first match
	// Attributes (only directory and hidden are dealt with yet)
	lpFindFileData->dwFileAttributes = 0;
	if (pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsInvisible)
		lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	if (pb.hFileInfo.ioFlAttrib & ioDirMask)
		lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	if (lpFindFileData->dwFileAttributes == 0)
		lpFindFileData->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	//TODO: Are the file times really necessary?

	// File size

	// File Name
	BlockMoveData(&pb.hFileInfo.ioNamePtr[1], lpFindFileData->cFileName,
		pb.hFileInfo.ioNamePtr[0]);
	lpFindFileData->cFileName[pb.hFileInfo.ioNamePtr[0]] = '\0'; // NUL terminate

	// MacOS file information
	lpFindFileData->dwFileType = pb.hFileInfo.ioFlFndrInfo.fdType;
	lpFindFileData->dwCreatorType = pb.hFileInfo.ioFlFndrInfo.fdCreator;
	lpFindFileData->wFinderFlags = pb.hFileInfo.ioFlFndrInfo.fdFlags;
	return (HANDLE)hProgress;

NoneFound:
	lpFindFileData->cFileName[0] = '\0';	// Empty file name
	if (err == noErr) return (HANDLE)hProgress;
	else
	{
		LastErrorCode = ERROR_NO_MORE_FILES;
		return INVALID_HANDLE_VALUE;
	}
}
#pragma export off

// DirectoryIDFromName
// This function obtains the name of a directory given its name and its
// parent dirID.


#define ClearObject(X)    std::memset(&X, 0, sizeof(X))

SInt32 DirectoryIDFromName(
	short			vRef,
	long			parDirID,
	StringPtr		dName)
{
	CInfoPBRec pb;

	ClearObject(pb);

	pb.dirInfo.ioCompletion = 0;
	pb.dirInfo.ioNamePtr = dName;
	pb.dirInfo.ioVRefNum = vRef;
	pb.dirInfo.ioFDirIndex = 0;		// use ioNamePtr
	pb.dirInfo.ioDrDirID = parDirID;

	OSErr error = PBGetCatInfoSync(&pb);
	return(pb.dirInfo.ioDrDirID);
}

// Wildcard testing function.
// Compares the Pascal string fName with the Pascal pattern string wCard
// and returns true if they match, otherwise false.
// Ignores case during comparison using tolower().
// Handles three types of pattern:
//	*		any file name
//	abc*	abc followed by any characters
//	*abc	any characters followed by abc
// Additionally a ? matches any single character

bool	wildmatch(StringPtr fName, StringPtr wCard)
{
	StringPtr			n=fName;	// Pointer into fName
	StringPtr	 		w=wCard;	// Pointer into wCard
	const int			nc=*n++;	// No. of chars in fName
	const int			wc=*w++;	// No. of chars in wCard
									// At this stage n and w point to the
									// first character in each string
	int					k=min(nc, wc);	// Max. no. of chars to scan

	if (*w == '*')
	{
		// Scan backwards from end
		n = &fName[nc];
		w = &wCard[wc];
		for (int i=0; i<k; i++)
		{
			if (*w == '*') return (true);
			if (*w == '?')
			{
				w--; n--;
				continue;
			}
			if (std::tolower(*n--) != std::tolower(*w--)) return (false);
		}
	} else
	{
		// Scan forwards from start
		// w and n were initialised earlier in this function
		for (int i=0; i<k; i++)
		{
			if (*w == '*') return (true);
			if (*w == '?')
			{
				w++; n++;
				continue;
			}
			if (std::tolower(*n++) != std::tolower(*w++)) return (false);
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
//		¥ FindNextFile
// ---------------------------------------------------------------------------
#pragma export on
BOOL FindNextFile(
    HANDLE hFindFile,
    LPWIN32_FIND_DATA lpFindFileData
    )
{
	OSErr					err;			// Mac system error return

	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	MAC_FIND_FILE_PARAMS	** hProgress = (MAC_FIND_FILE_PARAMS **) hFindFile;

	// Scan for next file
	CInfoPBRec pb;
	if ((**hProgress).wildcard)
	{
		Str31 fName;
		do {
			pb.hFileInfo.ioNamePtr = fName;
			pb.hFileInfo.ioVRefNum = (**hProgress).volRefNum;
			pb.hFileInfo.ioDirID = (**hProgress).dirID;
			pb.hFileInfo.ioFDirIndex = (**hProgress).index++;
			err = PBGetCatInfoSync(&pb);
			if (err) goto NoneFound;
		} while (!wildmatch(fName, (**hProgress).pattern));
	} else
	{
		// Details of an explicitly named file would have been returned when
		// FindFirstFile() set up the searching, so any subsequent calls to
		// FindNextFile should return the None Found flag. That way an app which
		// had not noticed that an explicit file name had already been supplied
		// would be brought to a suitable conclusion.
		goto NoneFound;
	}

	// Retrieve data for next match
	// Attributes (only directory and hidden are dealt with yet)
	lpFindFileData->dwFileAttributes = 0;
	if (pb.hFileInfo.ioFlFndrInfo.fdFlags & kIsInvisible)
		lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
	if (pb.hFileInfo.ioFlAttrib & ioDirMask)
		lpFindFileData->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	if (lpFindFileData->dwFileAttributes == 0)
		lpFindFileData->dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	//TODO: Are the file times really necessary?

	// File size

	BlockMoveData(&pb.hFileInfo.ioNamePtr[1], lpFindFileData->cFileName,
		pb.hFileInfo.ioNamePtr[0]);
	lpFindFileData->cFileName[pb.hFileInfo.ioNamePtr[0]] = '\0';
	lpFindFileData->dwFileType = pb.hFileInfo.ioFlFndrInfo.fdType;
	lpFindFileData->dwCreatorType = pb.hFileInfo.ioFlFndrInfo.fdCreator;
	lpFindFileData->wFinderFlags = pb.hFileInfo.ioFlFndrInfo.fdFlags;
	return true;

NoneFound:
	lpFindFileData->cFileName[0] = '\0';	// Empty file name
	LastErrorCode = ERROR_NO_MORE_FILES;
	return false;
}
#pragma export off

// ---------------------------------------------------------------------------
//		¥ FindClose
// ---------------------------------------------------------------------------
#pragma export on
BOOL	FindClose( /*IN OUT*/ HANDLE hFindFile )
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	DisposeHandle(hFindFile);
	return true;
}
#pragma export off

// ---------------------------------------------------------------------------
//		¥ SearchPath
//
//	Searches for a specified file on a specified path. If the file is found the
//	full pathname of the file is copied into the buffer lpbuffer and the number
//	of characters in the path is returned; except that if the buffer supplied
//  (having nBufferLength) is too small to hold the full path the value returned
//	is the size needed to hold the full path (and the application should call
//	SearchPath again but with a large enough buffer).
//
//	If the specified file cannot be found on the specified path, the return value
//	is zero.
// ---------------------------------------------------------------------------
#pragma export on
DWORD	SearchPath(
	LPCSTR lpPath,
	LPCSTR lpFileName,
	LPCSTR lpExtension,
	DWORD nBufferLength,
	LPSTR lpBuffer,
	LPSTR *lpFilePart
	)
{
	// Assume success; change later if necessary
	LastErrorCode = ERROR_SUCCESS;

	char		FFFPath[256];	// Assemble pathname to be passed to FindFirstFile()
	strncpy(FFFPath, lpPath, sizeof(FFFPath)-1);
	// The following line assumes that the Windows app doesn't mind colan separators
	if (FFFPath[strlen(FFFPath)] != ':') strcat(FFFPath, ":");
	strncat(FFFPath, lpFileName, sizeof(FFFPath)-1-strlen(lpPath));

	char *		p;					// Append extension if there is none
	if (!(p=strrchr(FFFPath, '.')))
		strncat(FFFPath, lpExtension, sizeof(FFFPath)-1-strlen(lpPath));

	// Extend for directory the app was launched from?

    WIN32_FIND_DATA	findFileData;
	HANDLE search = FindFirstFile(FFFPath, &findFileData);
	if (search == INVALID_HANDLE_VALUE) goto FileNotFound;
	else
	{
		int total = static_cast <int> (strlen(FFFPath));
		if (total < nBufferLength)
		{
			strcpy(lpBuffer, FFFPath);
			*lpFilePart = (strrchr(lpBuffer, ':') + 1);
		}
		FindClose(search);	// Discard the internal structure used by FindFirst/NextFile

		return static_cast <DWORD> (total);
	}

FileNotFound:
	LastErrorCode = ERROR_FILE_NOT_FOUND;
	return (0);
}
#pragma export off

// String/Memory comparisons
#pragma export on
int	_strnicmp(const char *p, const char *q, size_t n)
{
	while (n-- > 0)
	{
		char c = *p++;
		char d = *q++;

		if (std::toupper(c) != std::toupper(d))
			return c - d;
	}

	return 0;
}

int	_memicmp(const void *vp, const void *vq, size_t n)
{
	const char* p = static_cast<const char*>(vp);
	const char* q = static_cast<const char*>(vq);

	return _strnicmp(p, q, n);
}
#pragma export off

// Copy a Pascal string to a limited-size C string
void p2cstrncpy(const unsigned char* p, char* q, DWORD n)
{
	n = min(n, p[0]);
	BlockMoveData(p + 1, q, static_cast <long> (n));
	q[n] = 0;
}

// Compare two Pascal strings ignoring case
int pstricmp(const unsigned char * p, const unsigned char * q)
{
	int pn = p[0];
	int qn = q[0];

	if (pn == 0 && qn == 0) return 0;
	for (int i=1; i<= min(pn, qn); i++)
	{
		int pc = std::tolower(p[i]);
		int qc = std::tolower(q[i]);
		if (pc != qc) return (pc > qc);
	}
	if (pn > qn ) return 1;
	else
	{
		if (pn < qn) return -1;
		else return 0;
	}
}
