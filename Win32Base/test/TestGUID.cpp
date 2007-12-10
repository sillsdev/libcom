//
//	$Id$
//
//	GUID test
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

#include "COM.h"
#include <iostream>
#include <iomanip>

std::ostream& operator << (std::ostream& out, const PlainGUID& plain)
{
	using std::setw;

	out << std::hex << std::setfill('0') << std::setiosflags(std::ios::uppercase);
	
	out << "{";

	out << setw(8) << plain.Data1 << "-"
	    << setw(4) << plain.Data2 << "-"
	    << setw(4) << plain.Data3 << "-";

	for (int i = 0; i != sizeof(plain.Data4); ++i)
		out << std::setw(2) << int(plain.Data4[i]);

	out << "}";
	
	return out;
}

int main(int argc, char** argv)
{
	using std::cout;
	
	cout << "Generate new GUID\n";
	GUID guid(true);
	cout << guid << "\n";

	cout << "Construct Smart from Plain\n";
	PlainGUID plain = { 0x0c733a30, 0x2a1c, 0x11ce, {0xad, 0xe5, 0x00, 0xaa, 0x00, 0x44, 0x77, 0x3d} };
	SmartGUID smart(plain);
	cout << smart << "\n";

	cout << "Assign Plain from Plain\n";
	PlainGUID plain2 = plain;
	cout << plain2 << "\n";

	cout << "Assign Smart from Plain\n";
	smart = plain;
	cout << smart << "\n";

	cout << "Assign Plain from Smart\n";
	plain = smart;
	cout << plain << "\n";
}
