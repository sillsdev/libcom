//
//	$Id$
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

#include "WinBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <vector>
#include <unicode/ustring.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <time.h>

namespace
{
	// Local convenience functions

	inline int system(std::string cmd)
	{
		//fprintf(stderr, "%s: system(\"%s\")\n", __FILE__, cmd.c_str());
		int result = ::system(cmd.c_str());
		//fprintf(stderr, "%s: result: %d\n", __FILE__, result);
		return result;
	}

	bool convertToUtf8(const WCHAR* strIn, std::string& strOut)
	{
		if (strIn == 0)
			return false;

		std::vector<char> buffer;
		UErrorCode status;	

		do
		{
			status = U_ZERO_ERROR;
			int32_t requiredLen = buffer.size();
			u_strToUTF8(&buffer[0], buffer.size(), &requiredLen, strIn, -1, &status);
			buffer.resize(requiredLen);
		}
		while (status == U_BUFFER_OVERFLOW_ERROR);

		if (U_FAILURE(status))
			return false;

		strOut.assign(&buffer[0], buffer.size());
		return true;
	}
	
	inline bool fileExists(const char* name)
	{
		return ::access(name, R_OK) == 0;
	}
	
	inline bool fileExists(const WCHAR* lpName)
	{
		std::string name;
		if (!convertToUtf8(lpName, name))
			return false;
		return fileExists(name.c_str());
	}
}

// returns TRUE on success
// REVIEW Possible implement this propertly without using system calls
BOOL MoveFile(const char* lpExistingFileName, const char* lpNewFileName)
{
	if (lpExistingFileName == NULL || lpNewFileName == NULL)
		return FALSE;

	if (!fileExists(lpExistingFileName))
		return FALSE;

	std::string cmd("mv ");

	cmd += "'";
	cmd += lpExistingFileName;
	cmd += "' '";
	cmd += lpNewFileName;
	cmd += "'";

	int rv = system(cmd);	

	return rv != -1;
}	

// returns TRUE on success
BOOL MoveFile(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName)
{
	std::string existingFileName;
	std::string newFileName;

	if (!convertToUtf8(lpExistingFileName, existingFileName))
		return FALSE;
	if (!convertToUtf8(lpNewFileName, newFileName))
		return FALSE;

	return MoveFile(existingFileName.c_str(), newFileName.c_str());
}

BOOL MoveFileA(const char* lpExistingFileName, const char* lpNewFileName)
{
	return MoveFile(lpExistingFileName, lpNewFileName);
}

BOOL MoveFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName)
{
	return MoveFile(lpExistingFileName, lpNewFileName);
}

// returns TRUE if the function succeeds
// REVIEW Possible implement this propertly without using system calls
BOOL CopyFile(const char* lpExistingFileName, const char* lpNewFileName, BOOL bFailIfExists)
{
	if (lpExistingFileName == NULL || lpNewFileName == NULL)
		return FALSE;

	if (!fileExists(lpExistingFileName))
		return FALSE;

	mode_t uMaskNew = 0;
	mode_t uMaskOld = umask(uMaskNew); // set the new umask so file copies duplicate permissions

	std::string cmd("cp ");
	if (!bFailIfExists)
		cmd += "-f ";

	cmd += "'";
	cmd += lpExistingFileName;
	cmd += "' '";
	cmd += lpNewFileName;
	cmd += "'";

	int rv = system(cmd);

	umask(uMaskOld); // set the old new mask

	return rv != -1;
}

// returns TRUE on success
BOOL CopyFile(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists)
{
	std::string existingFileName;
	std::string newFileName;

	if (!convertToUtf8(lpExistingFileName, existingFileName))
		return FALSE;
	if (!convertToUtf8(lpNewFileName, newFileName))
		return FALSE;

	return CopyFile(existingFileName.c_str(), newFileName.c_str(), bFailIfExists);
}

BOOL CopyFileA(const char* lpExistingFileName, const char* lpNewFileName, BOOL bFailIfExists)
{
	return CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}

BOOL CopyFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists)
{
	return CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}

// Retuns INVALID_FILE_ATTRIBUTES if the functions fails
// REVIEW can this method map any other unix permistions to windows file attribues
DWORD GetFileAttributes(const char *lpFileName)
{
	if (lpFileName == NULL)
		return INVALID_FILE_ATTRIBUTES;

	struct stat sb;
	if (stat(lpFileName, &sb) == -1)
	{
		return INVALID_FILE_ATTRIBUTES;
	}

	DWORD rv = 0;

	switch(sb.st_mode & S_IFMT)
	{
		case S_IFDIR:
			rv |= FILE_ATTRIBUTE_DIRECTORY; break;
		case S_IFREG: 
			rv |= FILE_ATTRIBUTE_NORMAL; break;
		default: // The file exists but it not normal file or directoy
			rv |= FILE_ATTRIBUTE_NORMAL; break; 
	}

	// if owner doesn't have write permistion then we count this a readonly
	if (sb.st_mode & S_IWUSR)
		rv &= ~FILE_ATTRIBUTE_READONLY;
	else
		rv |= FILE_ATTRIBUTE_READONLY;
	
	return rv;
}

DWORD GetFileAttributes(const WCHAR* lpFileName)
{
	std::string fileName;

	if (!convertToUtf8(lpFileName, fileName))
		return INVALID_FILE_ATTRIBUTES;

	DWORD rv = GetFileAttributes(fileName.c_str());

	return rv;
}

DWORD GetFileAttributesA(const char *lpFileName)
{
	return GetFileAttributes(lpFileName);
}

DWORD GetFileAttributesW(const WCHAR* lpFileName)
{
	return GetFileAttributes(lpFileName);
}

// returns TRUE if the function succeeds
// Only actually does anything for the readonly or lack of readonly attribute
BOOL SetFileAttributes(const char* lpFileName, DWORD dwFileAttributes)
{
	BOOL rv = false;
	
	if (lpFileName == NULL)
		return rv;
	
	if (dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		rv = true;
	}
	
	if (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		rv = true;	
	}
	
	if (dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
	{
		rv = true;
	}
	
	if (dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	{
		rv = true;
	}

	if (dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)
	{
		rv = true;
	}

	if (dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
	{
		rv = true;
	}

	if (dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		rv = true;
	}
	
	if (dwFileAttributes & FILE_ATTRIBUTE_READONLY)
	{
		struct stat sb;
		if (stat(lpFileName, &sb) == -1)
		{
			rv = false;
		}
		else
		{
			// remove write permistions for user,group,others
			sb.st_mode = sb.st_mode & ~S_IWUSR & ~S_IWGRP & ~S_IWOTH;
			if (chmod(lpFileName, sb.st_mode) == -1)
			{
				rv = false;	
			}
			
			rv = true;
		}
	}
	else
	{
		struct stat sb;
		if (stat(lpFileName, &sb) == -1)
		{
			rv = false;
		}
		else
		{
			// add user and group write permistions.
			sb.st_mode = sb.st_mode | S_IWUSR | S_IWGRP;
			if (chmod(lpFileName, sb.st_mode) == -1)
			{
				rv = false;	
			}
			
			rv = true;
		}
	}
	
	return rv;	
}

BOOL SetFileAttributes(const WCHAR* lpFileName, DWORD dwFileAttributes)
{
	std::string fileName;

	if (!convertToUtf8(lpFileName, fileName))
		return FALSE;

	return SetFileAttributes(fileName.c_str(), dwFileAttributes);
}

BOOL SetFileAttributesA(const char* lpFileName, DWORD dwFileAttributes)
{
	return SetFileAttributes(lpFileName, dwFileAttributes);
}

BOOL SetFileAttributesW(const WCHAR* lpFileName, DWORD dwFileAttributes)
{
	return SetFileAttributes(lpFileName, dwFileAttributes);
}

// return TRUE if the function succeeds
// TODO-P4CL23677-Merge
BOOL CreateDirectory(const char* lpPathName, LPSECURITY_ATTRIBUTES secAttrib)
{
// TODO-P4CL23677-Merge
	assert("CreateDirectory");
	return FALSE;	
}

// return TRUE if the function succeeds
BOOL CreateDirectory(const WCHAR* lpPathName, LPSECURITY_ATTRIBUTES secAttrib)
{
	std::string pathName;

	if (!convertToUtf8(lpPathName, pathName))
		return FALSE;

	return CreateDirectory(pathName.c_str(), secAttrib);
}

BOOL CreateDirectoryA(const char* lpPathName, LPSECURITY_ATTRIBUTES secAttrib)
{
	return CreateDirectory(lpPathName, secAttrib);
}

BOOL CreateDirectoryW(const WCHAR* lpPathName, LPSECURITY_ATTRIBUTES secAttrib)
{
	return CreateDirectory(lpPathName, secAttrib);
}

// returns TRUE if the function succeeds
BOOL DeleteFile(const char* pFileName)
{
	return (0 == remove(pFileName));
}

// returns TRUE if the function succeeds
BOOL DeleteFile(const WCHAR* lpFileName)
{		
	std::string fileName;

	if (!convertToUtf8(lpFileName, fileName))
		return FALSE;

	return DeleteFile(fileName.c_str());
}

BOOL DeleteFileA(const char* lpFileName)
{
	return DeleteFile(lpFileName);
}

BOOL DeleteFileW(const WCHAR* lpFileName)
{
	return DeleteFile(lpFileName);
}

// TODO-P4CL23677-Merge
DWORD GetTickCount()
{
	return (time(NULL) * 1000);
}
