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

class ITextStoreACPSink;
#define IID_ITextStoreACPSink __uuidof(ITextStoreACPSink)
class ITextStoreACPServices;
#define IID_ITextStoreACPServices __uuidof(ITextStoreACPServices)
class ITextStoreACP;
#define IID_ITextStoreACP __uuidof(ITextStoreACP)
class ITfRangeACP;
#define IID_ITfRangeACP __uuidof(ITfRangeACP)
class ITfContextOwnerCompositionServices;
#define IID_ITfContextOwnerCompositionServices __uuidof(ITfContextOwnerCompositionServices)
class ITfContextOwnerCompositionSink;
#define IID_ITfContextOwnerCompositionSink __uuidof(ITfContextOwnerCompositionSink)
class ITfMouseTrackerACP;
#define IID_ITfMouseTrackerACP __uuidof(ITfMouseTrackerACP)
class PROP_ATTRIBUTE;
#define IID_PROP_ATTRIBUTE __uuidof(PROP_ATTRIBUTE)

#endif//_TEXTSERVICES_H_
