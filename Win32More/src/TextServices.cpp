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

// These all create their GUIDs dynamically - no good for multiple independent components
// ToDo: create and assign GUIDs statically
GUID GUID_ITextStoreACPSink(true);
GUID GUID_ITextStoreACPServices(true);
GUID GUID_ITextStoreACP(true);
GUID GUID_ITfRangeACP(true);
GUID GUID_ITfContextOwnerCompositionServices(true);
GUID GUID_ITfContextOwnerCompositionSink(true);
GUID GUID_ITfMouseTrackerACP(true);
GUID GUID_PROP_ATTRIBUTE(true);
