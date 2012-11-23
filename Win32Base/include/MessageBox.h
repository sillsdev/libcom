#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <ExtendedTypes.h>

enum
{
	MB_BUTTONMASK      = 0xf,
	MB_OK              = 0x0,
	MB_OKCANCEL,
	MB_ABORTRETRYIGNORE,
	MB_YESNOCANCEL,
	MB_YESNO,
	MB_RETRYCANCEL,
	MB_CANCELTRYCONTINUE,

	MB_ICONERROR       = 0x10,
	MB_ICONSTOP        = MB_ICONERROR,
	MB_ICONHAND        = MB_ICONERROR,
	MB_ICONQUESTION    = 0x20,
	MB_ICONWARNING     = 0x30,
	MB_ICONEXCLAMATION = MB_ICONWARNING,
	MB_ICONINFORMATION = 0x40,
	MB_ICONASTERISK    = MB_ICONINFORMATION,

	MB_DEFBUTTON1      = 0x000,
	MB_DEFBUTTON2      = 0x100,
	MB_DEFBUTTON3      = 0x200,
	MB_DEFBUTTON4      = 0x300,

	MB_APPLMODAL       = 0x0000,
	MB_SYSTEMMODAL     = 0x1000,
	MB_TASKMODAL       = 0x2000,

	MB_SETFOREGROUND   = 0x10000,
};

// Standard dialog button IDs

enum
{
	// Windows defines IDOK as 1, but we can't do that with xmessage since that indicates
	// an error
	IDOK = 0,
	IDCANCEL = 2,
	IDABORT = 3,
	IDRETRY = 4,
	IDIGNORE = 5,
	IDYES = 6,
	IDNO = 7,
	IDCLOSE = 8,
	IDHELP = 9,
	IDTRYAGAIN = 10,
	IDCONTINUE = 11
};

int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

#endif
