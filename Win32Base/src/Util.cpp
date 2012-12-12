/*
 *	Win32 utilities
 *
 *	MarkS - 2008-03-20
 *
 * COM Support Library Test
 * Copyright (C) 2007 SIL International
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

// Support functions

// Temporary, until we find a way of doing this properly on Unix

#include <cwchar>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include <BasicTypes.h>

// TODO not a hack
#define DECLARE_HANDLE(NAME) typedef struct NAME##__ *NAME;
DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

static wchar_t module_name[] = L"Unknown Module";

unsigned int GetModuleFileName(HMODULE, wchar_t* buf, unsigned int max)
{
	void * frames[2];
	int size = backtrace(frames, 2);

	char ** symbols = backtrace_symbols(frames, size);
	if (size > 1)
	{
		// Get name of module (executing binary) from stack. We skip the first frame because
		// that's this current method (and thus libcom).
		char* name = symbols[1];
		char* p = strchr(name, '(');
		if (p)
			*p = '\0';
		p = strrchr(name, '/');
		if (p)
			name = p + 1;
		size_t n = std::min(strlen(name), (size_t)max - 1);
		std::copy(name, name + n, buf);
		buf[n] = 0;
	}
	else
	{
		std::wcsncpy(buf, module_name, max - 1);
		buf[max - 1] = 0;
	}
	free (symbols);

	return std::wcslen(buf);
}

unsigned int GetModuleFileName(HMODULE h, OLECHAR* buf, unsigned int max)
{
	std::vector<wchar_t> tmp(max);
	unsigned int count = GetModuleFileName(h, &tmp[0], max);
	std::copy(tmp.begin(), tmp.end(), buf);
	return count;
}
