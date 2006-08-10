/*
 *	Hacks.cpp
 *
 *	Implementations needed to compile and run Win32 code on other platforms
 *
 *	Neil Mayhew - 21 Apr 2005
 *
 *	$Id$
 */

#include "Hacks.h"

int WideCharToMultiByte(int, int, const OLECHAR*, int, char*, int, char*, char*);
int MultiByteToWideChar(int, int, const char*, int, OLECHAR*, int);

bool IsBadStringPtrW(const OLECHAR*, unsigned long) {}
bool IsBadStringPtrA(const char*, unsigned long) {}
bool IsBadReadPtr(const void*, unsigned long) {}
bool IsBadWritePtr(const void*, unsigned long) {}

short ClientToScreen(HWND, POINT*);
short ScreenToClient(HWND, POINT*);

void GetSystemTimeAsFileTime(FILETIME*);
BOOL SystemTimeToFileTime(SYSTEMTIME*, FILETIME*);
BOOL FileTimeToSystemTime(FILETIME*, SYSTEMTIME*);
DWORD GetTickCount();

void InitializeCriticalSection(CRITICAL_SECTION*);
void DeleteCriticalSection(CRITICAL_SECTION*);
void EnterCriticalSection(CRITICAL_SECTION*);
void LeaveCriticalSection(CRITICAL_SECTION*);

// Support functions

int GetFullPathName(const char*, int, char*, const char**);
int GetFullPathName(const OLECHAR*, int, OLECHAR*, const OLECHAR**);
