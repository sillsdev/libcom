//
//	Test utilities
//
//	Eberhard Beilharz - 2010-03-19
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

#ifndef TESTUTILS_H
#define TESTUTILS_H

#define Assert_eq(expected, actual, msg) \
	if ((actual) != (expected)) \
	{ \
		std::cerr << "FAIL: " << (msg) << "; expected " << (expected) << ", but was " << (actual) << "." << std::endl; \
		nErr++; \
	}

#endif
