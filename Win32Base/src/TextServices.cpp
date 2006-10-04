/*
 *	$Id$
 *
 *	Emulation of Windows TextServices APIs
 *
 *	Neil Mayhew - 04 Oct 2006
 */

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
