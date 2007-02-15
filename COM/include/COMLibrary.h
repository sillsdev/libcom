/*
 *	$Id$
 *
 *	Functions unique to this COM Support Library
 *
 *	MarkS - 2007-02-05
 *
 */

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

