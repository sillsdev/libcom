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

#include "Hacks.h"

int main(int argc, char** argv)
{
	// \u2022 == \xE2\x80\xA2 == bullet, \u00E9 == e acute
	OutputDebugString(L"Wide\u2022Wide\u2022Wide\n");
	OutputDebugString("Narrow\xE2\x80\xA2Narrow\xe2\x80\xa2Narrow\n");
	OLECHAR buf[] = {'O', 'l', 0xE9, 0x2022, 'O', 'l', 0xE9, '\n', 0};
	OutputDebugString(buf);
}
