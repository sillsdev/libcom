// WinBase.h Win32 Complatiablity file 
//TODO REVIEW Add header
#ifndef _WINBASE_H_
#define _WINBASE_H

#pragma once

#include <sys/types.h>

// needs to be included above WinSupport
enum Win32FileAttributes
{
	INVALID_FILE_ATTRIBUTES = 0x00,
	FILE_ATTRIBUTE_READONLY = 0x1,
	FILE_ATTRIBUTE_HIDDEN = 0x2,
	FILE_ATTRIBUTE_SYSTEM = 0x4,
	FILE_ATTRIBUTE_DIRECTORY = 0x10,
	FILE_ATTRIBUTE_ARCHIVE = 0x20,
	FILE_ATTRIBUTE_DEVICE = 0x40,
	FILE_ATTRIBUTE_NORMAL = 0x80,
	FILE_ATTRIBUTE_TEMPORARY = 0x100,
	FILE_ATTRIBUTE_SPARSE_FILE = 0x200,
	FILE_ATTRIBUTE_REPARSE_POINT = 0x400,
	FILE_ATTRIBUTE_COMPRESSED = 0x800,
	FILE_ATTRIBUTE_OFFLINE = 0x1000,
	FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 0x2000,
	FILE_ATTRIBUTE_ENCRYPTED = 0x4000,
	FILE_ATTRIBUTE_VIRTUAL = 0x10000
	
};

// Retuns INVALID_FILE_ATTRIBUTES if the functions fails

//TODO REVIEW include type definitions...
#define WCHAR short unsigned int
#define BOOL long
#define DWORD long int

// returns non zero if the function succeeds
BOOL MoveFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName);

// returns non zero if the function succeeds.
BOOL CopyFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists);

// returns non zero if the function succeeds
BOOL DeleteFile(const WCHAR* lpFileName);

DWORD GetFileAttributes( const WCHAR* lpFileName );

#endif

