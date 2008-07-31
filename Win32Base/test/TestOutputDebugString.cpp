//
//	$Id$
//
//	OutputDebugString test
//
//	Neil Mayhew - 2007-01-11
//
// Win32 Compatibility Library
// Copyright (C) 2007 SIL International
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

#include <wchar.h>
#include "Hacks.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
	std::cerr.sync_with_stdio(false);
	std::wcerr.sync_with_stdio(false);
	OutputDebugString(L"WideWideWide\n");
	OutputDebugString("NarrowNarrowNarrow\n");
	OLECHAR buf[] = {'O', 'l', 'e', 'O', 'l', 'e', '\n', 0};
	OutputDebugString(buf);
}
