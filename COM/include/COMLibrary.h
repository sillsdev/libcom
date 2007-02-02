// COMLibrary.h
// Created 20061214 MarkS
// Separating the Windows/COM compatibility API from the COM Library functions 
// so I can just include the COM Library functions if I use, for example, 
// the Wine headers instead of our own Windows/COM headers.

#ifndef COMLibrary_h
#define COMLibrary_h

//------------------------------------------------------------------
//
//	Register Server
//
//	Components should call this function to register their class factory
//	with the ComRegistry map. This should be done in the constructor of the
//	server's class factory and the instance of the class factory should be
//	created as a static pointer to the class factory is initialised.
 
extern "C" void RegisterServer(const CLSID &classID, class IClassFactory* classFactory);

#endif /* !COMLibrary_h */

