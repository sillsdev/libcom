//	ComponentGUIDs.cpp
//
//	Adapted from GUIDs.cpp, Rogerson's book Chapter 7
//   - Defines all IIDs and CLSIDs for the client and the component.
//     The declaration of these GUIDs is in Iface.h
//
#include "windows.h"

// {32bb8320-b41b-11cf-a6bb-0080c7b2d682}
extern "C" const IID IID_IX = 
	{0x32bb8320, 0xb41b, 0x11cf,
	{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82}} ;

// {32bb8321-b41b-11cf-a6bb-0080c7b2d682}
extern "C" const IID IID_IY = 
	{0x32bb8321, 0xb41b, 0x11cf,
	{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82}} ;

// {32bb8322-b41b-11cf-a6bb-0080c7b2d682}
extern "C" const IID IID_IZ = 
	{0x32bb8322, 0xb41b, 0x11cf,
	{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82}} ;

// {0c092c21-882c-11cf-a6bb-0080c7b2d682}
extern "C" const CLSID CLSID_Component1 =
	{0x0c092c21, 0x882c, 0x11cf,
	{0xa6, 0xbb, 0x0, 0x80, 0xc7, 0xb2, 0xd6, 0x82}} ;
