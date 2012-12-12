//
//	Implementations needed to compile and run Win32 code on other platforms
//
//	Tom Hindle - 2008-04-28
//
// Win32 Compatibility Library
// Copyright (C) 2008 SIL International
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://www.gnu.org/licenses/lgpl.html
//

#ifndef _WINBASE_H_
#define _WINBASE_H_

#include <sys/types.h>
#include "BasicTypes.h"

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

// Emulations of WIN32 _SECURITY_ATTRIBUTES struct
struct SECURITY_ATTRIBUTES;

// returns TRUE if the function succeeds
BOOL MoveFile(const char*  lpExistingFileName, const char*  lpNewFileName);
BOOL MoveFile(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName);
BOOL MoveFileA(const char*  lpExistingFileName, const char*  lpNewFileName);
BOOL MoveFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName);

// returns TRUE if the function succeeds.
BOOL CopyFile(const char*  lpExistingFileNAme, const char*  lpNewFileName, BOOL bFailIfExists);
BOOL CopyFile(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists);
BOOL CopyFileA(const char*  lpExistingFileNAme, const char*  lpNewFileName, BOOL bFailIfExists);
BOOL CopyFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists);

// returns non zero if the function succeeds
DWORD GetFileAttributes(const char*  lpFileName);
DWORD GetFileAttributes(const WCHAR* lpFileName);
DWORD GetFileAttributesA(const char*  lpFileName);
DWORD GetFileAttributesW(const WCHAR* lpFileName);

// returns TRUE if the function succeeds
BOOL SetFileAttributes(const char*  lpFileName, DWORD dwFileAttributes);
BOOL SetFileAttributes(const WCHAR* lpFileName, DWORD dwFileAttributes);
BOOL SetFileAttributesA(const char*  lpFileName, DWORD dwFileAttributes);
BOOL SetFileAttributesW(const WCHAR* lpFileName, DWORD dwFileAttributes);

// return TRUE if the function succeeds
BOOL CreateDirectory(const char*  lpPathName, SECURITY_ATTRIBUTES* secAttrib);
BOOL CreateDirectory(const WCHAR* lpPathName, SECURITY_ATTRIBUTES* secAttrib);
BOOL CreateDirectoryA(const char*  lpPathName, SECURITY_ATTRIBUTES* secAttrib);
BOOL CreateDirectoryW(const WCHAR* lpPathName, SECURITY_ATTRIBUTES* secAttrib);

// returns TRUE if the function succeeds
BOOL DeleteFile(const char*  lpFileName);
BOOL DeleteFile(const WCHAR* lpFileName);
BOOL DeleteFileA(const char*  lpFileName);
BOOL DeleteFileW(const WCHAR* lpFileName);

#endif //_WINBASE_H_
