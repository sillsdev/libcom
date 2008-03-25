/*
 *	$Id$
 *
 *	COM utilities
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
#include <BasicTypes.h>

// TODO not a hack
#define DECLARE_HANDLE(NAME) typedef struct NAME##__ *NAME;
DECLARE_HANDLE(HINSTANCE);
typedef HINSTANCE HMODULE;

static wchar_t module_name[] = L"Unknown Module";

unsigned long GetModuleFileName(HMODULE, wchar_t* buf, unsigned long max)
{
	std::wcsncpy(buf, module_name, max);
	buf[max - 1] = 0;
	return std::wcslen(buf);
}

unsigned long GetModuleFileName(HMODULE h, OLECHAR* buf, unsigned long max)
{
	std::vector<wchar_t> tmp(max);
	unsigned long count = GetModuleFileName(h, &tmp[0], max);
	std::copy(tmp.begin(), tmp.end(), buf);
	return count;
}
