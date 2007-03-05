/*
 *	$Id$
 *
 *	Main COM Support Library code
 *
 *	Graeme Costin - 2001-05
 *
 * COM Support Library
 * Copyright (C) 2001, 2007 SIL International
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

#include "Types.h"
#include "WinError.h"
#include "COMInterfaces.h"

#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cctype>

#include "COMSupportInternals.h"
#include "COMLibrary.h"

#include <map>
#include <dlfcn.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>

#include <stdexcept>

#include "ComRegistry.h"

inline void swap(unsigned char &a, unsigned char &b) {
	char tmp = a;
	a = b;
	b = tmp;
}

/**
 * @brief Change a little endian GUID to a big endian GUID.
 * 
 * @param guid little endian GUID
 * @return big endian GUID based on guid
 */
GUID mangle_guid(GUID guid) {

	// this is implemented quickly and dirtily

	unsigned char tmp;
	
	//unsigned char* data = guid.str().c_str();
	unsigned char data[37];
	//const char* origGuid = guid.str().c_str();
	
	memcpy(data,reinterpret_cast<void*>(&guid),37);
	int last = sizeof(long)-1;

	for (int i=0;i< (last+1)/2 ; i++) {
		tmp = data[i];
		data[i] = data[last-i];
		data[last-i] = tmp;
	}
	
	//fprintf(stderr,"data8 is %c.\n", data[8]);
	swap(data[4],data[5]);
	swap(data[6],data[7]);
	//fprintf(stderr,"data8 is now %c.\n",data[8]);
	
	// bswap_32/16 would be better, but doesn't seem to be working for me
	
	//GUID origguidguid = origGuid;
	//GUID origguidguid;
	//CLSIDFromString(origGuid, origguidguid);
	//return origguidguid;
	
	memcpy((void*)&guid,data,37);
	return guid;
}

/**
 * @brief Register a class factory by class ID in the internal registry.
 * 
 * "Components should call this function to register their class factory
 * with the ComRegistry map. This should be done in the constructor of the
 * server's class factory and the instance of the class factory should be
 * created as a static pointer to the class factory is initialised." Or 
 * we will register components we load.
 * @param classID class ID to register
 * @param classFactory class factory to register that can create objects of class ID classID.
 */
#pragma export on
void RegisterFactory(const CLSID &classID, LPCLASSFACTORY classFactory)
{
	ComRegistry::get_instance()->register_factory(classID, classFactory);
}
#pragma export off

