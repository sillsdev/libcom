/*
 *	$Id$
 *
 *	C++ COM Ball Client example and test
 *
 *	MarkS - 2006-12-12
 *
 * Ball COM Support Library Test
 * Copyright (C) 2007 SIL International
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include <COMInterfaces.h>

#include "IBall.h"
#include "Ball.h"

#include <COMError.h>

int main() {

  void* handle;
  const char* error;
	// variable describing the function that we will want to call
  HRESULT (*CoCreateInstance)(
		REFCLSID requestedClassID,
		LPUNKNOWN outerAggregateIUnknown,
		DWORD dwClsContext,
		REFIID objectInterfaceID,
		LPVOID* objectInterface);

  REFCLSID requestedClassID = CLSID_Ball; // the class id that I'm looking for
  LPUNKNOWN outerAggregateIUnknown = NULL;
  DWORD dwClsContext = (DWORD)CLSCTX_INPROC_SERVER;
  REFIID objectInterfaceID = IID_IBall;
  LPVOID* objectInterface = NULL; // we want a pointer to an IUnknown interface
  HRESULT hr;
  bool success = false;

  handle = dlopen("libcom.so", RTLD_LAZY);

	if (!handle)
  {
    error = dlerror();
    fprintf(stderr, "ERROR: %s\n", error);
    exit(1);
  }

  *(void **) (&CoCreateInstance) = dlsym(handle, "CoCreateInstance");
  if (NULL != (error = dlerror()))
  {
    fprintf(stderr, "ERROR: %s\n", error);
    exit(1);
  }

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/com/html/0f171cf4-87b9-43a6-97f2-80ed344fe376.asp

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

	  printf("dist is %ld\n",dist);

		fprintf(stderr, "Success!\n");
		success = true;

	} else {
		fprintf(stderr,"hr did not succeed.\n");
	}

  dlclose(handle);
  return success ? 0 : 1;
}
