/*
 *	$Id$
 *
 *	Emulation of Windows TextServices APIs
 *
 *	Neil Mayhew - 04 Oct 2006
 */

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
