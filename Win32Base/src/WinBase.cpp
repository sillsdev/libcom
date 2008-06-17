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
#include <unicode/ustring.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// returns non zero on success
// REVIEW Possible implement this propertly without using system calls
BOOL MoveFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName)
{
	if (lpExistingFileName == NULL || lpNewFileName == NULL)
		return FALSE;

	UErrorCode status = U_ZERO_ERROR;	
	int32_t nSrc1Len = u_strlen(lpExistingFileName);
	int32_t nSrc2Len = u_strlen(lpNewFileName);
	int32_t nDestLen = nSrc1Len + nSrc2Len +1 + 6;
	int32_t tmp;
	char *buffer = new char[ 3+ nSrc1Len + nSrc2Len +1]; // +1 for null term +3 for the cmd
	strcpy(buffer, "mv ");
	u_strToUTF8(buffer + strlen(buffer), nDestLen - strlen(buffer), &tmp, lpExistingFileName, nSrc1Len, &status);
	strcat(buffer, " ");
	u_strToUTF8(buffer + strlen(buffer), nDestLen - strlen(buffer), &tmp, lpNewFileName, nSrc2Len, &status);
	int rv = system(buffer);	
	delete[] buffer;	

	return rv != -1;
}	


// returns non zero on success
// REVIEW Possible implement this propertly without using system calls
// Currently Ignores bFailIfExists
BOOL CopyFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists)
{
	if (lpExistingFileName == NULL || lpNewFileName == NULL)
		return FALSE;

	mode_t uMaskNew = 0;
	mode_t uMaskOld = umask(uMaskNew); // set the new umask so file copies dumplicate permistions

	UErrorCode status = U_ZERO_ERROR;	
	int32_t nSrc1Len = u_strlen(lpExistingFileName);
	int32_t nSrc2Len = u_strlen(lpNewFileName);
	int32_t nDestLen = nSrc1Len + nSrc2Len +1 + 6;
	int32_t tmp;
	char *buffer = new char[ 6 + nSrc1Len + nSrc2Len + 1]; // +1 for null term + 6 for the cmd
	if (bFailIfExists)
		strcpy(buffer, "cp ");
	else
		strcpy(buffer, "cp -f ");
	u_strToUTF8(buffer + strlen(buffer), nDestLen - strlen(buffer), &tmp, lpExistingFileName, nSrc1Len, &status);
	strcat(buffer, " ");
	u_strToUTF8(buffer + strlen(buffer), nDestLen - strlen(buffer), &tmp, lpNewFileName, nSrc2Len, &status);
	int rv = system(buffer);	
	delete[] buffer;

	umask(uMaskOld); // set the old new mask

	return rv != -1;
}

// REVIEW can this method map any other unix permistions to windows file attribues
// Retuns INVALID_FILE_ATTRIBUTES if the functions fails
// TODO REVIEW GetFileAttributes should not use the WCHAR but char *
DWORD GetFileAttributes( const WCHAR* lpFileName )
{
	if (lpFileName == NULL)
		return INVALID_FILE_ATTRIBUTES;

	UErrorCode status = U_ZERO_ERROR;
	int32_t nSrcLen = u_strlen(lpFileName);
	int32_t nDestLen = nSrcLen + 1;
	char *buffer = new char[nSrcLen +1]; // and 1 for null term
	u_strToUTF8(buffer, nDestLen, &nDestLen, lpFileName, nSrcLen, &status);

	struct stat sb;
	if (stat(buffer, &sb) == -1)
	{
		delete[] buffer; // can't auto ptr primitive array.
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
	
	delete[] buffer;
	return rv;
}

// returns non zero if the function succeeds
// Currently only have a W implementation
DWORD GetFileAttributesW( const WCHAR* lpFileName )
{
	return GetFileAttributes(lpFileName);
}

// returns non zero if the function succeeds
// Only actually does anything for the readonly or lack of readonly attribute
BOOL SetFileAttributesW( const WCHAR* lpFileName, DWORD dwFileAttributes)
{
	BOOL rv = false;
	
	if (lpFileName == NULL)
		return rv;

	// Convert to UTF8
	UErrorCode status = U_ZERO_ERROR;
	int32_t nSrcLen = u_strlen(lpFileName);
	int32_t nDestLen = nSrcLen + 1;
	char *buffer = new char[nSrcLen +1]; // and 1 for null term
	u_strToUTF8(buffer, nDestLen, &nDestLen, lpFileName, nSrcLen, &status);
	
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
		if (stat(buffer, &sb) == -1)
		{
			rv = false;
		}
		else
		{
			// remove write permistions for user,group,others
			sb.st_mode = sb.st_mode & ~S_IWUSR & ~S_IWGRP & ~S_IWOTH;
			if (chmod(buffer, sb.st_mode) == -1)
			{
				rv = false;	
			}
			
			rv = true;
		}
	}
	else
	{
		struct stat sb;
		if (stat(buffer, &sb) == -1)
		{
			rv = false;
		}
		else
		{
			// add user and group write permistions.
			sb.st_mode = sb.st_mode & S_IWUSR & S_IWGRP;
			if (chmod(buffer, sb.st_mode) == -1)
			{
				rv = false;	
			}
			
			rv = true;
		}
	}
	
	delete[] buffer;
	return rv;	
}

// returns non zero if the function succeeds
// REVIEW Possible implement this propertly without using system calls
BOOL DeleteFile(const WCHAR* lpFileName)
{		
	if (lpFileName == NULL )
		return FALSE;

	UErrorCode status = U_ZERO_ERROR;
	int32_t nSrcLen = u_strlen(lpFileName);
	int32_t nDestLen = nSrcLen +1;
	char *buffer = new char[ 3 + nSrcLen +1]; // 3 for "rm " and 1 for null term
	strcpy(buffer, "rm ");
	
	u_strToUTF8(buffer + strlen(buffer), nDestLen, &nDestLen, lpFileName, nSrcLen, &status);
	int rv = system(buffer);	
	delete[] buffer;
	return rv != -1;
}
