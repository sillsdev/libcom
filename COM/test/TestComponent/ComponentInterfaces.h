//	ComponentInterfaces.h
//
//	Adapted from Iface.h from Rogerson's book Chapter 7
//	- Declarations of interfaces, IIDs, and CLSID
//    shared by the client and the component.
//

#undef pascal
#define pascal

interface IX : IUnknown
{
	virtual void pascal Fx() = 0 ;
};


interface IY : IUnknown
{
	virtual void pascal Fy() = 0 ;
};


interface IZ : IUnknown
{
	virtual void pascal Fz() = 0 ;
};

//
// Declaration of GUIDs for interfaces and component.
//   These constants are defined in GUIDs.cpp.
//
extern "C" const IID IID_IX ;
extern "C" const IID IID_IY ;
extern "C" const IID IID_IZ ;

extern "C" const CLSID CLSID_Component1 ;
