/* *  Copyright � 2000-2002 Metrowerks Corporation.  All Rights Reserved. * *  Questions and comments to: *       <mailto:support@metrowerks.com> *       <http://www.metrowerks.com/> */#include <Carbon/Carbon.h>#include "WinSupportLibFramework.h"enum{	kAlertID = 128};#pragma export onvoid SayHello(void){	NoteAlert(kAlertID, NULL);}#pragma export off