//
//	Hacks tests
//
//	Eberhard Beilharz - 2010-03-09
//
// Win32 Compatibility Library
// Copyright (C) 2010 SIL International
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
#include <errno.h>
#include "TestUtils.h"

int nErr;

void InitBuf(OLECHAR* buf, int len)
{
	for (int i = 0; i < len; i++)
		buf[i] = i + 1;
}

void test_wcsncpy_s()
{
	OLECHAR buf[] = {'O', 'l', 'e', 'O', 'l', 'e', '\n', 0};
	OLECHAR dest[10];

	InitBuf(dest, 10);
	Assert_eq(EINVAL, wcsncpy_s(NULL, 1, buf, 7), "NULL destination should return EINVAL");
	Assert_eq(EINVAL, wcsncpy_s(dest, 0, buf, 7), "destination length of 0 should return EINVAL");

	InitBuf(dest, 10);
	Assert_eq(EINVAL, wcsncpy_s(dest, 10, NULL, 7), "NULL source should return EINVAL");
	Assert_eq(0, dest[0], "NULL source should set strDest[0] to 0");

	InitBuf(dest, 10);
	Assert_eq(EINVAL, wcsncpy_s(dest, 5, buf, 5), "destination to small should return EINVAL");
	Assert_eq(0, dest[0], "destination to small should set strDest[0] to 0");

	InitBuf(dest, 10);
	Assert_eq(0, wcsncpy_s(dest, 10, buf, 7), "copy should succeed");
	Assert_eq(0, dest[7], "copy should set terminating NULL character");

	InitBuf(dest, 10);
	Assert_eq(0, wcsncpy_s(dest, 5, buf, _TRUNCATE), "copy with truncate should succeed");
	Assert_eq('O', dest[0], "copy with truncate should copy characters");
	Assert_eq('l', dest[1], "copy with truncate should copy characters");
	Assert_eq('e', dest[2], "copy with truncate should copy characters");
	Assert_eq('O', dest[3], "copy with truncate should copy characters");
	Assert_eq(0, dest[4], "copy with truncate should set terminating NULL character");

	InitBuf(dest, 10);
	Assert_eq(0, wcsncpy_s(dest, 10, buf, _TRUNCATE), "copy with truncate should succeed if not truncating");
	Assert_eq(0, dest[7], "copy with truncate should set terminating NULL character if not truncating");
}

int main(int argc, char** argv)
{
	nErr = 0;
	test_wcsncpy_s();

	return nErr;
}
