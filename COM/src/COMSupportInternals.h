/*
 *	$Id$
 *
 *	COM Support Library internals
 *
 *	Graeme Costin - 2001-08
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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


#ifndef COMSupportInternals_h
#define COMSupportInternals_h

#include <map>
#include <string>
#include <algorithm>
using std::string;

// OLECHAR overload of wcslen
inline size_t wcslen(const OLECHAR* s)
{
	// Point to the top of the virtual address space
	const OLECHAR* no_end = (const OLECHAR*)0 - 1;
	return std::find(s, no_end, 0) - s;
}

#endif // COMSupportInternals_h
