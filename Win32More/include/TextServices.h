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

#ifndef _TEXTSERVICES_H_
#define _TEXTSERVICES_H_

#include "COM.h"

extern GUID GUID_ITextStoreACPSink;
#define IID_ITextStoreACPSink GUID_ITextStoreACPSink
extern GUID GUID_ITextStoreACPServices;
#define IID_ITextStoreACPServices GUID_ITextStoreACPServices
extern GUID GUID_ITextStoreACP;
#define IID_ITextStoreACP GUID_ITextStoreACP
extern GUID GUID_ITfRangeACP;
#define IID_ITfRangeACP GUID_ITfRangeACP
extern GUID GUID_ITfContextOwnerCompositionServices;
#define IID_ITfContextOwnerCompositionServices GUID_ITfContextOwnerCompositionServices
extern GUID GUID_ITfContextOwnerCompositionSink;
#define IID_ITfContextOwnerCompositionSink GUID_ITfContextOwnerCompositionSink
extern GUID GUID_ITfMouseTrackerACP;
#define IID_ITfMouseTrackerACP GUID_ITfMouseTrackerACP
extern GUID GUID_PROP_ATTRIBUTE;
#define IID_PROP_ATTRIBUTE GUID_PROP_ATTRIBUTE

#endif//_TEXTSERVICES_H_
