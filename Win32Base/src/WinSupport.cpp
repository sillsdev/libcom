/*----------------------------------------------------------------------------------------------
Copyright 2001, SIL International. All rights reserved.

File: WinSupport.cpp
Responsibility: Graeme Costin
Reviewed:	Date		Reviewer
			never

C++ file to provide substitutions and kludges when compiling FieldWorks for MacOS.

Requires WinSupport.h and WinSupportInternals.h.

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

#include "WinSupportInternals.h"

#include <String.h>
#include <assert.h>
#include <cstdlib>
#include <cctype>
#include <UTCUtils.h>
#include <OSUtils.h>

#include <LString.h>

//	LockResource(), LoadResource(), SizeofResource() and FindClose()
//	are defined later in this file.

// ---------------------------------------------------------------------------
//		¥ GetSystemTimeAsFileTime
// ---------------------------------------------------------------------------
//	Returns the system time as a 64-bit signed integer being the number of
//	100-nanosecond periods since 12:00AM on January 1, 1601.

void GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
	UTCDateTime		time;
	
	//	Get no. of seconds (top 48 bits) since midnight January 1, 1904
	//	This is unsigned.
	::GetUTCDateTime(&time, kUTCDefaultOptions);

	UTCMacToWin(time, lpSystemTimeAsFileTime);
}

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
	tWin->dwHighDateTime = (time64 & 0xFFFFFFFF00000000) >> 32;
	tWin->dwLowDateTime = time64 & 0xFFFFFFFF;
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

DWORD GetTimeZoneInformation( LPTIME_ZONE_INFORMATION lpTimeZoneInformation)
{
	MachineLocation		loc;

	ReadLocation(&loc);

	TIME_ZONE_INFORMATION	tZone;
	//	Most fields of the Windows time zone information cannot be filled from
	//	values read from the MacOS MachineLocation, so we set the lot to zero
	//	before setting the one item we can deal with.
	memset(&tZone, 0, sizeof tZone);
	
	//	gmtDelta
	//	The signed integer is contained in the lower 3 bytes of the long.
	//	The sign needs to be extended through the upper byte; this is done
	//	by shifting left and then shifting right - arithmetic shift right
	//	extends the sign bit.
	//
	//	The MacOS gmtDelta is in seconds; the Windows bias is in minutes.
	//
	//	The MacOS gmtDelta is added to GMT to get local time; the Windows
	//	Bias is added to local time to get UTC - hence the sign change.
	tZone.Bias = - ((((loc.u.gmtDelta & 0xFFFFFF) << 8) >>8) / 60);
	
	*lpTimeZoneInformation = tZone;
	//	This is the return value for no daylight saving on WinNT, 2000, XP.
	//	We don't have the information to deal with daylight saving items.
	return TIME_ZONE_ID_UNKNOWN;
}

// ---------------------------------------------------------------------------
//		¥ GetTickCount
// ---------------------------------------------------------------------------
//	This function returns the number of milliseconds since the system was booted.

DWORD GetTickCount(void)
{
	union {
		struct {
			UInt32	high;
			UInt32	low;
		} split;
		UInt64	joined;
	};
	split.low = ::TickCount();
	split.high = 0;
	
	return static_cast <DWORD> (joined * 100 / 6);
}

DWORD FormatMessage(
    /*IN*/ DWORD	dwFlags,
    /*IN*/ LPCVOID	lpSource,
    /*IN*/ DWORD	dwMessageId,
    /*IN*/ DWORD	dwLanguageId,
    /*OUT*/ LPSTR	lpBuffer,
    /*IN*/ DWORD	nSize,
    /*IN*/ va_list	*Arguments
    );

//	Facility for obtaining the file the application was launched from

ResFileDetails::ResFileDetails()
{
	refNum = CurResFile();

	FCBPBRec pb;
	pb.ioNamePtr = spec.name;
	pb.ioFCBIndx = 0;
	pb.ioRefNum = refNum;
	PBGetFCBInfoSync(&pb);
	spec.parID = pb.ioFCBParID;
	spec.vRefNum = pb.ioFCBVRefNum;
}

static ResFileDetails portApp; // Hopefully, this gets initialised before any other res files are opened

// ---------------------------------------------------------------------------
//		¥ GetModuleFileName
// ---------------------------------------------------------------------------
//	This function returns the full pathname of the application's file.
//	If hModule is NULL the path for the application's file is returned;
//	otherwise the pathname for the resource file hModule is returned.
//	It is likely that for MacFieldWorks these two cases will be the same.
//	TODO: This function can be extended if it is ever necessary.
DWORD	GetModuleFileName(
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize)
{
	short		vRefNum;		// volume ref number
	long		parID;			// parent directory ID
	Str63		name;			// file name
	OSErr		err;			// error code
	
	if (hModule == NULL) {
		// Get details of application's file
		vRefNum = portApp.spec.vRefNum;
		parID = portApp.spec.parID;
		unsigned char *p = &portApp.spec.name[0];
		unsigned char *q = &name[0];
		short i = -1;	//	First byte copied is the length byte.
		for (;i < portApp.spec.name[0]; i++) *q++ = *p++;
	} else {
		// Get details of the specified resource file
		FCBPBRec pb;
		pb.ioNamePtr = name;
		pb.ioFCBIndx = 0;
		pb.ioRefNum = hModule;
		err = PBGetFCBInfoSync(&pb);
		assert(err == noErr);	//	If hModule is not a valid file reference number
								//	then hModule has been mishandled somewhere else,
								//	probably in ModuleEntry where these ref numbers
								//	are obtained from MacOS as the app or module
								//	starts up.
		parID = pb.ioFCBParID;
		vRefNum = pb.ioFCBVRefNum;
	}
	
	LStr255		fullPath("\p");	// Initialize full pathname
	LStr255		dirName;		// Directory name

 	CInfoPBRec	pb;
	pb.dirInfo.ioNamePtr = &dirName[0];
    pb.dirInfo.ioVRefNum = vRefNum;	// Indicate target volume
    pb.dirInfo.ioDrParID = parID;	// Initialize parent directory ID
    pb.dirInfo.ioFDirIndex = -1;	// Indicate info about a directory

    // Get name of each parent directory, up to root directory.
	do {
		pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
		err = PBGetCatInfoSync(&pb);

		dirName = dirName + "\p:";
		fullPath = dirName + fullPath;
	} while (pb.dirInfo.ioDrDirID != fsRtDirID);

	//	Append file name
	fullPath = fullPath + name;

	// Return full pathname as C string
	short			n = fullPath[0];	// No. of chars returned
	short			i;
	unsigned char	*p = &fullPath[1];
	char			*q = lpFilename;
	
	for (i=0; i<fullPath[0]; i++) {
		*q++ = *p++;
		if (i == (nSize-2)) {
			n = i + 1;
			break;
		}
	}
	*q = '\0';	// NUL terminate the C string
	return n;
}

// ---------------------------------------------------------------------------
//		¥ GetModuleHandle
// ---------------------------------------------------------------------------
//	This function returns the refNum of the application's resource fork.
//	The only parameter implemented is NULL; the rest of the Windows' behaviour
//	is not needed yet.
//	TODO: This function can be extended if it is ever necessary.
HMODULE GetModuleHandle(LPCSTR lpModuleName)
{
	assert(lpModuleName == NULL);
	return portApp.refNum;
}

// ---------------------------------------------------------------------------
//		¥ OutputDebugString
// ---------------------------------------------------------------------------
VOID OutputDebugString(LPCSTR lpOutputString)
{
}

// ---------------------------------------------------------------------------
//		¥ FindResource
// ---------------------------------------------------------------------------
//	TODO: Extend this for other types of resources if necessary.
HRSRC	FindResource(void*, int rid, const char* type)
{
	if (std::strcmp( type, "TEMPLATE") == 0)
	{
		HRSRC	hrTmpl;

		if (!(hrTmpl = GetResource('TEXT', rid))) return NULL;
		return hrTmpl;
	}
	else return NULL;	// Other resource types not yet kludged!
}
					 
// Directory scan kludges

// ---------------------------------------------------------------------------
//		¥ FindFirstFile
// ---------------------------------------------------------------------------
HANDLE	FindFirstFile(
    LPCSTR lpFileName,
    LPWIN32_FIND_DATA lpFindFileData
    )
{
	MAC_FIND_FILE_PARAMS	**hProgress =
					(MAC_FIND_FILE_PARAMS**)NewHandle(sizeof(MAC_FIND_FILE_PARAMS));

	
	int				n = strlen(lpFileName);	// Length of supplied pathname
	unsigned char	pathName[MAX_PATH];		// Buffer for Pascal string pathname
//	PascalString<255>	pathName(lpFileName);		// Buffer for Pascal string pathname
	bool			wildcard = false;		// True if pattern is a wildcard
	OSErr			err;					// Mac system error return
	unsigned char	*p;						// Pointers for C->Pascal string copying
	const char		*q;

	// Get pattern into progress
	const char* split = strrchr(lpFileName, PATH_SEPARATOR);
	if (!split) split = lpFileName;
	else if (*split == ':') split++;
	p = &(**hProgress).pattern[0];
	*p++ = min(lpFileName + n - split, sizeof((**hProgress).pattern) - 1);
	BlockMoveData(split, p, (**hProgress).pattern[0]);
//	(**hProgress).pattern = split;
	
	// Test for wildcard
	const char * ast = strchr(split, '*');	// Point to asterisk if any
	if (strlen(split) && 					// empty wildcard not acceptable
		(ast || strchr(split, '?')) &&		// must have an '*' or a '?'
		(ast ? !strchr(ast, '*') : true)	// not more than one '*'
		) wildcard = true;

	// Get pathname into Pascal string buffer
	p = pathName;
	*p++ = min(n, sizeof(pathName) - 1);
	q = lpFileName;
	BlockMoveData(q, p, n);
	
	// Find volRefNum and dirID of parent directory
	FSSpec			f;
	FSMakeFSSpec(0, 0, pathName, &f);
	(**hProgress).volRefNum = f.vRefNum;
	(**hProgress).dirID = f.parID;

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
	BlockMoveData(&pb.hFileInfo.ioNamePtr[1], lpFindFileData->cFileName,
		pb.hFileInfo.ioNamePtr[0]);
	lpFindFileData->cFileName[pb.hFileInfo.ioNamePtr[0]] = '\0';
	lpFindFileData->dwFileType = pb.hFileInfo.ioFlFndrInfo.fdType;
	lpFindFileData->dwCreatorType = pb.hFileInfo.ioFlFndrInfo.fdCreator;
	lpFindFileData->wFinderFlags = pb.hFileInfo.ioFlFndrInfo.fdFlags;
	return (HANDLE)hProgress;
NoneFound:
	lpFindFileData->cFileName[0] = '\0';	// Empty file name
	return INVALID_HANDLE_VALUE;
}

// Wildcard testing function.
// Compares the Pascal string fName with the Pascal pattern string wCard
// and returns true if they match, otherwise false.
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
			if (*n-- != *w--) return (false);
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
			if (*n++ != *w++) return (false);
		}
	}
	return false;
}

// ---------------------------------------------------------------------------
//		¥ FindClose
// ---------------------------------------------------------------------------
BOOL	FindClose( /*IN OUT*/ HANDLE hFindFile )
{
	DisposeHandle(hFindFile);
	return true;
}

// ---------------------------------------------------------------------------
//		¥ SearchPath
// ---------------------------------------------------------------------------
DWORD	SearchPath(
	LPCSTR lpPath,
	LPCSTR lpFileName,
	LPCSTR lpExtension,
	DWORD nBufferLength,
	LPSTR lpBuffer,
	LPSTR *lpFilePart
	)
{
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
	if (search == INVALID_HANDLE_VALUE) return (0);
	else
	{
		int total = strlen(FFFPath);
		if (total < nBufferLength)
		{
			strcpy(lpBuffer, FFFPath);
			*lpFilePart = (strrchr(lpBuffer, ':') + 1);
		}
		FindClose(search);	// Discard the internal structure used by FindFirst/NextFile

		return total;
	}
}

// String/Memory comparison
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
