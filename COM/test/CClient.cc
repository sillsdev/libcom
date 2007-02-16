/* Ball COM Support Library Test
 * Copyright (C) 2007 SIL
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

// CClient.cc
// Created 20061212 MarkS
// Designed to test against libballmodule.so and our FakeCOMLibrary
// Using some of what was learned in .../code/tests/wineinvoke/CWi/cwi.c
// when g++ing, make sure to " -rdynamic -ldl "
// hm, instantaneously: g++ CClient.c GUIDs.cpp -rdynamic -ldl
// make libballmodule && g++ CCliet.c GUIDs.cpp -rdynamic -ldl && LD_PRELOAD=./libballmodule.so ./a.out
// this works, praised God :D. it reports dist as 55 :D
// Modified 20070119 MarkS
// Working in the fwCB_THREE test, mono seemed to make a Ball but failed to 
// poke the Ball.bounce() function. To help me understand why not, I'll use 
// a C++ way of working with the COM Ball to try to track down the problem.
// I'll need to modify this file to make it work with the now way of doing 
// things from the pCB2 to fwCB_THREE tests.
// Modified 20070124 MarkS
// LGPL'd.
// Modified 20070130 MarkS
// Cleaning up.
 

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include <COMInterfaces.h>

#include "IBall_idl.h"
#include "Ball.h"

int main() {

  void *handle;
  const char* error;
	// variable describing the function that we will want to call
  HRESULT (*CoCreateInstance)(
		/*IN*/ REFCLSID		requestedClassID,
		/*IN*/ LPUNKNOWN	outerAggregateIUnknown,
		/*IN*/ DWORD		dwClsContext,
		/*IN*/ REFIID		objectInterfaceID,
		/*OUT*/ LPVOID*		objectInterface);

  REFCLSID		requestedClassID = CLSID_Ball; // the class id that I'm looking for
  LPUNKNOWN	outerAggregateIUnknown = NULL;
  DWORD		dwClsContext = (DWORD)CLSCTX_INPROC_SERVER;
  REFIID		objectInterfaceID = IID_IBall;
  LPVOID*		objectInterface = NULL; // we want a pointer to an IUnknown interface
  HRESULT hr;

  handle = dlopen("libcom.so", RTLD_LAZY);

	if (!handle)
  {
    error = dlerror();
    fprintf (stderr, "ERROR: %s\n", error);
    exit(1);
  }

  *(void **) (&CoCreateInstance) = dlsym(handle, "CoCreateInstance");
  if (NULL != (error = dlerror()))
  {
    fprintf(stderr, "ERROR: %s\n", error);
    exit(1);
  }

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp

	hr = 123;
  hr = (*CoCreateInstance)(
		requestedClassID,
		outerAggregateIUnknown, // (IUnknown *)NULL,
		dwClsContext,
		objectInterfaceID, 
		(void **)&objectInterface);

  printf("hr is %d (0x%x)\n", hr, hr);

	if (SUCCEEDED(hr)) {

	  // I'm not familiar with casting with these templates, so this may do well to be changed.
	  reinterpret_cast<IBall*>(objectInterface)->bounce();
	
		long dist;
		reinterpret_cast<IBall*>(objectInterface)->roll(20, &dist);
	
	  printf("dist is %d\n.",dist);

		fprintf(stderr, "Success!\n");

	} else {
		fprintf(stderr,"hr did not succeed.\n");
	}

  dlclose(handle);
  return 0;
}
