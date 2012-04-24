//
//	$Id$
//
//	Emulation of Windows TextServices APIs
//
//	Neil Mayhew - 2006-10-04
//
// Win32More Compatibility Library
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

#include "TextServices.h"

// These GUID values were generated specifically for libcom. They are different from
// the values used on Windows or by WINE

DEFINE_UUIDOF(ITextStoreACPSink,0x7c6503dd,0xa878,0x49f8,0xbb,0x73,0x09,0x18,0x7c,0x59,0x73,0xdd);
DEFINE_UUIDOF(ITextStoreACPServices,0x6981987b,0xc3af,0x40eb,0x89,0x46,0xf9,0x15,0xbb,0xac,0x86,0xdb);
DEFINE_UUIDOF(ITextStoreACP,0xa875205d,0x2022,0x4750,0xbb,0xa7,0xc8,0xa4,0x93,0x79,0x6b,0x74);
DEFINE_UUIDOF(ITfRangeACP,0x97026dc5,0xc8bb,0x4a80,0x93,0x21,0x7e,0x60,0x58,0x09,0x72,0x3b);
DEFINE_UUIDOF(ITfContextOwnerCompositionServices,0x536a1b4d,0x7971,0x440b,0xb1,0x5f,0xd1,0xda,0x18,0x1a,0xe3,0xb7);
DEFINE_UUIDOF(ITfContextOwnerCompositionSink,0x1c72b3fb,0x8b2b,0x4095,0xb7,0x64,0x88,0xa5,0xd4,0x37,0xec,0x4e);
DEFINE_UUIDOF(ITfMouseTrackerACP,0x54419b2d,0x0028,0x484e,0xb4,0xe3,0xae,0xf7,0xb6,0x58,0x26,0xed);
DEFINE_UUIDOF(PROP_ATTRIBUTE,0xc87a336c,0x915a,0x40df,0x98,0xfe,0x4c,0xbe,0xc4,0x94,0x9a,0x09);
